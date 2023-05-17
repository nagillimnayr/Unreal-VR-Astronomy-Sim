// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_TimePanel.generated.h"


class USimulationSubsystem;
class UCommonTextBlock;
class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API UW_TimePanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void UpdateTime();
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* TimeScaleNum;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* SecondsCounter;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* DaysCounter;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* YearsCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USimulationSubsystem> Simulation;
};
