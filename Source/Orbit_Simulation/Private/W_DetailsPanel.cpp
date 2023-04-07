// Fill out your copyright notice in the Description page of Project Settings.


#include "W_DetailsPanel.h"

#include "Orbit.h"
#include "AstroBody.h"
#include "SimPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CommonTextBlock.h"
#include "CommonNumericTextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

void UW_DetailsPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	Init();
}

void UW_DetailsPanel::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UW_DetailsPanel::Init()
{
	if(!(Border &&
	BodyNameText &&
	SurfaceButton &&
	MassValue &&
	MeanRadiusValue))
	{return;}
	
	MassValue->SetNumericType(ECommonNumericType::Number);
	MassValue->FormattingSpecification.UseGrouping = true;
	MeanRadiusValue->SetNumericType(ECommonNumericType::Distance);
	MeanRadiusValue->FormattingSpecification.UseGrouping = true;

	SpeedValue->SetNumericType(ECommonNumericType::Number);
	SpeedValue->FormattingSpecification.UseGrouping = true;

	// Setup Button
	SurfaceButton->OnClicked.AddUniqueDynamic(this, &UW_DetailsPanel::OnSurfaceButtonClick);

	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	// Get Body from selection
	if(!SimPlayerController->GetSelected())
	{
		SetUnselected();
		return;
	}
	AAstroBody* Body = Cast<AAstroBody>(SimPlayerController->GetSelected());
	// If a body isn't selected
	if(!IsValid(Body))
	{
		SetUnselected();
		return;
	}

	SetSelected(Body);
}

void UW_DetailsPanel::SetUnselected()
{
	// Set Name to null
	BodyNameText->SetText(FText::FromString(FString("")));
	// Collapse
	SetVisibility(ESlateVisibility::Collapsed);
}

void UW_DetailsPanel::SetSelected(AAstroBody* Body)
{
	SetVisibility(ESlateVisibility::Visible);
	
	// Set Name
	BodyNameText->SetText(FText::FromString(Body->GetActorLabel()));
	// Set Mass
	MassValue->SetCurrentValue(Body->GetMassOfBody());
	// Set Mean Radius
	MeanRadiusValue->SetCurrentValue(Body->GetMeanRadius());
	
	AOrbit* Orbit = Body->GetOrbit();
	
	if(!IsValid(Orbit))
	{
		OrbitDetailsBox->SetVisibility(ESlateVisibility::Collapsed); // Collapse sub-panel
	}
	else
	{
		OrbitDetailsBox->SetVisibility(ESlateVisibility::Visible); 
		SpeedValue->SetCurrentValue(Orbit->GetOrbitalSpeed());
		OrbitalDistanceValue->SetCurrentValue(Orbit->GetOrbitalDistance());
	}
	
}

void UW_DetailsPanel::OnSurfaceButtonClick()
{
	// Get Player Controller
	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	SimPlayerController->GoToSurface();
}
