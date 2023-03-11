// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"
#include "Sim.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AAstroBody::AAstroBody() :
mass(0.0),
VelocityVector(FVector::ZeroVector),
AccelerationVector(FVector::ZeroVector),
Size(1.0),
Orbit(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);
	StaticSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	StaticSphereMesh->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Succeeded())
	{
		StaticSphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		StaticSphereMesh->SetRelativeLocation(FVector::ZeroVector);
		StaticSphereMesh->CastShadow = false; // Disable shadows
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

	// Initialize SpotLight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpotLightBoom"));
	SpotLightBoom->SetupAttachment(RootComponent); // Attach boom to root
	SpotLight->SetupAttachment(SpotLightBoom); // Attach SpotLight to boom
	
}

// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize Niagara System/ Component
	if (TrailSystem)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, StaticSphereMesh, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
			false);
		TrailComponent->SetVisibility(true);
		TrailComponent->SetHiddenInGame(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
	}
	
	StaticSphereMesh->CastShadow = false; // Disable shadows
	AccelerationArrow->SetHiddenInGame(true);
	VelocityArrow->SetHiddenInGame(true);
}

void AAstroBody::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	StaticSphereMesh->SetWorldScale3D(FVector(Size, Size, Size)); // Set size of Sphere
	
	
	/*AccelerationArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AccelerationArrow->SetRelativeLocation(FVector::ZeroVector);
	AccelerationArrow->ArrowLength = 75 + (25 * Radius);
	
	VelocityArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	VelocityArrow->SetRelativeLocation(FVector::ZeroVector);
	VelocityArrow->ArrowLength = 75 + (25 * Radius);*/
	
	StaticSphereMesh->CastShadow = false; // Disable shadows

	// Set the Attenuation Radius of the SpotLight to be relative to the boom's arm length and the body's size
	SpotLightBoom->TargetArmLength = 300 * Size;
	double SpotlightRadius = SpotLightBoom->TargetArmLength + (50.0 * Size);
	SpotLight->SetAttenuationRadius(SpotlightRadius);
	SpotLight->SetOuterConeAngle(15.0);
}

void AAstroBody::UpdateSpotLight(AActor* Source)
{
	FVector Direction = GetActorLocation() - Source->GetActorLocation(); // Get Direction Vector to Target
	FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator(); // Get Rotator from Direction Vector
	SpotLightBoom->SetWorldRotation(Rotation); // Set new rotation

	// Aim SpotLight
	//UpdateSpotLight();
}

void AAstroBody::CalculateAcceleration(AAstroBody* OtherBody)
{
	FVector OtherPos = OtherBody->GetActorLocation();
	FVector ThisPos = GetActorLocation();
	FVector Difference = OtherPos - ThisPos;
	double DistanceSquared = (Difference * ASim::DISTANCE_MULTIPLIER).SquaredLength(); // Multiply by DISTANCE_MULTIPLIER
	//FVector Direction = Difference.GetSafeNormal(1.0); // Normalized Direction Vector pointing from this Body to the other Body
	FVector Direction = Difference;
	Direction.Normalize(); // Normalized Direction Vector pointing from this Body to the other Body
	
	// Calculate gravitational force
	double Force = (ASim::GRAVITATIONAL_CONSTANT * OtherBody->mass * ASim::SOLAR_MASS) / DistanceSquared;
	AccelerationVector = Direction * Force;

	AccelerationMagnitude = Force;
	
	UpdateAccelerationArrow();
	//AccelerationArrow->ArrowLength = FMath::Max(0.1, AccelerationMagnitude);
}

void AAstroBody::UpdateVelocity(const double DeltaTime)
{
	VelocityVector += AccelerationVector * (DeltaTime * ASim::SECONDS_IN_DAY);
	OrbitalSpeed = VelocityVector.Length() / ASim::KM_TO_M;
	
	UpdateVelocityArrow();
	//VelocityArrow->ArrowLength = FMath::Max(0.1, OrbitalSpeed);
}

void AAstroBody::UpdatePosition(const double DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update Position based on Velocity
	// Divide by DISTANCE_MULTIPLIER so the result will be in in-editor units
	Position += VelocityVector * (DeltaTime * ASim::SECONDS_IN_DAY  / ASim::DISTANCE_MULTIPLIER);
	bool check = SetActorLocation(Position);
	if (!check)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Updating Position"));
	}
}

void AAstroBody::UpdateVelocityArrow()
{
	if(!VelocityArrow) {return;}
	FRotator Rotation = FRotationMatrix::MakeFromX(VelocityVector).Rotator();
	VelocityArrow->SetWorldRotation(Rotation);
}

void AAstroBody::UpdateAccelerationArrow()
{
	if(!AccelerationArrow) { return; }
	FRotator Rotation = FRotationMatrix::MakeFromX(AccelerationVector).Rotator();
	AccelerationArrow->SetWorldRotation(Rotation);
}

// Called every frame
void AAstroBody::Tick(float DeltaTime)
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
