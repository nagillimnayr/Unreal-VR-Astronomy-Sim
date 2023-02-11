// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"
#include "Sim.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AAstroBody::AAstroBody() :
	mass(0.0),
	InitialVelocity(0.0),
	OrbitalVelocity(FVector::ZeroVector),
	Acceleration(FVector::ZeroVector)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	//StaticSphereMesh->SetupAttachment(RootComponent);
	SetRootComponent(StaticSphereMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (SphereMeshAsset.Succeeded())
	{
		StaticSphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		StaticSphereMesh->SetRelativeLocation(FVector::ZeroVector);
	}

	// Initialize arrow
	XArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("X-Arrow"));
	XArrow->SetupAttachment(RootComponent);
	XArrow->SetVisibility(true, true);
	XArrow->SetHiddenInGame(false, true);
	XArrow->SetArrowColor(FLinearColor::Red);
	XArrow->SetRelativeLocation(FVector::ZeroVector);
	XArrow->ArrowSize = 2.0;
	XArrow->ArrowLength = 75.0;
	XArrow->bUseInEditorScaling = false;


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
		//TrailComponent->Activate();
		TrailComponent->SetVisibility(true);
		TrailComponent->SetHiddenInGame(false);
		//TrailComponent->ActivateSystem();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
	}

	
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
	Acceleration = Direction * Force;

	AccelerationMagnitude = Force;
	OrbitalDistance = (Difference/* * ASim::DISTANCE_MULTIPLIER*/).Length(); // Get distance between Bodies
}

void AAstroBody::UpdateVelocity(const double DeltaTime)
{
	OrbitalVelocity += Acceleration * (DeltaTime * ASim::SECONDS_IN_DAY);
	VelocityMagnitude = OrbitalVelocity.Length() / ASim::DISTANCE_MULTIPLIER;
}

void AAstroBody::UpdatePosition(const double DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update Position based on Velocity
	// Divide by DISTANCE_MULTIPLIER so the result will be in in-editor units
	Position += OrbitalVelocity * (DeltaTime * ASim::SECONDS_IN_DAY  / ASim::DISTANCE_MULTIPLIER);
	bool check = SetActorLocation(Position);
	if (!check)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Updating Position"));
	}
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
