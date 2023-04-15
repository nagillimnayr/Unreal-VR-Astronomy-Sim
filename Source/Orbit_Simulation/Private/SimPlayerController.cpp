// Fill out your copyright notice in the Description page of Project Settings.


#include "SimPlayerController.h"

#include "AstroBody.h"
#include "Orbit.h"
//#include "Simulation.h"
#include "SimulationSubsystem.h"
#include "System.h"
#include "Orbit_Simulation/SimGameMode.h"
#include "EnhancedInputComponent.h"
#include "SimInputConfigData.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Observer.h"
#include "ObservationPoint.h"
#include "SelectableInterface.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "CelestialSphere.h"

// Define collision channel for selection
#define COLLISION_SELECTION_CHANNEL ECC_GameTraceChannel1


void ASimPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to Simulation Subsystem
	Simulation = GetWorld()->GetSubsystem<USimulationSubsystem>();
	if(!Simulation)
	{
		UE_LOG(LogTemp, Warning, TEXT("> Error: Could not get Simulation Subsystem"));
	}

	// Get MaterialParameterCollection Instance
	if(MaterialParameterCollection)
	{
		GlobalMaterialParameters = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	}

	Possess(System);
}

void ASimPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(!IsValid(InPawn)) return;
	
	//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, FString("Possessing: ") + InPawn->GetActorLabel());

	// Attempt to cast to AAstroBody
	if(AAstroBody* Body = Cast<AAstroBody>(InPawn))
	{
		// If successful, nothing needs to be done
		return;
	}
	
	// Attempt to cast to ASystem
	if(ASystem* SystemPawn = Cast<ASystem>(InPawn))
	{
		// If Possessing an ASystem, Possess its Primary Body instead
		/*AAstroBody* PrimaryBody = SystemPawn->GetPrimaryBody();
		if(PrimaryBody)
		{
			//SetViewTargetWithBlend(PrimaryBody);
			Possess(PrimaryBody);
		}*/

		// Do nothing
		return;
	}
	// Attempt to cast to AOrbit
	if(AOrbit* OrbitPawn = Cast<AOrbit>(InPawn))
	{
		// If Possessing an AOrbit, Possess its Orbiting Body instead
		if(AAstroBody* OrbitingBody = OrbitPawn->GetOrbitingBody())
		{
			//SetViewTargetWithBlend(OrbitingBody);
			Possess(OrbitingBody);
		}
	}
	/*// Attempt to cast to Observer
	else if(AObserver* Observer = Cast<AObserver>(InPawn))
	{
		SetControlRotation(Observer->GetActorRotation());
	}*/
}

void ASimPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}


void ASimPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	// Show Mouse Cursor
	SetShowMouseCursor(true);
	
	// Switch pawn
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_0"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 0);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_1"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 1);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_2"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 2);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_3"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 3);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_4"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 4);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_5"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 5);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_6"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 6);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_7"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 7);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_8"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 8);
	InputComponent->BindAction<FInputSwitchPawnDelegate>(FName("Index_9"), IE_Pressed, this, &ASimPlayerController::SwitchPawn, 9);

	
	InputComponent->BindAction<FInputIncrementTimeScaleDelegate>(FName("IncreaseTimeScale"), EInputEvent::IE_Pressed, this, &ASimPlayerController::IncreaseTimeScale, 1);
	InputComponent->BindAction<FInputIncrementTimeScaleDelegate>(FName("DecreaseTimeScale"), EInputEvent::IE_Pressed, this, &ASimPlayerController::IncreaseTimeScale, -1);
	
	// Rotate camera
	//InputComponent->BindAxis(FName("YawAxis"),this, &ASimPlayerController::CameraYaw);
	//InputComponent->BindAxis(FName("PitchAxis"),this, &ASimPlayerController::CameraPitch);

	// Setup Enhanced Input
	SetInputMappingContext(SpaceViewInputMappingContext);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	check(InputActions->InputYaw &&
		InputActions->InputPitch &&
		InputActions->InputZoom &&
		InputActions->InputMouseClick &&
		InputActions->InputFreeLook);

	// SpaceView
	EnhancedInput->BindAction(InputActions->InputYaw, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraYaw);
	EnhancedInput->BindAction(InputActions->InputPitch, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraPitch);
	EnhancedInput->BindAction(InputActions->InputZoom, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraZoom);
	// On Mouse Click
	EnhancedInput->BindAction(InputActions->InputMouseClick, ETriggerEvent::Triggered, this, &ASimPlayerController::OnMouseClick);

	// Surface View
	EnhancedInput->BindAction(InputActions->InputFreeLook, ETriggerEvent::Triggered, this, &ASimPlayerController::FreeLook);
	
}

