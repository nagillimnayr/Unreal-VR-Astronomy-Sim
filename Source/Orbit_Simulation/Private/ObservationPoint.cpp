// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservationPoint.h"

#include "SimPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObservationPoint::AObservationPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	
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
	Surface->SetupAttachment(SceneRoot);

}

// Called when the game starts or when spawned
void AObservationPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObservationPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &AObservationPoint::DestroySelf);
	}
	
}

// Called every frame
void AObservationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObservationPoint::DestroySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("> AObservationPoint: DestroySelf()"));
	Destroy();
}

