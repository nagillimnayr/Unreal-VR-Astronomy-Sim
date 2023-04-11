// Fill out your copyright notice in the Description page of Project Settings.


#include "POVMotionMap.h"

#include "AstroBody.h"
#include "SplineTrace.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"


// Define collision channel for selection
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
	
	// Initialize collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Enable Collision
	CollisionSphere->InitSphereRadius(4000.0);
	CollisionSphere->SetupAttachment(SceneRoot);

	// Initialize Spline Trace
	//SplineTrace = CreateDefaultSubobject<ASplineTrace>(TEXT("SplineTrace"));
	
	// Initialize Static Mesh
	ProjectedBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projected Body"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if(MeshAsset.Succeeded() && ProjectedBody->IsValidLowLevel())
	{
		ProjectedBody->SetStaticMesh(MeshAsset.Object);
		ProjectedBody->CastShadow = false; // Disable shadows
	}

	
}

void APOVMotionMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetActorLocation(FVector::ZeroVector);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));

	SetSphereRadius();

	CollisionSphere->SetCollisionResponseToChannel(TraceChannel, ECollisionResponse::ECR_Block);
	
	if(ProjectedBody && DefaultMaterial)
	{
		ProjectedBody->SetMaterial(0, DefaultMaterial);
		ProjectedBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	/*if(SplineTrace->IsValidLowLevel())
	{
		SplineTrace->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}*/
	
}

// Called when the game starts or when spawned
void APOVMotionMap::BeginPlay()
{
	Super::BeginPlay();
	
	SetSphereRadius();

	// Initialize Niagara System/ Component
	if (TrailSystem)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, ProjectedBody, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
			false);
		/*TrailComponent->SetVisibility(true);
		TrailComponent->SetHiddenInGame(false);*/
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
	}

}

// Called every frame
void APOVMotionMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsValid(ReferenceBody) ||
		!IsValid(OtherBody) ||
		!IsValid(CollisionSphere) /* ||
		!SplineTrace->IsValidLowLevel()*/)
	{return;}
	
	TraceMotion();
}

void APOVMotionMap::SetSphereRadius()
{
	if(!IsValid(ReferenceBody) ||
		!IsValid(OtherBody) ||
		!IsValid(CollisionSphere) /* ||
		!SplineTrace->IsValidLowLevel()*/)
	{return;}

	const double Dist1 = FVector::Distance(ReferenceBody->GetActorLocation(), GetActorLocation());
	const double Dist2 = FVector::Distance(OtherBody->GetActorLocation(), GetActorLocation());

	// Set the sphere's radius to be twice the sum of the distances between each body and the center
	CollisionSphere->SetSphereRadius((Dist1 + Dist2) * 2);
}

void APOVMotionMap::SetSphereRadius(const double Radius)
{
	if(!CollisionSphere->IsValidLowLevel()) {return;}

	CollisionSphere->SetSphereRadius(Radius);
	
}

void APOVMotionMap::TraceMotion()
{
	if(!IsValid(ReferenceBody) ||
		!IsValid(OtherBody) ||
		!IsValid(CollisionSphere) /* ||
		!SplineTrace->IsValidLowLevel()*/)
	{return;}

	FHitResult HitResult;

	// Get direction vector between the two bodies
	FVector Direction = OtherBody->GetActorLocation() - ReferenceBody->GetActorLocation();
	Direction.Normalize(); // Normalize the Direction vector
	
	// Get start and end points of line trace
	const FVector TraceStart = ReferenceBody->GetActorLocation();
	const FVector TraceEnd = ReferenceBody->GetActorLocation() + (Direction * (CollisionSphere->GetScaledSphereRadius() * 2));
	// Since both bodies are within the sphere, the distance between either of them and the surface of the sphere in any
	// direction cannot be greater than the diameter of the sphere

	// Cast out line trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ReferenceBody);
	QueryParams.AddIgnoredActor(OtherBody);
	QueryParams.AddIgnoredComponent(ProjectedBody);
	//GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceEnd, TraceStart, TraceChannel, QueryParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 1.0f, 0, 10.0f);

	// If line trace wasn't blocked, return
	if (!HitResult.bBlockingHit) return;

	// Otherwise, store the collision point in the mapped points array
	MappedPoints.Add(HitResult.ImpactPoint);

	// Add position of impact point to the Spline Trace
	//SplineTrace->Update(HitResult.ImpactPoint);

	// Set position of Projected Body to impact point
	ProjectedBody->SetWorldLocation(HitResult.ImpactPoint);
	// Draw debug line
	//DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Blue, false, -1, 0, 10.0f);


	
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

