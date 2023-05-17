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
#include "CommonVisibilitySwitcher.h"
#include "CommonBorder.h"
#include "../CalculateOrbitalElements/AstroConstants.h"

void UW_DetailsPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UE_LOG(LogTemp, Warning, TEXT("> Details Panel: SynchronizeProperties()"))
	//Init();
}

void UW_DetailsPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("> Details Panel: NativeConstruct()"))
	Init();
}

/*void UW_DetailsPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}*/

void UW_DetailsPanel::UpdateSelected()
{
	// Get controller
	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	// Get Selected Actor from Player Controller
	SetSelected(SimPlayerController->GetSelected());
}

void UW_DetailsPanel::Init()
{
	//SetVisibility(ESlateVisibility::Collapsed);
	if(!(Border &&
	NameText &&
	SurfaceButton &&
	SpaceButton &&
	MassValue &&
	MeanRadiusValue &&
	OrbitDetailsBox &&
	OrbitalDistanceValue &&
	SpeedValue
	))
	{return;}
	
	UE_LOG(LogTemp, Warning, TEXT("> Details Panel: Init()"))

	// Set Numeric types
	MassValue->SetNumericType(ECommonNumericType::Number);
	MassValue->FormattingSpecification.UseGrouping = true;
	MeanRadiusValue->SetNumericType(ECommonNumericType::Number);
	MeanRadiusValue->FormattingSpecification.UseGrouping = true;
	//MeanRadiusValue->SetText(FText::FromString(TEXT("*10^7")));

	SpeedValue->SetNumericType(ECommonNumericType::Number);
	SpeedValue->FormattingSpecification.UseGrouping = true;

	// Setup Buttons
	SurfaceButton->OnClicked.AddUniqueDynamic(this, &UW_DetailsPanel::OnSurfaceButtonClicked);
	SpaceButton->OnClicked.AddUniqueDynamic(this,&UW_DetailsPanel::OnSpaceButtonClicked);
	
	// Get controller
	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	// Subscribe to delegates
	SimPlayerController->OnSelectDelegate.AddUniqueDynamic(this, &UW_DetailsPanel::SetSelected);
	SimPlayerController->OnUnSelectDelegate.AddUniqueDynamic(this, &UW_DetailsPanel::SetUnselected);
	
	UpdateSelected();
}

void UW_DetailsPanel::SetUnselected()
{
	//UE_LOG(LogTemp, Warning, TEXT("> Details Panel: SetUnselected()"))
	
	// No object is selected, so hide the details panel
	
	// Set Name to null
	NameText->SetText(FText::FromString(FString("")));
	// Collapse widget
	SetVisibility(ESlateVisibility::Collapsed);
}

void UW_DetailsPanel::SetSelected(AActor* SelectedActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("> Details Panel: SetSelected()"))
	
	if(!IsValid(SelectedActor))
	{
		SetUnselected();
		return;
	}
	// Attempt to cast AActor to AAstroBody
	AAstroBody* Body = Cast<AAstroBody>(SelectedActor);
	if(!Body)
	{
		// If cast failed, SelectedActor is not an AAstroBody
		SetUnselected();
		return;
	}
	
	// If cast succeeded, SelectedActor is an AAstroBody and we can continue
	SetVisibility(ESlateVisibility::Visible);
	
	// Set Name
	NameText->SetText(FText::FromString(Body->GetBodyName()));
	// Set Mass
	MassValue->SetCurrentValue(Body->GetMassOfBody()/* * Unit::SOLAR_MASS*/);
	// Set Mean Radius
	MeanRadiusValue->SetCurrentValue(Body->GetMeanRadius() /** Unit::EARTH_RADIUS * Unit::M_TO_KM*/);

	// Get the Body;s Orbit
	AOrbit* Orbit = Body->GetOrbit();
	// If the Body doesn't have an Orbit, collapse the Orbit details sub-panel
	if(!IsValid(Orbit))
	{
		OrbitDetailsBox->SetVisibility(ESlateVisibility::Collapsed); // Collapse sub-panel
	}
	else // Otherwise, update the Orbit Details 
	{
		OrbitDetailsBox->SetVisibility(ESlateVisibility::Visible); 
		//SpeedValue->SetCurrentValue(Orbit->GetOrbitalSpeed());
		SpeedValue->SetCurrentValue(Orbit->GetOrbitalSpeed() * Unit::M_TO_KM);
		OrbitalDistanceValue->SetCurrentValue(Orbit->GetOrbitalDistance() / 10000.0);
	}
	
}

void UW_DetailsPanel::OnSurfaceButtonClicked()
{
	// Get Player Controller
	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	SimPlayerController->TransitionToSurface();

	// Switch Buttons
	ButtonSwitcher->SetActiveWidgetIndex(1);
}

void UW_DetailsPanel::OnSpaceButtonClicked()
{
	// Get Player Controller
	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),  0));
	check(SimPlayerController);

	SimPlayerController->TransitionToSpace();
	
	// Switch Buttons
	ButtonSwitcher->SetActiveWidgetIndex(0);
}
