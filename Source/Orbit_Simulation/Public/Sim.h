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
	
	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void CalculateAcceleration(float DeltaTime);

	

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

	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AAstroBody*> Bodies; // Array of all the bodies in the simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AOrbit_Base*> Orbits; // Array of all the Orbits in the simulation
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeScale = 1;
};
