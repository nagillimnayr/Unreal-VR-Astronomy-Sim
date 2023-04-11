// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSubsystem.h"

#include "System.h"
#include "Kismet/GameplayStatics.h"

const int USimulationSubsystem::STEPS_PER_SECOND = 60;
const double USimulationSubsystem::FIXED_TIMESTEP = 1.0 / USimulationSubsystem::STEPS_PER_SECOND;

bool USimulationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void USimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("> Initializing Simulation Subsystem"));
	
	// Set starting timescale
	TimeScale = 5;

	// Populate Systems array
	FindSystems();
	Timer = 0.0; // Reset Timer
}

void USimulationSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	UE_LOG(LogTemp, Warning, TEXT("> De-Initializing Simulation Subsystem"));
}

void USimulationSubsystem::Tick(float DeltaTime)
{
	// If we're on the same frame, don't tick
	if(LastFrameNumberTicked == GFrameCounter) {return;}
	LastFrameNumberTicked = GFrameCounter;
	
	//Timer += FIXED_TIMESTEP * TimeScale;
	//Timer += DeltaTime * TimeScale; // Update timer
	
	Timer += DeltaTime; // Update timer in real-time
	Days += DeltaTime * TimeScale; // Update days counter
	
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

void USimulationSubsystem::IncreaseTimeScale(const int Amount)
{
	// Clamp the value to be within the range [0, 100]
	TimeScale = FMath::Clamp(TimeScale + Amount, 0, 100);
}

void USimulationSubsystem::DecreaseTimeScale(const int Amount)
{
	IncreaseTimeScale(-Amount);
}

void USimulationSubsystem::AddSystem(ASystem* NewSystem)
{
	if(!NewSystem) return;
	if(Systems.Contains(NewSystem)) return;

	Systems.Add(NewSystem);
}

void USimulationSubsystem::FindSystems()
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
