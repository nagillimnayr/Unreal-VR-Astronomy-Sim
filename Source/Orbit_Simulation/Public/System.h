// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System.generated.h"

class AAstroBody;
class AOrbit;
class ARetrogradePath;

UCLASS()
class ORBIT_SIMULATION_API ASystem : public AActor
{
	// This class represents a gravitationally bound system
	// (i.e. a planetary system, stellar system, etc) 
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Fixed Time Step
	static const double FIXED_TIMESTEP;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable)
	void AddRetrogradePath(ARetrogradePath* Path) { RetrogradePaths.Push(Path); }
	UFUNCTION(BlueprintCallable)
	void RemoveRetrogradePath(ARetrogradePath* Path) { RetrogradePaths.Remove(Path); }

	// Map
	/*UFUNCTION(BlueprintCallable)
	static void AddOrbit(FString OrbitingBodyName, AOrbit* Orbit)
	{
		OrbitMap.Add(OrbitingBodyName, Orbit);
	}
	UFUNCTION(BlueprintCallable)
	static AOrbit* GetOrbit(FString OrbitingBodyName)
	{
		AOrbit** OrbitPtr = OrbitMap.Find(OrbitingBodyName);
		if(OrbitPtr)
		{
			return *OrbitPtr;
		}
		else
		{
			return nullptr;
		}
	}
	*/
	
protected:
	// References to other Actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AAstroBody*> Bodies; // Array of all the bodies in the simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AOrbit*> Orbits; // Array of all the Orbits in the simulation
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TMap<FString, AOrbit*> OrbitMap; // Map to associate OrbitingBody with its Orbit */

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


