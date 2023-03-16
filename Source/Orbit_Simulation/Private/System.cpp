// Fill out your copyright notice in the Description page of Project Settings.


#include "System.h"

#include "Kismet/GameplayStatics.h"
#include "AstroBody.h"
#include "Orbit.h"
#include "RetrogradePath.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"

constexpr  int STEPS_PER_SECOND = 60;
constexpr double ASystem::FIXED_TIMESTEP = 1.0 / STEPS_PER_SECOND; // 1/60th of a second

// Sets default values
ASystem::ASystem() :
TimeScale(1),
Timer(0.0),
Remainder(0.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ASystem::BeginPlay()
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

void ASystem::OnConstruction(const FTransform& Transform)
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
void ASystem::Tick(float DeltaTime)
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
void ASystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Set TimeScale
	//GetWorldSettings()->SetTimeDilation(TimeScale);
}
#endif
