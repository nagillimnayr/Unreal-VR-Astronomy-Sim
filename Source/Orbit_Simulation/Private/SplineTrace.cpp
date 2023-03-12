// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTrace.h"

#include "Sim.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASplineTrace::ASplineTrace() :
Duration(3650.0),
Interval(5),
MeshScale(0.2, 0.2),
ForwardAxis(ESplineMeshAxis::Z),
SplinePointCount(0),
SplineMeshCount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get reference to Sim
	TArray<AActor*> SimArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASim::StaticClass(), SimArray);
	if (SimArray.Num() > 0)
	{
		Sim = Cast<ASim>(SimArray[0]);
	}
	
	// Create Scene Root Component
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);

	// Create Spline Component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
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
	}

	// Initialize Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		Mesh = CylinderMeshAsset.Object;
	}
	
	/*// Initialize Material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Engine/VREditor/UI/KeyDiskMaterial.KeyDiskMaterial"));
	if (Mat.Succeeded())
	{
		DefaultMaterial = Mat.Object;
	}*/

	// Clear Spline
	SplineComponent->ClearSplinePoints();
	SplineMeshes.Empty();
}

void ASplineTrace::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	SetActorLocation(FVector::ZeroVector);
	SplineComponent->SetClosedLoop(false);

	SetActorEnableCollision(false);
	
}

void ASplineTrace::Destroyed()
{
	Super::Destroyed();
}

// Called when the game starts or when spawned
void ASplineTrace::BeginPlay()
{
	Super::BeginPlay();
	
	Timer = 0.0; // Reset Timer
	
	// Reset timer since update
	TimeSinceLastUpdate = 0.0;
}

void ASplineTrace::InitializeFirstPoints(const FVector& Position)
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

void ASplineTrace::AddSplineMesh()
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
	SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SplineMeshComponent->SetForwardAxis(ForwardAxis);

	if (DefaultMaterial) SplineMeshComponent->SetMaterial(0, DefaultMaterial); // Set material

	SplineMeshComponent->SetCastShadow(false); // Disable Shadows
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision
	SplineMeshComponent->SetVisibility(true);
	SplineMeshComponent->SetHiddenInGame(false);

	SplineMeshes.Add(SplineMeshComponent); // Add Spline Mesh to Array
	
	SplineMeshComponent->SetStartScale(MeshScale);
	SplineMeshComponent->SetEndScale(MeshScale);

	// Update counter
	SplineMeshCount = SplineMeshes.Num();

	// Update mesh
	UpdateLastMesh();
}

void ASplineTrace::UpdateMesh()
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	const int NumOfPoints = SplineComponent->GetNumberOfSplinePoints();
	const int NumOfMeshes = SplineMeshes.Num();
	
	for(int i = 0; i < NumOfMeshes && i < NumOfPoints - 1; i++)
	{
		// Get Start Point
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

		// Get End Point
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		// Update Spline Mesh
		SplineMeshes.Last()->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshes.Last()->SetStartScale(MeshScale);
		SplineMeshes.Last()->SetEndScale(MeshScale);
	}
}

void ASplineTrace::UpdateLastPoint(const FVector& Position)
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	if(SplineComponent->GetNumberOfSplinePoints() < 2)
	{
		InitializeFirstPoints(Position);
		return;
	}
	

	// Get index of last Spline Point
	int index = SplineComponent->GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Point
	SplineComponent->SetLocationAtSplinePoint(index, Position, ESplineCoordinateSpace::World);

	// Update position of last Spline Mesh
	UpdateLastMesh();
}

void ASplineTrace::UpdateLastMesh()
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	// Get index of last Spline Point
	const int index = SplineComponent->GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Mesh
	const FVector StartPos = SplineComponent->GetLocationAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	const FVector EndPos = SplineComponent->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
	const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
	SplineMeshes.Last()->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
}

void ASplineTrace::AddNewPoint(const FVector& Position)
{
	if (!Mesh->IsValidLowLevel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Uninitialized Mesh on SplineMesh"));
		return;
	}
	
	if(Timer >= Duration)
	{
		SplineComponent->RemoveSplinePoint(0); // Remove Oldest Spline Point
		SplineMeshes[0]->DestroyComponent(); // Destroy Oldest Spline Mesh
		SplineMeshes.RemoveAt(0); // Remove Oldest Spline Mesh
	}
	

	
	// Add spline point at position of other body
	SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::World, true);	
	
	// Update counter
	SplinePointCount = SplineComponent->GetNumberOfSplinePoints();
}

// Called every frame
void ASplineTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineTrace::Update(const FVector& Position)
{
	double DeltaTime = GetWorld()->GetDeltaSeconds();
	if(!Sim) return;
	double Delta = DeltaTime * Sim->TimeScale;
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

