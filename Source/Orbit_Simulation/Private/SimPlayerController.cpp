// Fill out your copyright notice in the Description page of Project Settings.


#include "SimPlayerController.h"

#include "AstroBody.h"
#include "Orbit.h"
#include "Simulation.h"
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

// Define collision channel for selection
#define COLLISION_SELECTION_CHANNEL ECC_GameTraceChannel1


void ASimPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASimPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(!IsValid(InPawn)) return;
	
	// Attempt to cast to ASystem
	if(ASystem* SystemPawn = Cast<ASystem>(InPawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, SystemPawn->GetFName().ToString());
		AAstroBody* PrimaryBody = SystemPawn->GetPrimaryBody();
		if(PrimaryBody)
		{
			//SetViewTargetWithBlend(PrimaryBody);
			Possess(PrimaryBody);
			//CameraBoom = PrimaryBody->GetCameraBoom();
		}
		
	}
	// Attempt to cast to AOrbit
	else if(AOrbit* OrbitPawn = Cast<AOrbit>(InPawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, OrbitPawn->GetActorLabel());
		if(AAstroBody* OrbitingBody = OrbitPawn->GetOrbitingBody())
		{
			//SetViewTargetWithBlend(OrbitingBody);
			Possess(OrbitingBody);
			//CameraBoom = OrbitingBody->GetCameraBoom();
		}
	}
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

	// Rotate camera
	//InputComponent->BindAxis(FName("YawAxis"),this, &ASimPlayerController::CameraYaw);
	//InputComponent->BindAxis(FName("PitchAxis"),this, &ASimPlayerController::CameraPitch);

	
	// Get the local player subsystem
	check(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	check(InputActions->InputYaw &&
		InputActions->InputPitch &&
		InputActions->InputZoom);
	
	EnhancedInput->BindAction(InputActions->InputYaw, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraYaw);
	EnhancedInput->BindAction(InputActions->InputPitch, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraPitch);
	EnhancedInput->BindAction(InputActions->InputZoom, ETriggerEvent::Triggered, this,  &ASimPlayerController::CameraZoom);

	// On Mouse Click
	EnhancedInput->BindAction(InputActions->InputMouseClick, ETriggerEvent::Triggered, this, &ASimPlayerController::OnMouseClick);
}

void ASimPlayerController::Select(AActor* NewSelectedActor)
{
	// Deselect currently selected actor
	Deselect();
	
	ISelectableInterface* NewSelection = Cast<ISelectableInterface>(NewSelectedActor);
	// If NewSelectedActor does not implement ISelectableInterface, then something has gone wrong, check trace channels
	if(!NewSelection)
	{
		GLog->Log("> Error!: Selected Object does not implement ISelectableInterface.");
		return;
	}

	// If Actor does implement the interface, call its select function and set it as the SelectedObject
	NewSelection->Select();
	
	SelectedActor = NewSelectedActor;
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

		SelectedActor = nullptr;
	}
}

void ASimPlayerController::OnMouseClick()
{
	//GLog->Log("> Mouse Clicked!");
	// Use Raycast to determine if a selectable object has been clicked on
	FHitResult HitResult;
	GetHitResultUnderCursor(COLLISION_SELECTION_CHANNEL, false, HitResult);
	
	AActor* HitActor = HitResult.GetActor();
	// If no valid target was hit, deselect current selected item
	if(!IsValid(HitActor))
	{
		//GLog->Log("> No Actor hit!");
		// Deselect the currently selected actor
		Deselect();
		return;
	}

	//GLog->Log("> Hit Actor: " + HitActor->GetActorLabel());

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
	APawn* ControlledPawn = GetPawn();
	if(!IsValid(ControlledPawn)) {return;}

	USpringArmComponent* SpringArm = Cast<USpringArmComponent>(ControlledPawn->GetComponentByClass(USpringArmComponent::StaticClass()));
	if(!SpringArm) {return;}

	const double CurrentArmLength = SpringArm->TargetArmLength;
	const double ZoomAmount = AxisInput.Get<float>();
	const double ZoomIncrease = CurrentArmLength * 0.01 * ZoomAmount;
	
	SpringArm->TargetArmLength += FMath::FInterpTo(0.0, ZoomIncrease, GetWorld()->GetDeltaSeconds(), LookSpeed);
}

void ASimPlayerController::CameraYaw(const FInputActionValue& AxisInput)
{
	const double Value = AxisInput.Get<float>();
	GLog->Log("Input Axis Yaw Value: " + FString::SanitizeFloat(Value));
	const double InputValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed);
	AddYawInput(InputValue);
}

void ASimPlayerController::CameraPitch(const  FInputActionValue& AxisInput)
{
	const double Value = AxisInput.Get<float>();
	//GLog->Log("Input Axis Value: " + FString::SanitizeFloat(FloatVal));
	const double InputValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed / 2.0);
	AddPitchInput(InputValue);
}

void ASimPlayerController::SetSimulation(ASimulation* Sim)
{
	Simulation = Sim;
}

void ASimPlayerController::SetSystem(ASystem* NewSystem)
{
	System = NewSystem;
}

void ASimPlayerController::GoToSurface()
{
	check(IsValid(ObserverSubclass));
	check(IsValid(ObservationPointSubclass));
	if(!IsValid(SelectedActor)) return;

	AOrbit* Orbit = Cast<AOrbit>(SelectedActor);
	if(!IsValid(Orbit)) {return;}
	AAstroBody* Body = Orbit->GetOrbitingBody();
	check(Body);
	
	// Create new Observer actor
	AObserver* Observer = Cast<AObserver>(GetWorld()->SpawnActor(ObserverSubclass));

	// Create new ObservationPoint Actor
	AObservationPoint* ObservationPoint = Cast<AObservationPoint>(GetWorld()->SpawnActor(ObservationPointSubclass));

	// Attach observer to observation point
	Observer->AttachToActor(ObservationPoint, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		false
	));

	Possess(Observer);
	// Attach observation point to Body
	Body->GoToSurface(ObservationPoint);
}

void ASimPlayerController::GoToSpace()
{
	
}

