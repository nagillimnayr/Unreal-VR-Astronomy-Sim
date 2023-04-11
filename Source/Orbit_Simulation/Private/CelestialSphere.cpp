// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialSphere.h"

#include "Components/SphereComponent.h"
#include "SimPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACelestialSphere::ACelestialSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);
	
	// Initialize Sphere
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	Sphere->SetupAttachment(SceneRoot);
	Sphere->CastShadow = false; // Disable shadows

	if(SphereStaticMesh)
	{
		Sphere->SetStaticMesh(SphereStaticMesh);
	}
	if(BaseMaterial)
	{
		Sphere->SetMaterial(0, BaseMaterial);
	}

	// Collision Sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetSphereRadius(100.0);
	CollisionSphere->SetupAttachment(Sphere);
	
}

// Called when the game starts or when spawned
void ACelestialSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACelestialSphere::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(BaseMaterial && !MaterialInstance)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr);
		
	}

	// Subscribe to Delegate
	if(ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &ACelestialSphere::DestroySelf);
	}
	
}

// Called every frame
void ACelestialSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACelestialSphere::DestroySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("> AObservationPoint: DestroySelf()"));
	Destroy();
}

