// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainHUD.generated.h"

class UCanvasPanel;
class UW_Outliner;
class UW_OutlinerItem;
class AOrbit;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API UW_MainHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToOutliner(AOrbit* Orbit);

	
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCanvasPanel* Canvas;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UW_Outliner* Outliner;
};
