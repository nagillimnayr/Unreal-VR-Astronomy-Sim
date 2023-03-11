// Fill out your copyright notice in the Description page of Project Settings.


#include "Sim.h"

#include "Kismet/GameplayStatics.h"
#include "AstroBody.h"
#include "Orbit.h"
#include "RetrogradePath.h"

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
TimeScale(1),
Timer(0.0),
Remainder(0.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ASim::BeginPlay()
{
	Super::BeginPlay();
	// GetWorldSettings()->SetTimeDilation(TimeScale);
	
	Timer = 0.0; // Reset Timer
	
	TArray<AActor*> Actors;
	// Find all AOrbits
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbit::StaticClass(),  Actors);
	
	// Copy references into Orbits array
	for (auto Actor : Actors)
	{
		if (!Actor) { continue; }
		// Cast to AOrbit*
		AOrbit* orbit = Cast<AOrbit, AActor>(Actor);

		// Don't add element if already in array
		if (!Orbits.Contains(orbit))
		{
			Orbits.Add(orbit); // Add to array
		}
	}

	// Find all AAstroBodys
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAstroBody::StaticClass(),  Actors);
	// Copy references into Bodies array
	for (auto Actor : Actors)
	{
		if (!Actor) { continue; }
		// Cast to AAstroBody*
		AAstroBody* body = Cast<AAstroBody, AActor>(Actor);

		// Don't add element if already in array
		if (!Bodies.Contains(body))
		{
			Bodies.Add(body); // Add to array
		}
	}

	// Sort based on distance from origin
	Bodies.Sort([](const AAstroBody& Body1, const AAstroBody& Body2)
	{
		return Body1.GetActorLocation().Length() < Body2.GetActorLocation().Length();
	});
	Orbits.Sort([](const AOrbit& Orbit1, const AOrbit& Orbit2)
	{
		return Orbit1.OrbitingBody->GetActorLocation().Length() < Orbit2.OrbitingBody->GetActorLocation().Length();
	});
}

void ASim::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	/*
	TArray<AActor*> Actors;
	// Find all AOrbits
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbit::StaticClass(),  Actors);

	// Copy references into Orbits array
	for (auto Actor : Actors)
	{
		if (!Actor) { continue; }
		// Cast to AOrbit*
		AOrbit* orbit = Cast<AOrbit, AActor>(Actor);

		// Don't add element if already in array
		if (!Orbits.Contains(orbit))
		{
			Orbits.Add(orbit); // Add to array
		}
	}

	// Find all AAstroBodys
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAstroBody::StaticClass(),  Actors);
	// Copy references into Bodies array
	for (auto Actor : Actors)
	{
		if (!Actor) { continue; }
		// Cast to AAstroBody*
		AAstroBody* body = Cast<AAstroBody, AActor>(Actor);

		// Don't add element if already in array
		if (!Bodies.Contains(body))
		{
			Bodies.Add(body); // Add to array
		}
	}*/
}

// Called every frame
void ASim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Timer += FIXED_TIMESTEP * TimeScale;
	//Timer += DeltaTime * TimeScale; // Update timer
	Timer += DeltaTime; // Update timer

	Days += DeltaTime * TimeScale;
	/*if(Timer >= 365.25)
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
	}*/
	if(Days >= 365.25)
	{
		Days -= 365.25;
		Years++;
		//UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
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
			if(!orbit) {continue;}
			orbit->UpdateOrbitingBody(FIXED_TIMESTEP);
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