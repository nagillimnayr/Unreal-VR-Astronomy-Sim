// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit_Base.h"

// Sets default values
AOrbit_Base::AOrbit_Base() :
	CentralBody(nullptr),
	OrbitingBody(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOrbit_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrbit_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If either body is invalid, do nothing 
	if (!CentralBody || !OrbitingBody) {return;}
	
}