void ASimPlayerController::Select(AActor* NewSelectedActor)
{
	check(NewSelectedActor);

	UE_LOG(LogTemp, Warning, TEXT("> Selected: %s"), *NewSelectedActor->GetActorLabel());
	
	// Deselect currently selected actor
	Deselect();
	
	ISelectableInterface* NewSelection = Cast<ISelectableInterface>(NewSelectedActor);
	// If NewSelectedActor does not implement ISelectableInterface, then something has gone wrong, check trace channels
	if(!NewSelection)
	{
		GLog->Log("> Error!: Selected Object does not implement ISelectableInterface.");
		return;
	}

	// If Actor does implement the interface, call its select function and set it as the SelectedActor
	NewSelection->Select();
	SelectedActor = NewSelectedActor;

	// Broadcast Selection Delegate
	OnSelectDelegate.Broadcast(SelectedActor);
}

void ASimPlayerController::Deselect()
{
	// If an actor is currently selected, deselect them
	if(IsValid(SelectedActor))
	{
		// Attempt to cast currently selected actor to ISelectableInterface
		if(ISelectableInterface* Selected = Cast<ISelectableInterface>(SelectedActor))
		{
			// Deselect Selected Actor
			Selected->Deselect();
		}

		// Nullify pointer
		SelectedActor = nullptr;
	}
}

void ASimPlayerController::OnMouseClick()
{
	// Use Raycast to determine if a selectable object has been clicked on
	FHitResult HitResult;
	GetHitResultUnderCursor(COLLISION_SELECTION_CHANNEL, false, HitResult);
	
	AActor* HitActor = HitResult.GetActor();
	// If no valid target was hit, deselect current selected item
	if(!IsValid(HitActor))
	{
		// Deselect the currently selected actor
		Deselect();
		OnUnSelectDelegate.Broadcast(); // Broadcast deselection delegate
		return;
	}

	Select(HitActor);
}

void ASimPlayerController::SwitchPawn(const int Index)
{
	if(!IsValid(System)) {return;}
	if(Index < 0) {return;}

	if(Index == 0)
	{
		Possess(System->GetPrimaryBody());
		return;
	}
	
	TArray<AOrbit*> Orbits = System->GetOrbits();
	if(Index - 1 >= Orbits.Num()) return;

	Possess(Orbits[Index - 1]);
}

void ASimPlayerController::CameraZoom(const FInputActionValue& AxisInput)
{
	// Get the controlled Pawn
	APawn* ControlledPawn = GetPawn();
	if(!IsValid(ControlledPawn)) {return;}

	// Get the SpringArm of the Controlled Pawn, if it has one
	USpringArmComponent* SpringArm = Cast<USpringArmComponent>(ControlledPawn->GetComponentByClass(USpringArmComponent::StaticClass()));
	if(!SpringArm) {return;}

	//
	const double CurrentArmLength = SpringArm->TargetArmLength;
	const double InputAmount = AxisInput.Get<float>();
	const double ZoomAmount = CurrentArmLength * 0.02 * InputAmount;

	// Interpolate between current zoom level and target zoom level
	const double NewLength = SpringArm->TargetArmLength + FMath::FInterpTo(0.0, ZoomAmount, GetWorld()->GetDeltaSeconds(), LookSpeed);
	//SpringArm->TargetArmLength = FMath::Clamp(NewLength, 100.0, 100000.0); // Clamp the value
	SpringArm->TargetArmLength = NewLength;
}

void ASimPlayerController::CameraYaw(const FInputActionValue& AxisInput)
{
	const double Value = AxisInput.Get<float>();
	const double InputValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed);
	AddYawInput(InputValue);
}

void ASimPlayerController::CameraPitch(const  FInputActionValue& AxisInput)
{
	const double Value = AxisInput.Get<float>();
	const double InputValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed / 2.0);
	AddPitchInput(InputValue);
}

void ASimPlayerController::FreeLook(const FInputActionValue& AxisInput)
{
	const FVector2D InputVector = AxisInput.Get<FVector2D>();
	
	/*const double YawValue = FMath::FInterpTo(0.0, InputVector.X, GetWorld()->GetDeltaSeconds(), LookSpeed / 2.0);
	const double PitchValue = FMath::FInterpTo(0.0, InputVector.Y, GetWorld()->GetDeltaSeconds(), LookSpeed / 2.0);

	AddYawInput(YawValue);
	AddPitchInput(PitchValue);*/
	
	AddYawInput(InputVector.X);
	AddPitchInput(InputVector.Y);
	
}

void ASimPlayerController::SetSimulation(USimulationSubsystem* Sim)
{
	Simulation = Sim;
}

void ASimPlayerController::SetSystem(ASystem* NewSystem)
{
	System = NewSystem;
}

