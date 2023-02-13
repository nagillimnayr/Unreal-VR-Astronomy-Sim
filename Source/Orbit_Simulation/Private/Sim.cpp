// Fill out your copyright notice in the Description page of Project Settings.


#include "Sim.h"

#include "Kismet/GameplayStatics.h"
//#include "AstroBody.h"
#include "Orbit.h"

// Initialize static constants
const double ASim::GRAVITATIONAL_CONSTANT = 6.674e-11; // m^3/kg/s^2
//const double ASim::ASTRONOMICAL_UNIT = 1.495978707e11; // AU in m
const double ASim::SOLAR_MASS = 1.989e30; // Mass of the sun in kg

// Unit Conversion Multipliers
const double ASim::SECONDS_IN_DAY = 86400.0; // 1 second in simulation = 1 day
const double ASim::DISTANCE_MULTIPLIER = 1e11 / 1000.0; // 1 in-editor unit = 1x10^8m, 1000 in-editor units = 1x10^11m
const double ASim::KM_TO_M = 1000.0;

const int STEPS_PER_SECOND = 60;
const double ASim::FIXED_TIMESTEP = 1.0 / STEPS_PER_SECOND; // 1/60th of a second

// Sets default values
ASim::ASim() :
Timer(0.0),
TimeScale(1),
Remainder(0.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbit::StaticClass(),  Actors);

	for (auto Actor : Actors) // Copy references into Orbits array
	{
		Orbits.Add(Cast<AOrbit>(Actor)); // Cast to AOrbit*
	}*/

	
}

// Called when the game starts or when spawned
void ASim::BeginPlay()
{
	Super::BeginPlay();
	// GetWorldSettings()->SetTimeDilation(TimeScale);
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbit::StaticClass(),  Actors);

	// Copy references into Orbits array
	for (auto Actor : Actors)
	{
		// Cast to AOrbit*
		Orbits.Add(Cast<AOrbit>(Actor));
	}
}

// Called every frame
void ASim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Timer += FIXED_TIMESTEP * TimeScale;
	Timer += DeltaTime * TimeScale; // Update timer

	Days += DeltaTime * TimeScale;
	/*if(Timer >= 365.25)
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
	}*/
	if(Days >= 365.25)
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
		Days -= 365.25;
		Years++;
	}
	
	// Because increasing the Time Dilation just multiplies DeltaTime, which increase the Time Step and
	// results in a less accurate simulation, we can instead call the update function multiple times per tick
	// to emulate Unity's FixedUpdate();

	double Delta = (DeltaTime * STEPS_PER_SECOND * TimeScale) + Remainder; // The number of steps we need to perform as a real number
	int NumOfSteps = static_cast<int>(Delta); // Truncate to integer so we can use it to control the loop
	Remainder = Delta - NumOfSteps; // Isolate the fractional part by subtracting the truncated integer from the original real number
	// Remainder will be added on to Delta next tick.
	
	for (int i = 0; i < NumOfSteps; i++)
	{
		// Iterate through every orbit and update each
		for (auto orbit : Orbits)
		{
			orbit->Update(FIXED_TIMESTEP);
		}
	}
}



#if WITH_EDITOR
void ASim::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Set TimeScale
	//GetWorldSettings()->SetTimeDilation(TimeScale);
}
#endif