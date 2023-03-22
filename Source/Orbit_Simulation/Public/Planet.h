// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AstroBody.h"
#include "Planet.generated.h"

class USpringArmComponent;
class USpotLightComponent;
class AOrbit;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API APlanet : public AAstroBody
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	APlanet();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

protected:
	
};
