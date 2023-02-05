// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroBody.h"
#include "Sim.generated.h"

UCLASS()
class ORBIT_SIMULATOR_API ASim : public AActor
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

protected:
	// Constants
	static const double GRAVITATIONAL_CONSTANT;
	static const double ASTRONOMICAL_UNIT;
	static const double SOLAR_MASS;

	static const double SECONDS_IN_DAY;
	static const double DISTANCE_MULTIPLIER;
	static const double KM_TO_M;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AAstroBody*> Bodies; // Array of all the bodies in the simulation

};
