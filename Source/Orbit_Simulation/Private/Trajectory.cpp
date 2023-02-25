// Fill out your copyright notice in the Description page of Project Settings.


#include "Trajectory.h"
#include "MeshAttributes.h"

// Sets default values
ATrajectory::ATrajectory() :
ForwardAxis(ESplineMeshAxis::Z),
NumberOfPoints(32),
SemimajorAxis(1000.0),
SemiminorAxis(1000.0),
MeshScale(FVector2D(0.05, 0.05))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Scene Root Component
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);

	// Create Spline Component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (SplineComponent)
	{
		SplineComponent->SetupAttachment(SceneRoot);
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
	

	// Initialize Material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat1(TEXT("/ControlRig/Controls/ControlRigGizmoMaterial"));
	if (Mat1.Succeeded())
	{
		DefaultMaterial = Mat1.Object;
		AlternateMaterial = Mat1.Object;
	}

	// Initialize Arrows
	SemimajorAxisArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi-major Axis"));
	if (SemimajorAxisArrow)
	{
		SemimajorAxisArrow->SetupAttachment(SceneRoot);
		SemimajorAxisArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		SemimajorAxisArrow->SetArrowColor(FLinearColor::Red);
		SemimajorAxisArrow->ArrowSize = 1.0;
		SemimajorAxisArrow->ArrowLength = SemimajorAxis;
		SemimajorAxisArrow->bUseInEditorScaling = false;
		SemimajorAxisArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		SemimajorAxisArrow->SetVisibility(false);
		SemimajorAxisArrow->SetHiddenInGame(true);
	}
	SemiminorAxisArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi-minor Axis"));
	if (SemiminorAxisArrow)
	{
		SemiminorAxisArrow->SetupAttachment(SceneRoot);
		SemiminorAxisArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		SemiminorAxisArrow->SetArrowColor(FLinearColor::Blue);
		SemiminorAxisArrow->ArrowSize = 1.0;
		SemiminorAxisArrow->ArrowLength = SemiminorAxis;
		SemiminorAxisArrow->bUseInEditorScaling = false;
		FRotator Rotation = FRotationMatrix::MakeFromX(FVector(0.0, -1.0, 0.0)).Rotator();
		SemiminorAxisArrow->SetRelativeRotation(Rotation);
		SemiminorAxisArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		SemiminorAxisArrow->SetVisibility(false);
		SemiminorAxisArrow->SetHiddenInGame(true);
	}
	
	SplineComponent->ClearSplinePoints();
	SplineMeshes.Empty();
}

void ATrajectory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ForwardAxis = ESplineMeshAxis::Z;
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	//SplineComponent->ClearSplinePoints();
	InitializeSpline();
	InitializeSplineMesh();
	Update();
	SemimajorAxisArrow->ArrowLength = SemimajorAxis;
	SemiminorAxisArrow->ArrowLength = SemiminorAxis;
}

void ATrajectory::Update()
{
	UpdateEllipse();
	UpdateSplineMesh();
	SemimajorAxisArrow->ArrowLength = SemimajorAxis;
	SemiminorAxisArrow->ArrowLength = SemiminorAxis;
}

// Called when the game starts or when spawned
void ATrajectory::BeginPlay()
{
	Super::BeginPlay();
	
	/*FString str = TEXT("Number of Spline Points: ");
	str.AppendInt(SplineComponent->GetNumberOfSplinePoints());
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, str);
	
	str = TEXT("Number of Spline Meshes: ");
	str.AppendInt(SplineMeshes.Num());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, str);*/
	
}

void ATrajectory::InitializeSpline()
{
	if(SplineComponent->GetNumberOfSplinePoints() > 0) {return;}
	FVector Center = GetActorLocation();
	double angle = 2 * PI / NumberOfPoints;

	// Parametric equation of an ellipse
	for (int i = 0; i < NumberOfPoints; i++)
	{
		double X = SemimajorAxis * cos(i * angle);
		double Y = SemiminorAxis * sin(i * angle);
		FVector Position = FVector(X, Y, Center.Z);
		SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::Local, false);
	}
	
	SplineComponent->SetClosedLoop(isClosedLoop);
	SplineComponent->UpdateSpline(); // Update Spline
}

void ATrajectory::UpdateEllipse()
{
	FVector Center = GetActorLocation();
	double angle = 2 * PI / NumberOfPoints;

	// Parametric equation of an ellipse
	for (int i = 0; i < NumberOfPoints; i++)
	{
		double X = SemimajorAxis * cos(i * angle);
		double Y = SemiminorAxis * sin(i * angle);
		FVector Position = FVector(X, Y, Center.Z);
		SplineComponent->SetLocationAtSplinePoint(i, Position, ESplineCoordinateSpace::Local, false);
	}
	
	SplineComponent->SetClosedLoop(isClosedLoop);
	SplineComponent->UpdateSpline(); // Update Spline
}

void ATrajectory::InitializeSplineMesh()
{
	if (!Mesh) {return;}

	if(SplineMeshes.Num() > 0)
	{
		SplineMeshes.Empty();
	}
	
	for (int SplineSegment = 0; SplineSegment < NumberOfPoints; SplineSegment++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// Start Point
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineSegment, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineSegment, ESplineCoordinateSpace::Local);

		// End Point
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineSegment + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineSegment + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshComponent->SetStartScale(MeshScale);
		SplineMeshComponent->SetEndScale(MeshScale);

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
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMeshComponent->SetVisibility(true);
		SplineMeshComponent->SetHiddenInGame(false);
		//SplineMeshComponent->bAllowSplineEditingPerInstance = true;
		SplineMeshes.Add(SplineMeshComponent);

	}
}

void ATrajectory::UpdateSplineMesh()
{
	for (int index = 0; index < NumberOfPoints; index++)
	{
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
		
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(index + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(index + 1, ESplineCoordinateSpace::Local);

		SplineMeshes[index]->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshes[index]->SetStartScale(MeshScale);
		SplineMeshes[index]->SetEndScale(MeshScale);
	}
}

// Called every frame
/*void ATrajectory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/
