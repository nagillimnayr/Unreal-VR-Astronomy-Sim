// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Outliner.generated.h"

struct FOutlinerEntryData;
class UW_OutlinerItem;
class UTreeView;
class AOrbit;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API UW_Outliner : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init();
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItem(AOrbit* Orbit);

	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetEntryWidgetClass() {return EntryWidgetClass;};
protected:
	UPROPERTY(EditAnywhere, Category="Outliner")
	TSubclassOf<UUserWidget> EntryWidgetClass = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category="Outliner", meta=(BindWidget))
	UTreeView* OutlinerTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Outliner")
	TArray<AOrbit*> Orbits;

	
};
