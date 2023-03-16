// Fill out your copyright notice in the Description page of Project Settings.


#include "RetrogradePath.h"
#include "AstroBody.h"
#include "System.h"
#include "ToolBuilderUtil.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARetrogradePath::ARetrogradePath() :
ReferenceBody(nullptr),
OtherBody(nullptr),
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASystem::StaticClass(), SimArray);
	if (SimArray.Num() > 0)
	{
		Sim = Cast<ASystem>(SimArray[0]);
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
	
	SplineComponent->ClearSplinePoints();
	SplineMeshes.Empty();
}

void ARetrogradePath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Add self to Sim's array
	Sim->AddRetrogradePath(this);

	SetActorScale3D(FVector(1.0, 1.0, 1.0));

	if(!ReferenceBody) return;

	// Attach to Reference Body
	SetActorLocation(ReferenceBody->GetActorLocation());
	AttachToActor(ReferenceBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	SplineComponent->SetClosedLoop(false);
	
	SetActorEnableCollision(false);
}

void ARetrogradePath::Destroyed()
{
	Super::Destroyed();
	Sim->RemoveRetrogradePath(this);
}

// Called when the game starts or when spawned
void ARetrogradePath::BeginPlay()
{
	Super::BeginPlay();
	
	Timer = 0.0; // Reset Timer
	SplineComponent->SetWorldLocation(ReferenceBody->GetActorLocation());

	// Create first two points
	InitializeFirstPoints();

	// Reset timer since update
	TimeSinceLastUpdate = 0.0;
}

void ARetrogradePath::InitializeFirstPoints()
{
	if (!OtherBody || !SplineComponent || !Mesh) return;

	// Add two initial points and one Spline Mesh between them
	AddNewPoint();
	AddNewPoint();
	AddSplineMesh();
	//UpdateMesh();
	
}

void ARetrogradePath::AddSplineMesh()
{
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

void ARetrogradePath::UpdateMesh()
{
	int NumOfPoints = SplineComponent->GetNumberOfSplinePoints();
	int NumOfMeshes = SplineMeshes.Num();
	
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

// Called every frame
void ARetrogradePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		UpdateLastPoint();
	}
	else
	{
		AddNewPoint();
		AddSplineMesh();
		TimeSinceLastUpdate = 0.0;
	}
	
	//UpdateMesh();
}

void ARetrogradePath::UpdateLastPoint()
{
	if(!OtherBody || !SplineComponent || !Mesh) return;
	if(SplineComponent->GetNumberOfSplinePoints() < 2) return;
	
	// Get position of other body
	FVector Position = OtherBody->GetActorLocation();

	// Get index of last Spline Point
	int index = SplineComponent->GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Point
	SplineComponent->SetLocationAtSplinePoint(index, Position, ESplineCoordinateSpace::World);

	// Update position of last Spline Mesh
	UpdateLastMesh();
}

void ARetrogradePath::UpdateLastMesh()
{
	if(!SplineComponent) return;
	if(SplineMeshes.Num() == 0) return;
	if(!SplineMeshes.Last()) return;
	
	// Get index of last Spline Point
	int index = SplineComponent->GetNumberOfSplinePoints() - 1;
	
	// Update position of last Spline Mesh
	FVector StartPos = SplineComponent->GetLocationAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	FVector EndPos = SplineComponent->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
	FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
	FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
	SplineMeshes.Last()->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
}

void ARetrogradePath::AddNewPoint()
{
	if(!OtherBody || !SplineComponent || !Mesh) return;

	if(Timer >= Duration)
	{
		SplineComponent->RemoveSplinePoint(0); // Remove Oldest Spline Point
		SplineMeshes[0]->DestroyComponent(); // Destroy Oldest Spline Mesh
		SplineMeshes.RemoveAt(0); // Remove Oldest Spline Mesh
	}
	
	// Get position of other body
	FVector Position = OtherBody->GetActorLocation();
	
	// Add spline point at position of other body
	SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::World, true);	
	
	// Update counter
	SplinePointCount = SplineComponent->GetNumberOfSplinePoints();

}

