// Fill out your copyright notice in the Description page of Project Settings.


#include "Trajectory.h"
#include "MeshAttributes.h"

// Sets default values
ATrajectory::ATrajectory() :
NumberOfPoints(360),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
		SplineComponent->SetCastShadow(false); // Disable Shadows
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Creating Spline Component"));
	}
	
}

void ATrajectory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DrawEllipse();
	UpdateSplineMesh();
}

// Called when the game starts or when spawned
void ATrajectory::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrajectory::DrawEllipse()
{
	// Clear old spline
	SplineComponent->ClearSplinePoints(false);
	
	double angle = 2 * PI / NumberOfPoints;
	// Parametric equation of an ellipse
	for (int i = 0; i <= NumberOfPoints; i++)
	{
		double X = SemiMajorAxis * cos(i * angle);
		double Y = SemiMinorAxis * sin(i * angle);
		FVector Position = FVector(X, Y, 0);
		SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::World, false);
	}
	SplineComponent->UpdateSpline(); // Update Spline
}

void ATrajectory::UpdateSplineMesh()
{
	if (!Mesh) {return;}
	int NumberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
	/*for (auto SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
	}*/
	/*SplineMeshes.Empty();
	SplineMeshes.Reserve(NumberOfSplinePoints + 1);*/
	/*if (NumberOfPoints != NumberOfSplinePoints)
	{
		DrawEllipse();
	}*/

	//if (!SplineComponent->IsClosedLoop()) { NumberOfSplinePoints--; }
	
	for (int SplineSegment = 0; SplineSegment < NumberOfSplinePoints; SplineSegment++)
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
			else SplineMeshComponent->SetMaterial(0, DefaultMaterial);
		}
		
		SplineMeshComponent->SetCastShadow(false); // Disable Shadows

		//SplineMeshes.Add(SplineMeshComponent); // Add Spline Mesh to array
	}
}

// Called every frame
void ATrajectory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
