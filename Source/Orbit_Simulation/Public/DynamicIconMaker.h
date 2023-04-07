// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "DynamicIconMaker.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ORBIT_SIMULATION_API UDynamicIconMaker : public USceneCaptureComponent2D
{
	GENERATED_BODY()
protected:
	virtual void InitializeComponent() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Icon")
	UTextureRenderTarget2D* GetIconRenderTarget();
protected:
	UFUNCTION(BlueprintCallable, Category = "Icon")
	void CreateIcon();
public:
	
protected:
	// Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon")
	int32 Resolution;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Icon")
	TArray<FColor> PixelData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Icon")
	TObjectPtr<UTextureRenderTarget2D> IconRenderTarget;
};
