// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalPathComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"

UOrbitalPathComponent::UOrbitalPathComponent() :
bMeshVisibility(true),
MeshScale(FVector2D(0.05, 0.05))
{
	ClearSplinePoints();
	
	// Initialize Mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Orbit_Sim/Meshes/cylinder_open.cylinder_open'"));
	if (CylinderMeshAsset.Succeeded())
	{
		Mesh = CylinderMeshAsset.Object;
	}

	// Initialize Material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/Trajectory_MI.Trajectory_MI'"));
	if(MatAsset.Succeeded())
	{
		BaseMaterial = MatAsset.Object;
		// Initialize Material
		if(IsValid(BaseMaterial))
		{ 
			// Create Dynamic Material Instance
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, FName("Orbital Path Dyn_Mat_Inst"));
			
		}
	}
	SetClosedLoop(true);
	SetCanEverAffectNavigation(false);
	SetDrawDebug(false);
}

void UOrbitalPathComponent::InitializeSpline(const double SemiMajorAxis, const double SemiMinorAxis)
{
	// Reset Spline Points
	ClearSplinePoints();

	const double Eccentricity = Eccentricity::CalculateFromAxes(SemiMajorAxis, SemiMinorAxis);
	//const double SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	const double LinearEccentricity = LinearEccentricity::CalculateFromEccentricity(SemiMajorAxis, Eccentricity);
	const double Angle = (2 * PI) / NumberOfPoints;

	// Parametric equation of an ellipse
	for (int i = 0; i < NumberOfPoints; i++)
	{
		double X = SemiMajorAxis * FMath::Cos(i * Angle);
		double Y = -(SemiMinorAxis * FMath::Sin(i * Angle)); // Negative so that it will go counter-clockwise around the Z-axis
		FVector Position(X - LinearEccentricity, Y, 0.0);
		AddSplinePoint(Position, ESplineCoordinateSpace::Local, false);
	}
	
	UpdateSpline(); // Update Spline
}

void UOrbitalPathComponent::InitializeSplineMesh(const double SemiMajorAxis, const double SemiMinorAxis)
{
	if (!IsValid(Mesh)) {return;}
	
	// Reset Meshes
	SplineMeshes.Empty();
	
	for (int PointIndex = 0; PointIndex < NumberOfPoints; PointIndex++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		// Start Point
		const FVector StartPoint = GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local);
		const FVector StartTangent = GetTangentAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local);

		// End Point
		const FVector EndPoint = GetLocationAtSplinePoint(PointIndex + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = GetTangentAtSplinePoint(PointIndex + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshComponent->SetStartScale(MeshScale);
		SplineMeshComponent->SetEndScale(MeshScale);

		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
		if (IsValid(MaterialInstance)) SplineMeshComponent->SetMaterial(0, MaterialInstance);
		
		SplineMeshComponent->SetCastShadow(false); // Disable Shadows
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMeshComponent->SetCanEverAffectNavigation(false);

		SplineMeshComponent->SetVisibility(bMeshVisibility);
		
		SplineMeshes.Add(SplineMeshComponent);

	}
}

void UOrbitalPathComponent::DrawPath(const double SemiMajorAxis, const double SemiMinorAxis)
{
	if(!IsValid(MaterialInstance)) {CreateMaterialInstance();}
	
	InitializeSpline(SemiMajorAxis, SemiMinorAxis);
	InitializeSplineMesh(SemiMajorAxis, SemiMinorAxis);
}

void UOrbitalPathComponent::SetNewMaterial(UMaterialInterface* Material)
{
	if(!IsValid(Material)) return;

	BaseMaterial = Material;
	CreateMaterialInstance();
}

void UOrbitalPathComponent::CreateMaterialInstance()
{
	if(!IsValid(BaseMaterial)) return;

	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, FName("Orbital Path Dyn_Mat_Inst"));
	if(IsValid(MaterialInstance))
	{
		for(USplineMeshComponent* SplineMeshComponent : SplineMeshes)
		{
			if(!IsValid(SplineMeshComponent))
			{
				SplineMeshes.Remove(SplineMeshComponent);
				continue;
			}
			// Set material for each spline mesh
			SplineMeshComponent->SetMaterial(0, MaterialInstance);
		}
	}
}

void UOrbitalPathComponent::SetMeshVisibility(bool bVisibility)
{
	bMeshVisibility = bVisibility;
}

void UOrbitalPathComponent::SetColor(const FLinearColor Color)
{
	if(IsValid(MaterialInstance))
	{
		MaterialInstance->SetVectorParameterValue(FName("Color"), Color);
	}
}
