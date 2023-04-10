// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "W_UserWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "W_OutlinerItem.generated.h"


class AOrbit;
class UTextBlock;
class UImage;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FOutlinerEntryData
{
	GENERATED_BODY()
	
	// String to set the name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Outliner")
	FString Label;
	// Material to create the Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Outliner")
	UMaterialInterface* IconMaterial;
	
	// Ptr to reference Orbit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Outliner")
	AOrbit* OrbitReference;
};


UCLASS()
class ORBIT_SIMULATION_API UW_OutlinerItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void Init();
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

	// Setters
	void SetOrbit(AOrbit* NewOrbit) {Orbit = NewOrbit;}

	// Getters
	AOrbit* GetOrbit() const {return Orbit;}
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* IconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AOrbit> Orbit;
	
};
