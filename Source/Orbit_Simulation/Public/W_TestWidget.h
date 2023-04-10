// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_TestWidget.generated.h"


class AAstroBody;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API UW_TestWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void Init();
	virtual void SynchronizeProperties() override;
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* TextName;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ImageIcon;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> Body;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconTexture;
};
