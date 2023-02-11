// Fill out your copyright notice in the Description page of Project Settings.


#include "Trajectory.h"

// Sets default values
ATrajectory::ATrajectory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Updating Position"));
	}
	
}

void ATrajectory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!Mesh) {return;}

	/*SplineComponent->AddPoint();
	SplineComponent->AddPoints();
	SplineComponent->AddSplinePoint();*/
	
	for (int SplineSegment = 0; SplineSegment < SplineComponent->GetNumberOfSplinePoints() - 1; SplineSegment++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineSegment, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineSegment, ESplineCoordinateSpace::Local);
		
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineSegment + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineSegment + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

		SplineMeshComponent->SetForwardAxis(ForwardAxis);

		if (SplineSegment % 2 == 0)
		{
			if (DefaultMaterial) SplineMeshComponent->SetMaterial(0, DefaultMaterial);
		}
		else
		{
			if (AlternateMaterial) SplineMeshComponent->SetMaterial(0, AlternateMaterial);
		}
		
	}
}

// Called when the game starts or when spawned
void ATrajectory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrajectory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

