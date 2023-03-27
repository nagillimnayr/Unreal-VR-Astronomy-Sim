// Fill out your copyright notice in the Description page of Project Settings.


#include "System.h"

#include "Kismet/GameplayStatics.h"
#include "AstroBody.h"
#include "Orbit.h"
//#include "RelativeMotionMap.h"
//#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "Camera/CameraComponent.h"


// Sets default values
ASystem::ASystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	
	// Initialize Camera
	/*Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera->SetupAttachment(CameraBoom);
	CameraBoom->SetupAttachment(SceneRoot);*/
	
}

void ASystem::Initialize()
{
	
	if(!IsValid(PrimaryBody)) return;
	
	FindOrbits();

	// Attach Primary Body to the System Actor
	PrimaryBody->AttachToActor(this, 
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));
	
	// Attach Camera to Primary Body
	/*CameraBoom->AttachToComponent(PrimaryBody->GetRootComponent(), 
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));*/
	/*CameraBoom->TargetArmLength = 400.0 * PrimaryBody->GetSize();
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 0.0)));*/
}

// Called when the game starts or when spawned
void ASystem::BeginPlay()
{
	Super::BeginPlay();
	FindOrbits();
}

void ASystem::FindOrbits()
{
	TArray<AActor*> FoundActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbit::StaticClass(),  FoundActors);
	
	//Orbits.Empty(); // Clear Array

	for(AActor* Actor : FoundActors)
	{
		if(!IsValid(Actor)) continue;

		// Attempt to cast the AActor to an AOrbit
		AOrbit* Orbit = Cast<AOrbit>(Actor);
		if(!IsValid(Orbit)) continue;
		if(Orbits.Contains(Orbit)) continue; // If Orbit is already in the array, skip
		Orbits.Push(Orbit); // Add Orbit actor to the array
	}

	// Sort based on distance of Periapsis
	Orbits.Sort([](const AOrbit& Orbit1, const AOrbit& Orbit2)
	{
		return Orbit1 < Orbit2;
	});
}

void ASystem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Initialize();
}

void ASystem::UpdateOrbits(double DeltaTime)
{
	// Update each AOrbit actor in the System
	for(AOrbit* Orbit : Orbits)
	{
		if(!IsValid(Orbit))
		{
			Orbits.Remove(Orbit);
			continue;
		}
		
		Orbit->UpdateOrbitingBody(DeltaTime);
	}
}

void ASystem::AddOrbit(AOrbit* Orbit)
{
	if(!IsValid(Orbit)) return;
	if(Orbits.Contains(Orbit)) return; // If Orbit is already in the array, skip
	
	Orbits.Push(Orbit); // Add Orbit actor to the array
	
	// Sort Orbits based on distance of Periapsis
	Orbits.Sort([](const AOrbit& Orbit1, const AOrbit& Orbit2)
	{
		return Orbit1 < Orbit2;
	});
}


#if WITH_EDITOR
void ASystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Set TimeScale
	//GetWorldSettings()->SetTimeDilation(TimeScale);
}
#endif
