// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ORBIT_SIMULATION_API UW_UserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
};
