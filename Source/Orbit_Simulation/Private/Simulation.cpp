// Fill out your copyright notice in the Description page of Project Settings.


#include "Simulation.h"

#include "SimPlayerController.h"
#include "System.h"
#include "Kismet/GameplayStatics.h"
#include "Orbit_Simulation/CalculateOrbitalElements/AstroConstants.h"


const int ASimulation::STEPS_PER_SECOND = 60;
const double ASimulation::FIXED_TIMESTEP = 1.0 / ASimulation::STEPS_PER_SECOND;

// Sets default values
ASimulation::ASimulation() :
TimeScale(10)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

void ASimulation::Initialize()
{
	
	SetActorLocation(FVector::ZeroVector);
	
	FindSystems();
	Timer = 0.0; // Reset Timer

	for(ASystem* System : Systems)
	{
		System->AttachToActor(this, 
			FAttachmentTransformRules(
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				false));
	}
	
}

// Called when the game starts or when spawned
void ASimulation::BeginPlay()
{
	Super::BeginPlay();
	
	/*if(ASimPlayerController* SimController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SimController->SetSimulation(this);
	}*/
	Initialize();
}

void ASimulation::FindSystems()
{
	//Systems.Empty(); // Clear array
	
	TArray<AActor*> Actors;
	// Find all ASystem actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASystem::StaticClass(),  Actors);
	
	// Copy references into Systems array
	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;
		// Cast to ASystem
		ASystem* System = Cast<ASystem, AActor>(Actor);
		if(!System) continue;

		// Don't add element if already in array
		if (Systems.Contains(System)) continue;
		
		Systems.Add(System); // Add to array
	}
}

void ASimulation::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Initialize();
}


// Called every frame
void ASimulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Timer += FIXED_TIMESTEP * TimeScale;
	//Timer += DeltaTime * TimeScale; // Update timer
	Timer += DeltaTime; // Update timer

	Days += DeltaTime*  TimeScale;
	/*if(Days >= 365.25)
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
	}*/
	while(Days >= 365.25)
	{
		Days -= 365.25;
		Years++;
	}
	
	// Because increasing the Time Dilation just multiplies DeltaTime, which increase the Time Step and
	// results in a less accurate simulation, we can instead call the update function multiple times per tick
	// to emulate Unity's FixedUpdate();

	const double NumOfStepsReal = (DeltaTime * STEPS_PER_SECOND * TimeScale) + Remainder; // The number of steps we need to perform as a real number
	const int NumOfSteps = static_cast<int>(NumOfStepsReal); // Truncate to integer so we can use it to control the loop
	Remainder = NumOfStepsReal - NumOfSteps; // Isolate the fractional part by subtracting the truncated integer from the original real number
	// Remainder will be added on to NumOfStepsReal next tick.
	
	for (int i = 0; i < NumOfSteps; i++)
	{
		// Iterate through every orbit and update each
		for (ASystem* System : Systems)
		{
			if(!IsValid(System))
			{
				Systems.Remove(System);
				continue;
			}
			System->UpdateOrbits(FIXED_TIMESTEP);
		}
	}

}

