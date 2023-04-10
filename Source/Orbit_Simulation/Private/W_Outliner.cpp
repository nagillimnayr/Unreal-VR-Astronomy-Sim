// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Outliner.h"

#include "AstroBody.h"
#include "Components/TreeView.h"
#include "Orbit.h"
#include "W_OutlinerItem.h"

void UW_Outliner::Init()
{
	check(OutlinerTree);
	
	EntryWidgetClass = OutlinerTree->GetEntryWidgetClass();

	// Create list of entries
	//OutlinerTree->SetListItems(Orbits);
}

void UW_Outliner::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	Init();
}

void UW_Outliner::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UW_Outliner::AddItem(AOrbit* Orbit)
{
	check(OutlinerTree);
	check(IsValid(Orbit));
	
	
	/*AAstroBody* Body = Orbit->GetOrbitingBody();
	check(IsValid(Body));

	// Create Entry Widget
	//FString WidgetName = "Outliner Entry: " + Body->GetActorNameOrLabel();
	//UUserWidget* EntryWidget = CreateWidget(this, OutlinerTree->GetEntryWidgetClass(), FName(WidgetName));
	//EntryWidget->SetDisplayLabel(WidgetName);

	FOutlinerEntryData EntryData;
	EntryData.Label = "Outliner Entry: " + Body->GetActorNameOrLabel();
	*/
	
	
	
	// Attempt to cast to OutlinerItem
	/*UW_OutlinerItem* OutlinerEntry = Cast<UW_OutlinerItem>(EntryWidget);
	if(OutlinerEntry)
	{
		// Set Orbit
		OutlinerEntry->SetOrbit(Orbit);
		// Additional list of references to the widgets
		OutlinerData.Add(OutlinerEntry);
		UE_LOG(LogTemp, Display, TEXT("> Succesfully casted to UW_OutlinerItem"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("> Failed to cast to UW_OutlinerItem"));
	}*/

	// Add widget to outliner
	//OutlinerTree->AddItem(EntryWidget);

	if(!Orbits.Contains(Orbit))
	{
		Orbits.Add(Orbit);
		OutlinerTree->AddItem(Orbit);
	}
}
