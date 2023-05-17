// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_DetailsPanel.generated.h"

//DECLARE_DELEGATE(FOnSurfaceButtonClickedDelegate);

class UVerticalBox;
class AAstroBody;
class UCommonTextBlock;
class UCommonNumericTextBlock;
class UCommonBorder;
class UButton;
class UCommonVisibilitySwitcher;
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

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Init();

	
protected:
	UFUNCTION(BlueprintCallable)
	void UpdateSelected();
	UFUNCTION(BlueprintCallable)
	void SetUnselected();
	UFUNCTION(BlueprintCallable)
	void SetSelected(AActor* SelectedActor);

	UFUNCTION(BlueprintCallable)
	void OnSurfaceButtonClicked();
	UFUNCTION(BlueprintCallable)
	void OnSpaceButtonClicked();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonBorder* Border;

	// Body data
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* NameText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* MassValue;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* MeanRadiusValue;

	//Orbit data
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* OrbitDetailsBox;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* SpeedValue;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonNumericTextBlock* OrbitalDistanceValue;

	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonVisibilitySwitcher* ButtonSwitcher; // For Switching between the two buttons when one of them is pressed
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* SurfaceButton; // Button to switch to surface view
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* SpaceButton; // Button to switch to space view
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* TraceButton; // Button to enable Tracing apparent motion of another body

	
};
