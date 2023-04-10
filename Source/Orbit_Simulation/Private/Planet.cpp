// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "System.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();
}

void APlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APlanet::PostInitProperties()
{
	Super::PostInitProperties();
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
