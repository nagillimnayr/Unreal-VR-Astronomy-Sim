// Fill out your copyright notice in the Description page of Project Settings.


#include "W_OutlinerItem.h"
#include "Orbit.h"
#include "AstroBody.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Components/Button.h"


void UW_OutlinerItem::Init()
{if(NameText)
{
	NameText->SetText(FText::FromString(FString("Name")));
}
	if(IsValid(Orbit))
	{
		AAstroBody* Body = Orbit->GetOrbitingBody();
		
		if(!IsValid(Body) && NameText)
		{
			NameText->SetText(FText::FromString(Body->GetActorNameOrLabel()));
		}
		if(IsValid(Body) && NameText)
		{
			NameText->SetText(FText::FromString(Body->GetActorNameOrLabel()));
		}
		if(IsValid(Body) && IconImage)
		{
			UTextureRenderTarget2D* RenderTarget = Body->GetIconRenderTarget();
			SetDisplayLabel("Outliner Entry: " + Body->GetActorNameOrLabel()); 
			if(!IsValid(RenderTarget))
			{
				GLog->Log("> Invalid Render Target!");
			}
			else
			{
				IconImage->SetBrushFromMaterial(Body->GetIconMaterial());
				
			}
		}
	}
}

void UW_OutlinerItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	// Cast to Data struct
	//FOutlinerEntryData* EntryData = Cast<FOutlinerEntryData>(ListItemObject);
	// Cast to AOrbit
	AOrbit* OrbitRef = Cast<AOrbit>(ListItemObject);
	check(OrbitRef);
	Orbit = OrbitRef;
	Init();
}

void UW_OutlinerItem::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Init();
}

void UW_OutlinerItem::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}
