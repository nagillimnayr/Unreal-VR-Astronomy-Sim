// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalPlaneComponent.h"

#include "StaticMeshAttributes.h"


// Sets default values
UOrbitalPlaneComponent::UOrbitalPlaneComponent() :
GridSize(10.0),
NumberOfSides(36),
SemiMajorAxis(100.0),
SemiMinorAxis(100.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;


	// Initialize Procedural Mesh
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->CastShadow = false;

	// Initialize Materials
	//ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/OrbitalPlane_MI.OrbitalPlane_MI'"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> OrbitMatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/OrbitalPlane_MI.OrbitalPlane_MI'"));
	if(OrbitMatAsset.Succeeded())
	{
		OrbitMaterial = OrbitMatAsset.Object;
		
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/Grid_Opacity_MI.Grid_Opacity_MI'"));
	if(GridMatAsset.Succeeded())
	{
		GridMaterial = GridMatAsset.Object;
	}
	Material = OrbitMaterial;
}

// Called when the game starts or when spawned
void UOrbitalPlaneComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(Material)
		Mesh->SetMaterial(0, Material);
	
}

/*void AOrbitalPlane::PostActorCreated()
{
	Super::PostActorCreated();
	
	CreateEllipse();
	if(Material)
		Mesh->SetMaterial(0, Material);
}*/

void UOrbitalPlaneComponent::PostLoad()
{
	Super::PostLoad();
	Initialize();
}

void UOrbitalPlaneComponent::Initialize()
{
	if(GridMaterial && !GridMaterialInstance)
	{ 
		GridMaterialInstance = CreateDynamicMaterialInstance(0, GridMaterial);
		SetGridSize(GridSize);
	}
	CreateEllipse();
	if(Material)
		Mesh->SetMaterial(0, Material);

	Mesh->AttachToComponent(this,
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			false
		)
	);
}

void UOrbitalPlaneComponent::SetAxes(double a, double b)
{
	SemiMajorAxis = a;
	SemiMinorAxis = b;
	Initialize();
}

/*void AOrbitalPlane::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateEllipse();
	
}*/

void UOrbitalPlaneComponent::CreateEllipse()
{
	if(NumberOfSides < 4) NumberOfSides = 4;
	if(!Mesh) return;
	
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
	VertexColors.Init(FLinearColor(0.75, 0.75, 0.75, 1.0), NumberOfSides + 1);
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
	
	/*for(int32 i = 0; i < NumberOfSides; i++)
	{
		// Parametric formula for an ellipse
		const double X = 0.5 + (SemiMajorAxis * FMath::Cos(Angle * i));
		const double Y = 0.5 + (SemiMinorAxis * FMath::Sin(Angle * i));
		UV0.Add(FVector2d(X, Y));
	}*/

	/*Tangents.Add(FProcMeshTangent(0.0, 1.0, 0.0));
	Tangents.Add(FProcMeshTangent(0.0, 1.0, 0.0));
	Tangents.Add(FProcMeshTangent(0.0, 1.0, 0.0));*/

	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	Mesh->ContainsPhysicsTriMeshData(true);
	
	Mesh->CastShadow = false;
	
	if(Material)
	{
		Mesh->SetMaterial(0, Material);
	}
}


void UOrbitalPlaneComponent::SetGridSize(const double Size)
{
	GridSize = Size;
	if(GridMaterialInstance)
	{
		// Set Grid Size in Material Instance Parameter
		GridMaterialInstance->SetScalarParameterValue(FName("Grid Size"), GridSize);
	}
}

void UOrbitalPlaneComponent::PostInitProperties()
{
	Super::PostInitProperties();

	Initialize();
}

void UOrbitalPlaneComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Initialize();
	const FName Name = PropertyChangedEvent.GetPropertyName();
	if(Name == "GridSize")
	{
		SetGridSize(GridSize);
	}
}

