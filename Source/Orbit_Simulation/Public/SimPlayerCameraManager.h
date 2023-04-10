// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SimPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API ASimPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	
};
