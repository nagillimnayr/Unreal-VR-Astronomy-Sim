// Fill out your copyright notice in the Description page of Project Settings.


#include "Trajectory.h"

#include "AstroBody.h"
#include "MeshAttributes.h"
#include "Orbit.h"
#include "StaticMeshAttributes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Orbit_Simulation/CalculateOrbitalElements/OrbitalElements.h"

// Sets default values
ATrajectory::ATrajectory() :
ForwardAxis(ESplineMeshAxis::Z),
NumberOfPoints(48),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0),
MeshScale(FVector2D(0.05, 0.05))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
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
	SemiMajorAxisArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi-major Axis"));
	if (SemiMajorAxisArrow->IsValidLowLevel())
	{
		SemiMajorAxisArrow->SetupAttachment(SceneRoot);
		SemiMajorAxisArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		SemiMajorAxisArrow->SetArrowColor(FLinearColor::Red);
		SemiMajorAxisArrow->ArrowSize = 1.0;
		SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
		SemiMajorAxisArrow->bUseInEditorScaling = false;
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
		SemiMinorAxisArrow->bUseInEditorScaling = false;
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
	SemiLatusRectumArrow->bUseInEditorScaling = false;
	FRotator Rotation = FRotationMatrix::MakeFromX(FVector(0.0, -1.0, 0.0)).Rotator();
	SemiLatusRectumArrow->SetRelativeRotation(Rotation);
	SemiLatusRectumArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	SemiLatusRectumArrow->SetVisibility(true);
	SemiLatusRectumArrow->SetHiddenInGame(true);
	
	
	SplineComponent->ClearSplinePoints();
	SplineMeshes.Empty();

	// Initialize Ascending Node Marker
	AscendingNodeMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ascending Node Marker"));
	AscendingNodeMarker->SetupAttachment(SceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarkerMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Orbit_Sim/Meshes/RotationHandleIndicator.RotationHandleIndicator'"));
	
	if (MarkerMeshAsset.Succeeded())
	{
		AscendingNodeMarker->SetStaticMesh(MarkerMeshAsset.Object);
		AscendingNodeMarker->SetRelativeLocation(FVector::ZeroVector);
		AscendingNodeMarker->CastShadow = false; // Disable shadows
	}
} // End of Constructor

void ATrajectory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ForwardAxis = ESplineMeshAxis::Z;
	SetActorScale3D(FVector(1.0, 1.0, 1.0));
	//SplineComponent->ClearSplinePoints();
	InitializeSpline();
	InitializeSplineMesh();
	Draw();
	SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;

	SetActorRelativeLocation(FVector::ZeroVector);
	
	// Disable Collision
	SetActorEnableCollision(false);

	UpdateArrows();
}

void ATrajectory::SetAxes(const double a, const double b)
{
	SetSemiMajorAxis( FMath::Max(a, b)); 
	SetSemiMinorAxis(FMath::Min(a, b)); 

	UpdateArrows();
}

void ATrajectory::Draw()
{
	UpdateEllipse();
	UpdateSplineMesh();
	UpdateArrows();
}

void ATrajectory::PositionAscendingNodeMarker(double Angle, double Distance)
{
	if(!SplineComponent) return;
	
	FVector ReferenceAxis = GetActorForwardVector(); 
	FVector RotatedVector = ReferenceAxis.RotateAngleAxis(Angle, GetActorUpVector());
	

	/*const double Distance = Fraction * SplineComponent->GetSplineLength();
	//const FVector Position = SplineComponent->GetLocationAtTime(TimeFraction, ESplineCoordinateSpace::Local, false);
	const FVector Position = SplineComponent->GetWorldLocationAtDistanceAlongSpline(Distance);*/

	//double Distance = Orbit->GetRadiusAtTrueAnomaly(Angle);
	FVector Position = RotatedVector * Distance;
	AscendingNodeMarker->SetWorldLocation(Position);
	// Draw debug line
	//DrawDebugLine(GetWorld(), GetActorLocation(), Position, FColor::Red, true, -1, 0, 10.0f);

}


void ATrajectory::RotateSpline(const double Angle)
{
	//FVector Forward = FVector::XAxisVector;
	SplineComponent->SetRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	
	//FVector RotatedVector = Forward.RotateAngleAxis(Angle, GetActorUpVector());
	//FRotator Rotation = FRotationMatrix::MakeFromX(RotatedVector).Rotator(); // Get Rotator from Vector

	//SplineComponent->SetRelativeRotation(Rotation);
	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, Angle)); // Get Rotator from Vector
	SplineComponent->AddLocalRotation(Rotation);
	
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

FVector ATrajectory::PolarCoordinates(const double Angle, const double Eccentricity, const double SemiLatusRectum)
{
	// Returns the Polar Coordinates of a point on an ellipse at the given angle (Relative to the focus at the origin)

	// Get Polar Coordinates, convert to cartesian coordinates, then construct a Vector from the X and Y coordinates
	double Radius = OrbitalRadius::CalculateRadiusAtTrueAnomaly(Angle, Eccentricity, SemiLatusRectum);
	const double X = Radius * FMath::Cos(Angle);
	const double Y = -(Radius * FMath::Sin(Angle)); // Negative so that it will go counter-clockwise around the Z-axis
	return FVector(X, Y, 0.0);
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
	for (int index = 0; index < SplineComponent->GetNumberOfSplinePoints(); index++)
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
