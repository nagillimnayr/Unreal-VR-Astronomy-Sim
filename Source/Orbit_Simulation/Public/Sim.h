// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sim.generated.h"

class AAstroBody;
class AOrbit;

UCLASS()
class ORBIT_SIMULATION_API ASim : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASim();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Constants
	static const double GRAVITATIONAL_CONSTANT;
	static const double ASTRONOMICAL_UNIT;
	static const double SOLAR_MASS;

	// Unit Conversion Multipliers
	static const double SECONDS_IN_DAY;
	static const double DISTANCE_MULTIPLIER;
	static const double KM_TO_M;

	// Fixed Time Step
	static const double FIXED_TIMESTEP;

	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AAstroBody*> Bodies; // Array of all the bodies in the simulation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AOrbit*> Orbits; // Array of all the Orbits in the simulation

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Timer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Days = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Years = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeScale;

	double Remainder;
};

// Constants
#define AU = 1.495978707e11; // Astronomical Unit

// Orbital Parameters of Planets in-editor units
/*
 * Mercury:
	- Distance: 698.1
 * Venus: 
 * Earth: 1495.978707
 * Mars: 2428.5
 * Jupiter: 
 * Saturn:
 * Uranus:
 * Neptune:

*/

// Semi-Major axes of planets in AU
#define MARS_SMA = 1.52371034;
#define MERCURY_SMA = 0.38709927;
#define VENUS_SMA = 0.72333566;
#define JUPITER_SMA = 5.20288700;
#define SATURN_SMA = 9.53667594;
#define URANUS_SMA = 19.18916464;
#define NEPTUNE_SMA = 30.06992276;
