// Fill out your copyright notice in the Description page of Project Settings.


#include "W_TimePanel.h"
#include "CommonNumericTextBlock.h"
#include "SimulationSubsystem.h"


void UW_TimePanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UW_TimePanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Get reference to Subsystem
	Simulation = GetWorld()->GetSubsystem<USimulationSubsystem>();
}

void UW_TimePanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTime();
	
}

void UW_TimePanel::Init()
{
	
}

void UW_TimePanel::UpdateTime()
{
	if(!Simulation) {return;}
	
	//UE_LOG(LogTemp, Warning, TEXT("Time Panel: Updating Timers"));
	
	// Update Timers
	TimeScaleNum->SetCurrentValue(Simulation->GetTimeScale());
	SecondsCounter->SetCurrentValue(Simulation->GetTimer());
	DaysCounter->SetCurrentValue(Simulation->GetDays());
	YearsCounter->SetCurrentValue(Simulation->GetYears());
}
