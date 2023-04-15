// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTraceComponent.h"

#include "Simulation.h"
#include "SimulationSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
USplineTraceComponent::USplineTraceComponent() :
Lifespan(100.0),
Interval(1.0),
MeshScale(0.1, 0.1),
ForwardAxis(ESplineMeshAxis::Z),
SplinePointCount(0),
SplineMeshCount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryComponentTick.bCanEverTick = true;
	// Create Scene Root Component
	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	//SetRootComponent(SceneRoot);

	// Get reference to Simulation
	/*TArray<AActor*> SimArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimulation::StaticClass(), SimArray);
	if(SimArray.Num() > 0)
	{
		Sim = Cast<ASimulation>(SimArray[0]);
	}*/
	

	// Create Spline Component
	/*SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (SplineComponent)
	{
		SplineComponent->SetupAttachment(SceneRoot);
		SplineComponent->SetRelativeLocation(FVector::ZeroVector);
		SplineComponent->SetCastShadow(false); // Disable Shadows
		SplineComponent->SetDrawDebug(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Creating Spline Component"));
	}*/

	SetCastShadow(false); // Disable Shadows
	SetDrawDebug(false);
	
	// Initialize Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		Mesh = CylinderMeshAsset.Object;
	}
	
	// Initialize Material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Engine/VREditor/UI/KeyDiskMaterial.KeyDiskMaterial"));
	if (Mat.Succeeded())
	{
		MeshMaterial = Mat.Object;
	}

	// Clear Spline
	//SplineComponent->ClearSplinePoints();
	ClearSplinePoints();
	SplineMeshes.Empty();
}

/*
void USplineTraceComponent::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	SetActorLocation(FVector::ZeroVector);
	SplineComponent->SetClosedLoop(false);

	SetActorEnableCollision(false);
	
}

void USplineTraceComponent::Destroyed()
{
	Super::Destroyed();
}*/

// Called when the game starts or when spawned
void USplineTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
	Timer = 0.0; // Reset Timer
	// Reset timer since update
	TimeSinceLastUpdate = 0.0;

}

void USplineTraceComponent::Initialize()
{
	SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
	SetRelativeLocation(FVector::ZeroVector);
	SetClosedLoop(false);

	// Get Simulation Subsystem
	Simulation = Cast<USimulationSubsystem>(GetWorld()->GetSubsystem<USimulationSubsystem>());
}

void USplineTraceComponent::InitializeFirstPoints(const FVector& Position)
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}

	// Add two initial points and one Spline Mesh between them
	AddNewPoint(Position);
	AddNewPoint(Position);
	AddSplineMesh();
	//UpdateMesh();
}

void USplineTraceComponent::AddSplineMesh()
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	// Add Spline Mesh Component
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMeshComponent->SetMobility(EComponentMobility::Movable); // Set Mobility
	SplineMeshComponent->SetStaticMesh(Mesh);
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	SplineMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	SplineMeshComponent->SetForwardAxis(ForwardAxis);

	if (MeshMaterial) SplineMeshComponent->SetMaterial(0, MeshMaterial); // Set material

	SplineMeshComponent->SetCastShadow(false); // Disable Shadows
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision
	SplineMeshComponent->SetVisibility(true);
	SplineMeshComponent->SetHiddenInGame(false);

	SplineMeshes.Add(SplineMeshComponent); // Add Spline Mesh to Array
	
	SplineMeshComponent->SetStartScale(MeshScale);
	SplineMeshComponent->SetEndScale(MeshScale);

	if(MeshMaterial) SplineMeshComponent->SetMaterial(0, MeshMaterial);
	
	// Update counter
	SplineMeshCount = SplineMeshes.Num();

	// Update mesh
	UpdateLastMesh();
}

void USplineTraceComponent::UpdateMesh()
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	const int NumOfPoints = GetNumberOfSplinePoints();
	const int NumOfMeshes = SplineMeshes.Num();
	
	for(int i = 0; i < NumOfMeshes && i < NumOfPoints - 1; i++)
	{
		// Get Start Point
		const FVector StartPoint = GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

		// Get End Point
		const FVector EndPoint = GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		// Update Spline Mesh
		SplineMeshes.Last()->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshes.Last()->SetStartScale(MeshScale);
		SplineMeshes.Last()->SetEndScale(MeshScale);
	}
}

void USplineTraceComponent::UpdateLastPoint(const FVector& Position)
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	if(GetNumberOfSplinePoints() < 2)
	{
		InitializeFirstPoints(Position);
		return;
	}
	

	// Get index of last Spline Point
	int index = GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Point
	SetLocationAtSplinePoint(index, Position, ESplineCoordinateSpace::World);

	// Update position of last Spline Mesh
	UpdateLastMesh();
}

void USplineTraceComponent::UpdateLastMesh()
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	// Get index of last Spline Point
	const int index = GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Mesh
	const FVector StartPos = GetLocationAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	const FVector EndPos = GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
	const FVector StartTangent = GetTangentAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	const FVector EndTangent = GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
	SplineMeshes.Last()->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
}

void USplineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USplineTraceComponent::AddNewPoint(const FVector& Position)
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	if(Timer >= Lifespan)
	{
		RemoveSplinePoint(0); // Remove Oldest Spline Point
		SplineMeshes[0]->DestroyComponent(); // Destroy Oldest Spline Mesh
		SplineMeshes.RemoveAt(0); // Remove Oldest Spline Mesh
	}
	

	
	// Add spline point at position of other body
	AddSplinePoint(Position, ESplineCoordinateSpace::World, true);	
	
	// Update counter
	SplinePointCount = GetNumberOfSplinePoints();
}

// Called every frame
/*void USplineTraceComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void USplineTraceComponent::Update(const FVector& Position)
{
	double DeltaTime = GetWorld()->GetDeltaSeconds();
	if(!Simulation) return;
	
	double Delta = DeltaTime * Simulation->GetTimeScale();
	// Update timers
	Timer += Delta;
	TimeSinceLastUpdate += Delta;

	// Rather than simply adding a new point every interval, which would look very abrupt,
	// we can update just the position of the last point each tick, and add a new point
	// at each interval. This way we can have longer intervals, and need to have less points overall

	// If not enough time has passed between updates, update position of last Spline Point
	if(TimeSinceLastUpdate < Interval)
	{
		UpdateLastPoint(Position);
	}
	else
	{
		AddNewPoint(Position);
		AddSplineMesh();
		TimeSinceLastUpdate = 0.0;
	}
	
	//UpdateMesh();
}



void USplineTraceComponent::SetMeshMaterial(UMaterialInterface* Material)
{
	MeshMaterial = Material;
}

