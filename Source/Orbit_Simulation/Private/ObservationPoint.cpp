// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservationPoint.h"

#include "GameFramework/SpringArmComponent.h"

// Sets default values
AObservationPoint::AObservationPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Surface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Surface"));
	if(IsValid(SurfaceMesh))
	{
		Surface->SetStaticMesh(SurfaceMesh);
	}
	else
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SurfaceMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		if (SurfaceMeshAsset.Succeeded())
		{
			Surface->SetStaticMesh(SurfaceMeshAsset.Object);
		}
	}

}

// Called when the game starts or when spawned
void AObservationPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObservationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

