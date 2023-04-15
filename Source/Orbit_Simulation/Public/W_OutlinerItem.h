// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "W_OutlinerItem.generated.h"


class AOrbit;
class AAstroBody;
class UButton;
class UTextBlock;
class UImage;

/**
 * 
 */

UCLASS()
class ORBIT_SIMULATION_API UW_OutlinerItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void Init();
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SelectEntry();
	
	// Setters
	void SetOrbit(AOrbit* NewOrbit) {Orbit = NewOrbit;}
	void SetBody(AAstroBody* NewBody) {Body = NewBody;}

	// Getters
	AOrbit* GetOrbit() const {return Orbit;}
	AAstroBody* GetBody() const {return Body;}
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* ClickableContainer; // When clicked on, will select the AAstroBody associated with this widget
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* IconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AOrbit> Orbit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> Body;
	
};
