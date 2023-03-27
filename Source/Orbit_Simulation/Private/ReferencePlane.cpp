// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferencePlane.h"

#include "OrbitalPlaneComponent.h"

// Sets default values
AReferencePlane::AReferencePlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}

void AReferencePlane::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

// Called when the game starts or when spawned
void AReferencePlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReferencePlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

