// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroConstants.h"
#include "Sim.generated.h"

class AAstroBody;
class AOrbit;
class ARetrogradePath;

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
	virtual void OnConstruction(const FTransform& Transform) override;

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

	void AddRetrogradePath(ARetrogradePath* Path) { RetrogradePaths.Push(Path); }
	void RemoveRetrogradePath(ARetrogradePath* Path) { RetrogradePaths.Remove(Path); }
	
protected:
	// References to other Actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AAstroBody*> Bodies; // Array of all the bodies in the simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AOrbit*> Orbits; // Array of all the Orbits in the simulation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<ARetrogradePath*> RetrogradePaths; // Array of all the Orbits in the simulation
	

public:
	// Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeScale;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Timer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Days = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Years = 0;
	
	double Remainder;
};


