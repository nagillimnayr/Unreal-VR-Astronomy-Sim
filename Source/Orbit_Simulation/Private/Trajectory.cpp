// Fill out your copyright notice in the Description page of Project Settings.


#include "Trajectory.h"

//#include "AstroBody.h"
#include "MeshAttributes.h"
#include "Orbit.h"
#include "OrbitalPlaneComponent.h"
#include "StaticMeshAttributes.h"
#include "Engine/ObjectLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Orbit_Simulation/CalculateOrbitalElements/OrbitalElements.h"

// Sets default values
ATrajectory::ATrajectory() :
ForwardAxis(ESplineMeshAxis::Z),
NumberOfPoints(36),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0),
MeshScale(FVector2D(0.05, 0.05)),
Color(FLinearColor::White)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Create Scene Root Component
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);

	// Create Spline Component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(SceneRoot);
	SplineComponent->SetCastShadow(false); // Disable Shadows
	SplineComponent->SetDrawDebug(false);
	


	// Initialize Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		Mesh = CylinderMeshAsset.Object;
	}
	
	// Load base material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/Trajectory_MI.Trajectory_MI'"));
	if (Mat.Succeeded())
	{
		BaseMaterial = Mat.Object;
	}
	
	// Initialize Arrows
	SemiMajorAxisArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi-major Axis"));
	if (SemiMajorAxisArrow->IsValidLowLevel())
	{
		SemiMajorAxisArrow->SetupAttachment(SceneRoot);
		SemiMajorAxisArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		SemiMajorAxisArrow->SetArrowColor(FLinearColor::Red);
		SemiMajorAxisArrow->ArrowSize = 1.0;
		SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
		SemiMajorAxisArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		SemiMajorAxisArrow->SetVisibility(true);
		SemiMajorAxisArrow->SetHiddenInGame(true);
	}
	SemiMinorAxisArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi-minor Axis"));
	if (SemiMinorAxisArrow->IsValidLowLevel())
	{
		SemiMinorAxisArrow->SetupAttachment(SceneRoot);
		SemiMinorAxisArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		SemiMinorAxisArrow->SetArrowColor(FLinearColor::Blue);
		SemiMinorAxisArrow->ArrowSize = 1.0;
		SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
		FRotator Rotation = FRotationMatrix::MakeFromX(FVector(0.0, -1.0, 0.0)).Rotator();
		SemiMinorAxisArrow->SetRelativeRotation(Rotation);
		SemiMinorAxisArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		SemiMinorAxisArrow->SetVisibility(true);
		SemiMinorAxisArrow->SetHiddenInGame(true);
	}
	
	SemiLatusRectumArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi Latus Rectum"));
	SemiLatusRectumArrow->SetupAttachment(SceneRoot);
	SemiLatusRectumArrow->SetArrowColor(FLinearColor::Green);
	SemiLatusRectumArrow->ArrowSize = 1.0;
	FRotator Rotation = FRotationMatrix::MakeFromX(FVector(0.0, -1.0, 0.0)).Rotator();
	SemiLatusRectumArrow->SetRelativeRotation(Rotation);
	SemiLatusRectumArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	SemiLatusRectumArrow->SetVisibility(true);
	SemiLatusRectumArrow->SetHiddenInGame(true);
	
	
	SplineComponent->ClearSplinePoints();
	SplineMeshes.Empty();

	// Create Orbital Plane Components
	OrbitalPlane = CreateDefaultSubobject<UOrbitalPlaneComponent>(TEXT("Orbital Plane"));
	OrbitalPlane->SetupAttachment(SceneRoot);


} // End of Constructor

/*void ATrajectoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UE_LOG(LogTemp, Log, TEXT("> Initializing Trajectory"));
	ForwardAxis = ESplineMeshAxis::Z;
	SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	//SplineComponent->ClearSplinePoints();
	InitializeMaterial();
	InitializeSpline();
	InitializeSplineMesh();
	Draw();
	SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
	
	if(IsValid(OrbitalPlane)) OrbitalPlane->SetAxes(SemiMajorAxis, SemiMinorAxis);

	SetRelativeLocation(FVector::ZeroVector);

	UpdateArrows();

}*/

void ATrajectory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	/*ForwardAxis = ESplineMeshAxis::Z;
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	//SplineComponent->ClearSplinePoints();
	InitializeMaterial();
	InitializeSpline();
	InitializeSplineMesh();
	Draw();
	SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
	
	if(IsValid(OrbitalPlane)) OrbitalPlane->SetAxes(SemiMajorAxis, SemiMinorAxis);

	SetActorRelativeLocation(FVector::ZeroVector);
	
	// Disable Collision
	SetActorEnableCollision(false);

	UpdateArrows();*/
	Initialize();
}

void ATrajectory::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("> Initializing Trajectory"));
	ForwardAxis = ESplineMeshAxis::Z;
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	//SplineComponent->ClearSplinePoints();
	InitializeMaterial();
	InitializeSpline();
	InitializeSplineMesh();
	Draw();
	SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
	
	if(IsValid(OrbitalPlane)) OrbitalPlane->SetAxes(SemiMajorAxis, SemiMinorAxis);

	SetActorRelativeLocation(FVector::ZeroVector);

	UpdateArrows();
}

void ATrajectory::SetAxes(const double a, const double b)
{
	SetSemiMajorAxis( FMath::Max(a, b)); 
	SetSemiMinorAxis(FMath::Min(a, b)); 

	if(IsValid(OrbitalPlane)) OrbitalPlane->SetAxes(SemiMajorAxis, SemiMinorAxis);
	
	//Draw();
}

void ATrajectory::Draw()
{
	UpdateEllipse();
	UpdateSplineMesh();
	UpdateArrows();
}

