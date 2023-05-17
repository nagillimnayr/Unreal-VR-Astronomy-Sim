// Fill out your copyright notice in the Description page of Project Settings.


#include "POVMotionMap.h"

#include "AstroBody.h"
#include "SplineTraceComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "SimPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Define collision channel for Celestial Sphere
#define COLLISION_CHANNEL_CELESTIAL_SPHERE ECC_GameTraceChannel2

// Sets default values
APOVMotionMap::APOVMotionMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Scene component
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	TraceChannel = COLLISION_CHANNEL_CELESTIAL_SPHERE;
	
	/*// Initialize collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Enable Collision
	CollisionSphere->InitSphereRadius(4000.0);
	CollisionSphere->SetupAttachment(SceneRoot);*/

	// Initialize Spline Trace
	SplineTrace = CreateDefaultSubobject<USplineTraceComponent>(TEXT("SplineTrace"));
	SplineTrace->SetupAttachment(SceneRoot);
	SplineTrace->SetMeshScale(FVector2D(0.075));
	
	// Initialize Static Mesh
	ProjectedBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projected Body"));
	ProjectedBody->SetupAttachment(SceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if(MeshAsset.Succeeded() && ProjectedBody)
	{
		ProjectedBody->SetStaticMesh(MeshAsset.Object);
		
	}
	
	ProjectedBody->CastShadow = false; // Disable shadows
	
}

void APOVMotionMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetActorLocation(FVector::ZeroVector);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));

	/*SetSphereRadius();

	CollisionSphere->SetCollisionResponseToChannel(TraceChannel, ECollisionResponse::ECR_Block);*/
	
	if(ProjectedBody && BodyMaterialBase)
	{
		ProjectedBody->SetMaterial(0, BodyMaterialBase);
		ProjectedBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	/*if(SplineTrace->IsValidLowLevel())
	{
		SplineTrace->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}*/
	
	SplineTrace->SetMeshScale(FVector2D(0.075));

	ProjectedBody->CastShadow = false; // Disable shadows
	
}

// Called when the game starts or when spawned
void APOVMotionMap::BeginPlay()
{
	Super::BeginPlay();
	
	//SetSphereRadius();
	SetActorRelativeLocation(FVector::ZeroVector);

	// Subscribe to Delegate
	if(ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &APOVMotionMap::DestroySelf);
	}

	// Initialize Niagara System/ Component
	/*if (TrailSystem)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, ProjectedBody, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
			false);
		//TrailComponent->SetVisibility(true);
		//TrailComponent->SetHiddenInGame(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
	}*/

	// Subscribe to Delegates
	
	// Get Simulation Subsystem
	/*if(USimulationSubsystem* Simulation = Cast<USimulationSubsystem>(GetWorld()->GetSubsystem<USimulationSubsystem>()))
	{
		Simulation->OnUpdatePositionsDelegate.AddUniqueDynamic(this, &APOVMotionMap::TraceMotion);
	}*/

	
}

// Called every frame
void APOVMotionMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TraceMotion();
}


void APOVMotionMap::TraceMotion()
{
	if(!IsValid(ReferenceBody) ||
		!IsValid(OtherBody))
	{return;}

	FHitResult HitResult;

	// Get direction vector between the two bodies
	//FVector Direction = OtherBody->GetActorLocation() - ReferenceBody->GetActorLocation();
	//Direction.Normalize(); // Normalize the Direction vector
	
	// Get start and end points of line trace
	//const FVector TraceStart = ReferenceBody->GetActorLocation();
	//const FVector TraceEnd = ReferenceBody->GetActorLocation() + (Direction * (CollisionSphere->GetScaledSphereRadius() * 2));
	//const FVector TraceEnd = OtherBody->GetActorLocation();
	// Since both bodies are within the sphere, the distance between either of them and the surface of the sphere in any
	// direction cannot be greater than the diameter of the sphere

	
	// Get start and end points of line trace
	const FVector TraceStart = OtherBody->GetActorLocation();
	const FVector TraceEnd = ReferenceBody->GetActorLocation();
	
	// Cast out line trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ReferenceBody);
	QueryParams.AddIgnoredActor(OtherBody);
	QueryParams.AddIgnoredComponent(ProjectedBody);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);
	//GetWorld()->LineTraceSingleByChannel(HitResult, TraceEnd, TraceStart, TraceChannel, QueryParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 1.0f, 0, 10.0f);

	// If line trace wasn't blocked, return
	if (!HitResult.bBlockingHit) return;

	// Otherwise, store the collision point in the mapped points array
	//MappedPoints.Add(HitResult.ImpactPoint);


	// Set position of Projected Body to impact point
	ProjectedBody->SetWorldLocation(HitResult.ImpactPoint);
	
	// Add position of impact point to the Spline Trace
	SplineTrace->Update(HitResult.ImpactPoint);
	
	// Draw debug line
	//DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Blue, false, -1, 0, 10.0f);

	// Set Trail Spawn Position
	/*if(TrailComponent)
	{
		TrailComponent->SetNiagaraVariablePosition(FString("User.Position"), HitResult.ImpactPoint);
	}*/

	
	// Display debug message
	/*AActor* HitActor = HitResult.GetActor();
	auto HitComponent = HitResult.Component;
	if(!HitActor) 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HitActor: None"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HitActor: " + HitActor->GetName()));

	if(!HitComponent->IsValidLowLevel())
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HitComponent: None"));
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HitComponent: " + HitComponent->GetName()));
	}*/
}

void APOVMotionMap::SetReferenceBody(AAstroBody* Body)
{
	ReferenceBody = Body;
	
	if(ReferenceBody)
	{
		AttachToActor(ReferenceBody, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		));
	}
}

void APOVMotionMap::SetOtherBody(AAstroBody* Body)
{
	OtherBody = Body;

	CreateMaterialInstance();
}

void APOVMotionMap::DestroySelf()
{
	Destroy();
}


void APOVMotionMap::CreateMaterialInstance()
{
	if(!OtherBody) return;
	if(!BodyMaterialBase) return;
	if(!SplineMaterialBase) return;
	
	FLinearColor Color = OtherBody->GetColor();

	if(!BodyMaterialInstance)
	{
		BodyMaterialInstance = UMaterialInstanceDynamic::Create(BodyMaterialBase, nullptr);
	}
	if(!SplineMaterialInstance)
	{
		SplineMaterialInstance = UMaterialInstanceDynamic::Create(SplineMaterialBase, nullptr);
	}
	
	BodyMaterialInstance->SetVectorParameterValue(TEXT("Color"), Color);
	SplineMaterialInstance->SetVectorParameterValue(TEXT("Color"), Color);

	ProjectedBody->SetMaterial(0, BodyMaterialInstance);
	SplineTrace->SetMeshMaterial(SplineMaterialInstance);
	
	if(!TrailComponent) return;
	TrailComponent->SetNiagaraVariableLinearColor(FString("User.DynamicColor"), Color);
}

