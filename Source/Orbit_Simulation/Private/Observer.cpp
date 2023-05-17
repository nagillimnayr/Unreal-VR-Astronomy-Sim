// Fill out your copyright notice in the Description page of Project Settings.


#include "Observer.h"

#include "EnhancedInputComponent.h"
#include "SimInputConfigData.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SimPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AObserver::AObserver()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	// Initialize Capsule
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetupAttachment(SceneRoot);
	Capsule->SetRelativeLocation(FVector(0.0, 0.0, Capsule->GetScaledCapsuleHalfHeight()));
	
	// Initialize Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(SceneRoot);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->TargetArmLength = 0.0;*/
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SceneRoot);
	// Position the Camera at the center of the Capsule
	//CameraBoom->SetRelativeLocation(FVector(0.0, 0.0, Capsule->GetScaledCapsuleHalfHeight()));

}

// Called when the game starts or when spawned
void AObserver::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObserver::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	/*CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->TargetArmLength = 0.0;*/
	Camera->bUsePawnControlRotation = false;
	
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetRelativeLocation(FVector(0.0, 0.0, Capsule->GetUnscaledCapsuleHalfHeight()));
	Camera->SetRelativeLocation(FVector(0.0, 0.0, Capsule->GetUnscaledCapsuleHalfHeight() * 1.5));
}

// Called every frame
void AObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AObserver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputActions);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInput->BindAction(InputActions->InputYaw, ETriggerEvent::Triggered, this,  &AObserver::CameraYaw);
	EnhancedInput->BindAction(InputActions->InputPitch, ETriggerEvent::Triggered, this,  &AObserver::CameraPitch);
	EnhancedInput->BindAction(InputActions->InputFreeLook, ETriggerEvent::Triggered, this, &AObserver::FreeLook);

}

void AObserver::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/*Camera->bUsePawnControlRotation = false;
	if(ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(NewController))
	{
		
	}*/
}

void AObserver::UnPossessed()
{
	Super::UnPossessed();
	
	//Camera->bUsePawnControlRotation = false;
}

void AObserver::CameraYaw(const FInputActionValue& AxisInput)
{
	const double Value = -AxisInput.Get<float>();
	const double YawValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed);

	
	/*const double YawValue = -AxisInput.Get<float>();*/
	
	const FRotator Yaw = FRotator::MakeFromEuler(FVector(0.0, 0.0, YawValue));
	
	//Camera->AddLocalRotation(Yaw);
	Camera->AddRelativeRotation(Yaw);
}

void AObserver::CameraPitch(const FInputActionValue& AxisInput)
{
	const double Value = AxisInput.Get<float>();
	const double PitchValue = FMath::FInterpTo(0.0, Value, GetWorld()->GetDeltaSeconds(), LookSpeed);
	
	/*const double PitchValue = AxisInput.Get<float>();*/
	
	const FRotator Pitch = FRotator::MakeFromEuler(FVector(0.0, PitchValue, 0.0));
	Camera->AddRelativeRotation(Pitch);
}

void AObserver::FreeLook(const FInputActionValue& AxisInput)
{
	
	const FVector2D InputVector = AxisInput.Get<FVector2D>();
	
	const double YawValue = FMath::FInterpTo(0.0, InputVector.X, GetWorld()->GetDeltaSeconds(), LookSpeed);
	const double PitchValue = FMath::FInterpTo(0.0, InputVector.Y, GetWorld()->GetDeltaSeconds(), LookSpeed);

	const FRotator Yaw = FRotator::MakeFromEuler(FVector(0.0, 0.0, YawValue));
	const FRotator Pitch = FRotator::MakeFromEuler(FVector(0.0, PitchValue, 0.0));

	Camera->AddRelativeRotation(Yaw);
	Camera->AddRelativeRotation(Pitch);
	
}