void ASimPlayerController::IncreaseTimeScale(const int Amount)
{
	check(Simulation);
	Simulation->IncreaseTimeScale(Amount);
}

void ASimPlayerController::TransitionToSurface()
{
	check(IsValid(ObserverSubclass));
	check(IsValid(ObservationPointSubclass));
	check(IsValid(CelestialSphereSubclass));
	
	// These shouldn't be possible, so something has gone wrong
	checkf(SelectedActor, TEXT("> Error: Selected Actor is invalid. Something has gone wrong."));
	AAstroBody* Body = Cast<AAstroBody>(SelectedActor); // Cast to Body
	checkf(Body, TEXT("> Error: Selected Actor is not an AAstroBody. Something has gone wrong."));
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Spawn new Observer actor
	AObserver* Observer = Cast<AObserver>(GetWorld()->SpawnActor(ObserverSubclass,
		0,
		0,
		SpawnParameters
		));

	// Spawn new ObservationPoint Actor
	AObservationPoint* ObservationPoint = Cast<AObservationPoint>(
		GetWorld()->SpawnActor(ObservationPointSubclass,
		0,
		0,
		SpawnParameters
		));

	// Attach the observer to the observation point
	Observer->AttachToActor(ObservationPoint, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
		));
	
	// Attach observation point to the Body
	Body->SetObservationPoint(ObservationPoint);

	// Spawn Celestial Sphere
	ACelestialSphere* CelestialSphere = Cast<ACelestialSphere>(
		GetWorld()->SpawnActor(CelestialSphereSubclass,0,0,SpawnParameters));

	CelestialSphere->AttachToActor(Body, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		false
		));

	//CelestialSphere->SetActorScale3D(FVector(Body->GetOrbit()->GetSemiMajorAxis() * 2.0));
	
	CelestialSphere->SetActorScale3D(FVector(Body->GetMeanRadius() * 20.0));

	CelestialSphere->SetParentBody(Body);
	
	// Possess the observer
	Possess(Observer);

	// Switch Input Mapping Context
	SetInputMappingContext(SurfaceViewInputMappingContext);
	
	// Broadcast Delegate
	OnTransitionToSurfaceDelegate.Broadcast();


	// Disable Cursor
	SetShowMouseCursor(false);

	// Set Global Material Parameters
	if(GlobalMaterialParameters)
	{
		GlobalMaterialParameters->SetScalarParameterValue(TEXT("ConstellationFigureOpacity"), 0.2);
		GlobalMaterialParameters->SetScalarParameterValue(TEXT("CelestialGridOpacity"), 0.2);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("> Error: Material Parameter Collection Instance is invalid"));
	}


	SurfaceBody = Body;
	SurfaceBody->GetRootComponent()->SetVisibility(true, true);
	// Set Particle Trail Visibility
	SurfaceBody->SetParticleTrailInvisibile();
	ObservationPoint->GetRootComponent()->SetVisibility(true, true);
}

void ASimPlayerController::TransitionToSpace()
{
	// These shouldn't be possible, so something has gone wrong
	//checkf(SelectedActor, TEXT("> Error: Selected Actor is invalid. Something has gone wrong."));
	//AAstroBody* Body = Cast<AAstroBody>(SelectedActor); // Cast to Body
	//(Body, TEXT("> Error: Selected Actor is not an AAstroBody. Something has gone wrong."));

	// Get Observer
	AObserver* Observer = Cast<AObserver>(GetPawn());
	checkf(Observer, TEXT("> Error: Possessed Pawn is not an AObserver. Something has gone wrong."))

	// Possess Body
	Possess(SurfaceBody);

	Observer->Destroy(); // Destroy Observer

	//SurfaceBody->RemoveObservationPoint(); // Destroy Observation Point
	
	// Switch Input Mapping Context
	SetInputMappingContext(SpaceViewInputMappingContext);

	// Enable Cursor
	SetShowMouseCursor(true);
	
	// Broadcast Delegate
	OnTransitionToSpaceDelegate.Broadcast(); // Should destroy the AObservationPoint and ACelestialSphere
	

	// Set Global Material Parameters
	if(GlobalMaterialParameters)
	{
		GlobalMaterialParameters->SetScalarParameterValue(TEXT("ConstellationFigureOpacity"), 0.0);
		GlobalMaterialParameters->SetScalarParameterValue(TEXT("CelestialGridOpacity"), 0.0);
	}
	
	// Set Particle Trail Visibility
	SurfaceBody->SetParticleTrailVisibility(true);

	Select(SurfaceBody);
	SurfaceBody = nullptr;
}

void ASimPlayerController::SetInputMappingContext(UInputMappingContext* IMC)
{
	// Get the local player subsystem
	check(IMC);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->ClearAllMappings(); // Clear previous Mappings
	Subsystem->AddMappingContext(IMC, 0);
}