void ATrajectory::SetColor(const FLinearColor NewColor)
{
	Color = NewColor;
	InitializeMaterial();
	if(IsValid(OrbitalPlane))
	{
		OrbitalPlane->SetColor(Color);
	}
}

// Called when the game starts or when spawned
void ATrajectory::BeginPlay()
{
	Super::BeginPlay();
}

void ATrajectory::InitializeSpline()
{
	if(SplineComponent->GetNumberOfSplinePoints() > 0)
	{
		SplineComponent->ClearSplinePoints();
	}

	const double Eccentricity = Eccentricity::CalculateFromAxes(SemiMajorAxis, SemiMinorAxis);
	//const double SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	const double LinearEccentricity = LinearEccentricity::CalculateFromEccentricity(SemiMajorAxis, Eccentricity);
	const double Angle = (2 * PI) / NumberOfPoints;

	// Parametric equation of an ellipse
	for (int i = 0; i < NumberOfPoints; i++)
	{
		double X = SemiMajorAxis * cos(i * Angle);
		double Y = -(SemiMinorAxis * sin(i * Angle)); // Negative so that it will go counter-clockwise around the Z-axis
		FVector Position(X - LinearEccentricity, Y, 0.0);
		//FVector Position = PolarCoordinates(Angle * i, Eccentricity, SemiLatusRectum);
		SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::Local, false);
	}
	
	SplineComponent->SetClosedLoop(isClosedLoop);
	SplineComponent->UpdateSpline(); // Update Spline
}

void ATrajectory::UpdateEllipse()
{
	const double Eccentricity = Eccentricity::CalculateFromAxes(SemiMajorAxis, SemiMinorAxis);
	//const double SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	const double LinearEccentricity = LinearEccentricity::CalculateFromEccentricity(SemiMajorAxis, Eccentricity);
	const double Angle = (2 * PI) / SplineComponent->GetNumberOfSplinePoints();

	// Parametric equation of an ellipse
	for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
	{
		double X = SemiMajorAxis * cos(i * Angle);
		double Y = -(SemiMinorAxis * sin(i * Angle)); // Negative so that it will go counter-clockwise around the Z-axis
		FVector Position(X - LinearEccentricity, Y, 0.0);
		//FVector Position = PolarCoordinates(Angle * i, Eccentricity, SemiLatusRectum);
		SplineComponent->SetLocationAtSplinePoint(i, Position, ESplineCoordinateSpace::Local, false);
	}
	
	SplineComponent->SetClosedLoop(isClosedLoop);
	SplineComponent->UpdateSpline(); // Update Spline
}


FVector ATrajectory::GetCenter()
{
	// Get the position of the Periapsis from the first Spline Point 
	FVector Periapsis = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	// Get the length of the radius at Periapsis
	double PeriapsisRadius = Periapsis.Length();
	// Get the LinearEccentricity
	double LinearEccentricity = SemiMajorAxis - PeriapsisRadius;

	// Get the Direction Vector by negating the Forward Vector
	FVector Direction = -GetActorForwardVector();
	// Get the center of the Ellipse
	FVector Center = GetActorLocation() + (Direction * LinearEccentricity);
	
	return Center;
}

void ATrajectory::UpdateArrows()
{
	// Set Positions of the Arrows to be the center of the Ellipse
	const double Eccentricity = Eccentricity::CalculateFromAxes(SemiMajorAxis, SemiMinorAxis);
	const double LinearEccentricity = LinearEccentricity::CalculateFromEccentricity(SemiMajorAxis, Eccentricity);
	FVector Position = FVector(-LinearEccentricity, 0.0, 0.0);
	SemiMajorAxisArrow->SetRelativeLocation(Position);
	SemiMinorAxisArrow->SetRelativeLocation(Position);

	const double SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	SemiLatusRectumArrow->ArrowLength = SemiLatusRectum;
	SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
}

FVector ATrajectory::GetPeriapsisVector()
{
	return SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
}

void ATrajectory::InitializeMaterial()
{
	
	// Initialize Material
	if(IsValid(BaseMaterial) && !IsValid(MaterialInstance))
	{
		// Create Dynamic Material Instance
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, FName("Trajectory Dyn_Mat_Inst"));
	}
	if(IsValid(MaterialInstance))
	{
		MaterialInstance->SetVectorParameterValue(FName("Color"), Color);
		//UpdateSplineMesh();
	}
}

/*void ATrajectory::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName Name = PropertyChangedEvent.GetPropertyName();
	if(Name == "BaseMaterial")
	{
		// If Base Material changed, reset Material Instance
		MaterialInstance = nullptr;
		InitializeMaterial();
	}
	else if(Name == FName("Color"))
	{
		// If Color changed, change Material Instance parameter
		if(IsValid(OrbitalPlane)) OrbitalPlane->SetColor(Color);
		InitializeMaterial();
	}
	else if(Name == FName("SemiMajorAxis") || Name == FName("SemiMinorAxis"))
	{
		if(IsValid(OrbitalPlane)) OrbitalPlane->SetAxes(SemiMajorAxis, SemiMinorAxis);
	}
}*/

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
		if (MaterialInstance) SplineMeshComponent->SetMaterial(0, MaterialInstance);
		
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
	for (int index = 0; index < SplineComponent->GetNumberOfSplinePoints(); index++)
	{
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
		
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(index + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(index + 1, ESplineCoordinateSpace::Local);

		SplineMeshes[index]->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshes[index]->SetStartScale(MeshScale);
		SplineMeshes[index]->SetEndScale(MeshScale);
		
		if (MaterialInstance) SplineMeshes[index]->SetMaterial(0, MaterialInstance);
	}
}

// Called every frame
/*void ATrajectory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/
