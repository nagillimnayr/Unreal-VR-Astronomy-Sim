// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "SimInputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API USimInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	// SpaceView
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMouseClick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputYaw;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputPitch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputZoom;

	// SurfaceView
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputFreeLook;
};
