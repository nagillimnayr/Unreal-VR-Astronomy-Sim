// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"
#include "System.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"

// Sets default values
AAstroBody::AAstroBody() :
Mass(0.0),
VelocityVector(FVector::ZeroVector),
AccelerationVector(FVector::ZeroVector),
Size(1.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(SceneRoot);
		SphereMesh->CastShadow = false; // Disable shadows

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		SphereMesh->SetRelativeLocation(FVector::ZeroVector);
	}
	//StaticSphereMesh->SetWorldScale3D(FVector(Radius, Radius, Radius)); // Set size of Sphere

	// Initialize arrows
	AccelerationArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Acceleration-Arrow"));
	if (AccelerationArrow)
	{
		AccelerationArrow->SetupAttachment(SceneRoot);
		AccelerationArrow->SetVisibility(true, true);
		AccelerationArrow->SetHiddenInGame(false, true);
		AccelerationArrow->SetArrowColor(FLinearColor::Red);
		AccelerationArrow->SetRelativeLocation(FVector::ZeroVector);
		AccelerationArrow->SetWorldLocation(GetActorLocation());
		AccelerationArrow->ArrowSize = 2.0;
		AccelerationArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		AccelerationArrow->ArrowLength = 75.0;
		AccelerationArrow->bUseInEditorScaling = false;
	}
	
	VelocityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Velocity-Arrow"));
	if (VelocityArrow)
	{
		VelocityArrow->SetupAttachment(SceneRoot);
		VelocityArrow->SetVisibility(true, true);
		VelocityArrow->SetHiddenInGame(false, true);
		VelocityArrow->SetArrowColor(FLinearColor::Green);
		VelocityArrow->SetRelativeLocation(FVector::ZeroVector);
		VelocityArrow->ArrowSize = 2.0;
		VelocityArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		VelocityArrow->ArrowLength = 75.0;
		VelocityArrow->bUseInEditorScaling = false;
	}

	// Disable collision
	SetActorEnableCollision(false);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize Niagara System/ Component
	if (TrailSystem)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, SphereMesh, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
			false);
		TrailComponent->SetVisibility(true);
		TrailComponent->SetHiddenInGame(false);
		TrailComponent->SetColorParameter(TEXT("Color"), Color);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
	}
	
	SphereMesh->CastShadow = false; // Disable shadows
	AccelerationArrow->SetHiddenInGame(true);
	VelocityArrow->SetHiddenInGame(true);
}

void AAstroBody::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	SphereMesh->SetWorldScale3D(FVector(Size, Size, Size)); // Set size of Sphere
	
	
	/*AccelerationArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AccelerationArrow->SetRelativeLocation(FVector::ZeroVector);
	AccelerationArrow->ArrowLength = 75 + (25 * Radius);
	
	VelocityArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	VelocityArrow->SetRelativeLocation(FVector::ZeroVector);
	VelocityArrow->ArrowLength = 75 + (25 * Radius);*/
	
	SphereMesh->CastShadow = false; // Disable shadows
	
	// Disable Collision
	SetActorEnableCollision(false);


	AccelerationArrow->SetRelativeRotation(FRotator::ZeroRotator);

	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, 90.0));
	VelocityArrow->SetRelativeRotation(Rotation);

	// Set Color

}


/*void AAstroBody::CalculateAcceleration(const AAstroBody* const OtherBody)
{
	const FVector OtherPos = OtherBody->GetActorLocation();
	FVector ThisPos = GetActorLocation();
	FVector Difference = OtherPos - ThisPos;
	double DistanceSquared = (Difference * Unit::DISTANCE_MULT).SquaredLength(); // Multiply by DISTANCE_MULTIPLIER
	//FVector Direction = Difference.GetSafeNormal(1.0); // Normalized Direction Vector pointing from this Body to the other Body
	FVector Direction = Difference;
	Direction.Normalize(); // Normalized Direction Vector pointing from this Body to the other Body
	
	// Calculate gravitational force
	double Force = (Unit::GRAVITATIONAL_CONSTANT * OtherBody->mass * Unit::SOLAR_MASS) / DistanceSquared;
	AccelerationVector = Direction * Force;

	AccelerationMagnitude = Force;
	
	UpdateAccelerationArrow();
	//AccelerationArrow->ArrowLength = FMath::Max(0.1, AccelerationMagnitude);
}*/

void AAstroBody::SetAcceleration(const FVector NewAcceleration)
{
	AccelerationVector = NewAcceleration;
	AccelerationMagnitude = AccelerationVector.Length();
	UpdateAccelerationArrow();
}

void AAstroBody::UpdateVelocity(const FVector NewVelocity)
{
	VelocityVector = NewVelocity;
	OrbitalSpeed = VelocityVector.Length();
	UpdateVelocityArrow();
	//VelocityArrow->ArrowLength = FMath::Max(0.1, OrbitalSpeed);
}

void AAstroBody::UpdatePosition(const double DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update Position based on Velocity
	// Divide by DISTANCE_MULTIPLIER so the result will be in in-editor units
	Position += VelocityVector * (DeltaTime * Unit::SECONDS_IN_DAY  / Unit::DISTANCE_MULT);
	const bool bCheck = SetActorLocation(Position);
	if (!bCheck)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Updating Position"));
	}
}

void AAstroBody::UpdateVelocityArrow()
{
	if(!VelocityArrow) {return;}
	const FRotator Rotation = FRotationMatrix::MakeFromX(VelocityVector).Rotator();
	VelocityArrow->SetWorldRotation(Rotation);
}

void AAstroBody::UpdateAccelerationArrow()
{
	if(!AccelerationArrow) { return; }
	const FRotator Rotation = FRotationMatrix::MakeFromX(AccelerationVector).Rotator();
	AccelerationArrow->SetWorldRotation(Rotation);
}

void AAstroBody::AimAccelerationArrow(const AActor* const Target)
{
	const FVector Direction = Target->GetActorLocation() - GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	AccelerationArrow->SetWorldRotation(Rotation);
	
}

void AAstroBody::SetColor(const FLinearColor NewColor)
{
	Color = NewColor;
	if(TrailComponent)
		TrailComponent->SetColorParameter(TEXT("Color"), Color);
}

// Called every frame
void AAstroBody::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAstroBody::PostInitProperties()
{
	Super::PostInitProperties();

}

#if WITH_EDITOR
void AAstroBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
