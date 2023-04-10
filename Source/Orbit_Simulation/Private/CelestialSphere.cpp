// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialSphere.h"

// Sets default values
ACelestialSphere::ACelestialSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACelestialSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACelestialSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

