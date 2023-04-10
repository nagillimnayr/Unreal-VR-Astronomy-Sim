// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameFramework/Actor.h"
#include "Orbit_Simulation/CalculateOrbitalElements/OrbitalElements.h"
#include "Simulation.generated.h"

class ASystem;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API ASimulation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimulation();

protected:
	virtual void Initialize(); 
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	void FindSystems();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	double GetTimeScale() { return TimeScale; }
	UFUNCTION(BlueprintCallable)
	double GetTimer() { return Timer; }
	UFUNCTION(BlueprintCallable)
	double GetDays() { return Days; }
	UFUNCTION(BlueprintCallable)
	double GetYears() { return Years; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASystem*> Systems; //Array of references to all of the systems in the simulation
	
	// Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeScale;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Timer = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Days = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Years = 0;
	
	double Remainder;

public:
	static const int STEPS_PER_SECOND;
	static const double FIXED_TIMESTEP;

};


