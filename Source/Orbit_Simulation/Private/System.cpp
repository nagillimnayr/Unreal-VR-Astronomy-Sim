// Fill out your copyright notice in the Description page of Project Settings.


#include "System.h"

#include "Kismet/GameplayStatics.h"
#include "AstroBody.h"
#include "Orbit.h"
//#include "RelativeMotionMap.h"
//#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "SimPlayerController.h"
#include "SimulationSubsystem.h"
#include "W_MainHUD.h"
#include "W_OutlinerItem.h"
#include "Camera/CameraComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "Orbit_Simulation/SimGameMode.h"


// Sets default values
ASystem::ASystem() :
SystemRadius(10000000000.0) // 10 billion meters
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	
	// Initialize Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera->SetupAttachment(CameraBoom);
	CameraBoom->SetupAttachment(SceneRoot);

	// Celestial Sphere
	HeliocentricCelestialSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CelestialSphere"));
	HeliocentricCelestialSphere->SetupAttachment(SceneRoot);
	
}

void ASystem::Initialize()
{
	// Set scale of Celestial Sphere
	HeliocentricCelestialSphere->SetWorldScale3D(FVector(SystemRadius / 100.0));
	
	if(!IsValid(PrimaryBody)) return;
	
	FindOrbits();

	// Attach Primary Body to the System Actor
	PrimaryBody->AttachToActor(this, 
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));
	

	// Camera
	CameraBoom->TargetArmLength = 100.0 * PrimaryBody->GetMeanRadius();
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 0.0)));
}

// Called when the game starts or when spawned
void ASystem::BeginPlay()
{
	Super::BeginPlay();

	USimulationSubsystem* Simulation = GetWorld()->GetSubsystem<USimulationSubsystem>();
	if(Simulation)
	{
		Simulation->AddSystem(this);
	}
	FindOrbits();

	// Add reference to self to Player Controller
	if(ASimPlayerController* SimController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SimController->SetSystem(this);
	}

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	ASimGameMode* SimGameMode = Cast<ASimGameMode>(GameMode);

	check(IsValid(SimGameMode));

	UW_MainHUD* HUD = SimGameMode->GetMainHUD();
	
	// Create Outliner Entry Widgets for each Orbit
	for(AOrbit* Orbit : Orbits)
	{
		if(!IsValid(Orbit))
		{
			Orbits.Remove(Orbit);
			continue;
		}

		AAstroBody* Body = Orbit->GetOrbitingBody();
		check(Body);
		Body->CreateIcon();
		
		// The HUD will pass orbit on to the outliner, which will construct the widget
		HUD->AddItemToOutliner(Orbit);
	}
	
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

void ASystem::UpdateOrbits(const double DeltaTime)
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

}
#endif


void ASystem::ReinitializeAll()
{
	// Reinitialize all Orbits
	for(AOrbit* Orbit : Orbits)
	{
		Orbit->Initialize();
	}
}
