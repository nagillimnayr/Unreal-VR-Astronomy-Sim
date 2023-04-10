// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_DetailsPanel.generated.h"

DECLARE_DELEGATE(FOnSurfaceButtonClickedDelegate)

class UVerticalBox;
class AAstroBody;
class UTextBlock;
class UCommonTextBlock;
class UCommonNumericTextBlock;
class UBorder;
class UButton;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API UW_DetailsPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void Init();

	void SetUnselected();
	void SetSelected(AAstroBody* Body);

	UFUNCTION(BlueprintCallable)
	void OnSurfaceButtonClick();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UBorder* Border;

	// Body data
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UCommonTextBlock* BodyNameText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UCommonNumericTextBlock* MassValue;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UCommonNumericTextBlock* MeanRadiusValue;

	//Orbit data
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UVerticalBox* OrbitDetailsBox;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UCommonNumericTextBlock* SpeedValue;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UCommonNumericTextBlock* OrbitalDistanceValue;

	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UButton* SurfaceButton;
};
