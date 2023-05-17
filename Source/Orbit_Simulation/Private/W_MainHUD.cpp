// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MainHUD.h"

#include "W_Outliner.h"
#include "W_OutlinerItem.h"
#include "Components/CanvasPanel.h"
#include "Orbit.h"
#include "W_DetailsPanel.h"

void UW_MainHUD::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UE_LOG(LogTemp, Warning, TEXT("> MainHUD: SynchronizeProperties()"))
}

void UW_MainHUD::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("> MainHUD: NativeConstruct()"))
	Init(); // Initialize
}

void UW_MainHUD::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("> MainHUD: Init()"))
	check(DetailsPanel);

}

void UW_MainHUD::AddItemToOutliner(AOrbit* Orbit)
{
	check(IsValid(Outliner));
	check(Orbit);

	// Pass the orbit actor on to the outliner to construct the widget
	Outliner->AddItem(Orbit);
}
