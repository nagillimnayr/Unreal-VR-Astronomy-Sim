// Fill out your copyright notice in the Description page of Project Settings.


#include "EllipseMeshComponent.h"



UEllipseMeshComponent::UEllipseMeshComponent(const FObjectInitializer& ObjectInitializer) : UProceduralMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	bUseAttachParentBound = true;
	SetCanEverAffectNavigation(false);
	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/OrbitalPlane_MI.OrbitalPlane_MI'"));
	if(MatAsset.Succeeded())
	{
		BaseMaterial = MatAsset.Object;
		// Initialize Material
		if(IsValid(BaseMaterial))
		{ 
			// Create Dynamic Material Instance
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, FName("Orbital Plane Dyn_Mat_Inst"));
			if(IsValid(MaterialInstance))
			{
				Super::SetMaterial(0, MaterialInstance);
			}
		}
	}
}

void UEllipseMeshComponent::CreateMesh(int NumberOfSides, const double SemiMajorAxis, const double SemiMinorAxis, const FLinearColor Color)
{
	// Reset Mesh
	Super::ClearAllMeshSections();
	
	if(NumberOfSides < 4) NumberOfSides = 4;
	TArray<FVector> Vertices; // Array of vertices
	TArray<int32> Triangles; // Array of triangles
	TArray<FVector> Normals; // Array of normals
	TArray<FVector2d> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
	Vertices.Emplace(0.0, 0.0, 0.0); // Center of the plane
	// Populate the array with normals pointing up the Z-axis
	Normals.Init(FVector::ZAxisVector, NumberOfSides + 1);
	UV0.Emplace(0.5, 0.5);
	VertexColors.Init(Color, NumberOfSides + 1);
	Tangents.Init(FProcMeshTangent(1.0, 0.0, 0.0), NumberOfSides + 1);

	// Parametric formula for an ellipse
	double Angle = (2.0 * PI) / NumberOfSides;
	for(int32 i = 0; i < NumberOfSides; i++)
	{
		// Parametric formula for an ellipse
		double X = SemiMajorAxis * FMath::Cos(Angle * i);
		double Y = SemiMinorAxis * FMath::Sin(Angle * i);
		Vertices.Emplace(X, Y, 0.0); // Add vertex to the array
	}

	for(int32 i = 1; i < NumberOfSides; i++)
	{
		// Each triangle will have its starting vertex as the center vertex
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}
	// Last triangle will have its last vertex as vertex 1
	Triangles.Add(0);
	Triangles.Add(NumberOfSides);
	Triangles.Add(1);
	
	// UVs
	const double Ratio = SemiMajorAxis / SemiMinorAxis;
	for(int32 i = 0; i < NumberOfSides; i++)
	{
		// Parametric formula for an ellipse
		const double X = 0.5 + (Ratio * FMath::Cos(Angle * i));
		const double Y = 0.5 + FMath::Sin(Angle * i);
		UV0.Add(FVector2d(X, Y));
	}
	
	CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	ContainsPhysicsTriMeshData(true);
	CastShadow = false;

	if(IsValid(BaseMaterial) && !IsValid(MaterialInstance)) {CreateMaterialInstance();}

	if(IsValid(MaterialInstance)) SetMaterial(0, MaterialInstance);
	SetColor(Color);
}

void UEllipseMeshComponent::SetColor(const FLinearColor Color)
{
	if(IsValid(MaterialInstance))
	{
		MaterialInstance->SetVectorParameterValue(FName("Color"), Color);
	}
}

void UEllipseMeshComponent::SetNewMaterial(UMaterialInterface* Material)
{
	if(!IsValid(Material)) return;

	BaseMaterial = Material;
	CreateMaterialInstance();
}

void UEllipseMeshComponent::CreateMaterialInstance()
{
	if(!IsValid(BaseMaterial)) return;

	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, FName("Orbital Plane Dyn_Mat_Inst"));
	if(IsValid(MaterialInstance))
	{
		Super::SetMaterial(0, MaterialInstance);
	}
}
