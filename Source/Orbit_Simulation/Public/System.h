// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System.generated.h"

class AAstroBody;
class AOrbit;
class ACelestialSphere;
class ARelativeMotionMap;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable, BlueprintType, meta=(ShortToolTip="This class represents a gravitationally bound system (i.e. a planetary system, stellar system, etc)"))
class ORBIT_SIMULATION_API ASystem : public APawn
{
	// This class represents a gravitationally bound system
	// (i.e. a planetary system, stellar system, etc) 
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASystem();

protected:
	virtual void Initialize(); 
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	void FindOrbits();

public:
	UFUNCTION(BlueprintCallable, Category="Orbit")
	virtual void UpdateOrbits(double DeltaTime);

	void AddOrbit(AOrbit* Orbit);

	UFUNCTION(BlueprintCallable, Category="Orbit")
	TArray<AAstroBody*> GetAstroBodies();
	
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
UFUNCTION(BlueprintCallable)
	AAstroBody* GetPrimaryBody() {return PrimaryBody;}
UFUNCTION(BlueprintCallable)
	TArray<AOrbit*>& GetOrbits() {return Orbits;}
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ReinitializeAll();
protected:
	// References to other Actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TArray<AOrbit*> Orbits; // Array of all the Orbits in the system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<AAstroBody> PrimaryBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<UStaticMeshComponent> HeliocentricCelestialSphere;

	// Scene Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SystemRadius;
	
	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
};


