// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MainHUD.h"

#include "W_Outliner.h"
#include "W_OutlinerItem.h"
#include "Components/CanvasPanel.h"
#include "Orbit.h"

void UW_MainHUD::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UW_MainHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UW_MainHUD::AddItemToOutliner(AOrbit* Orbit)
{
	check(IsValid(Outliner));
	check(Orbit);

	// Pass the orbit actor on to the outliner to construct the widget
	Outliner->AddItem(Orbit);
}
