// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"
#include "AstroBody.h"
#include "System.h"
#include "Trajectory.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"

// Sets default values
AOrbit::AOrbit() :
PeriapsisRadius(1000.0),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0),
Inclination(0.0),
Eccentricity(0.0),
Trajectory(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);
	
	//Trajectory = CreateDefaultSubObject<ATrajectory>(TEXT("Trajectory")); // Create Trajectory SubObject
	
	// Initialize SpringArm
	// The SpringArm is used with the Trajectory to ensure that the Semi-major axis always passes through the
	// Central Body when the Trajectory is rotated, since the center of the Orbit is not necessarily at the same
	// position as the Central Body.
	/*SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);*/

	// Initialize Arrows
	/*SemiLatusRectumArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Semi Latus Rectum"));
	SemiLatusRectumArrow->SetupAttachment(SceneRoot);
	SemiLatusRectumArrow->SetArrowColor(FLinearColor::Green);
	SemiLatusRectumArrow->ArrowSize = 1.0;
	SemiLatusRectumArrow->bUseInEditorScaling = false;
	FRotator Rotation = FRotationMatrix::MakeFromX(FVector(0.0, -1.0, 0.0)).Rotator();
	SemiLatusRectumArrow->SetRelativeRotation(Rotation);
	SemiLatusRectumArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	SemiLatusRectumArrow->SetVisibility(true);
	SemiLatusRectumArrow->SetHiddenInGame(true);*/
	
	
	AscendingNodeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Ascending Node Arrow"));
	AscendingNodeArrow->SetupAttachment(SceneRoot);
	AscendingNodeArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	AscendingNodeArrow->SetArrowColor(FColor::Purple);
	AscendingNodeArrow->ArrowSize = 1.0;
	AscendingNodeArrow->bUseInEditorScaling = false;
	AscendingNodeArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	AscendingNodeArrow->SetVisibility(true);
	AscendingNodeArrow->SetHiddenInGame(true);
	
} // End of Constructor

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
	// If either body is invalid, destroy Orbit and return
	if ((!IsValid(CentralBody) || !IsValid(OrbitingBody))) { Destroy(); }
	
	/*else
	{
		/*InitializeOrbitingBody();
		UpdateOrbitalDistance();
		CalculateOrbit();#1#

		
		//InitializeTrajectory();
		//CalculateTrajectory();
		//DrawTrajectory();
	}*/

	/*if (IsValid(CentralBody))
		CalculatePeriod(SemiMajorAxis, CentralBody->GetMassOfBody());*/
}

void AOrbit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FlushPersistentDebugLines(GetWorld());

	// Clamp the Eccentricity within the range [0, 1)
	Eccentricity = FMath::Clamp(Eccentricity, 0.0, 1.0);
	
	// If either body is invalid, return
	if ((!IsValid(CentralBody) || !IsValid(OrbitingBody))) { return; }

	// Attach Orbit to the CentralBody
	AttachToActor(CentralBody,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
	SetActorRelativeLocation(FVector::ZeroVector); // Reset Position
	SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	
	CalculateOrbit(); // Calculate the Orbital Elements
	//CalculateOrbitFromEccentricity(); // Calculate the Orbital Elements

	if(IsValid(Trajectory))
	{
		//Trajectory->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		Trajectory->AttachToComponent(SceneRoot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		Trajectory->SetActorRelativeLocation(FVector::ZeroVector); // Reset Position
		Trajectory->SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
		//SemiLatusRectumArrow->AttachToComponent(Trajectory->GetRootComponent(),  FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false));

		// Attach OrbitingBody to the Trajectory
		OrbitingBody->AttachToActor(Trajectory,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		OrbitingBody->SetActorRelativeLocation(Trajectory->GetActorForwardVector() * PeriapsisRadius); // Position OrbitingBody at the Periapsis

		DrawTrajectory();
		OrientOrbit();
	}
	else
	{
		// Attach OrbitingBody to the Orbit
		OrbitingBody->AttachToActor(this,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		OrbitingBody->SetActorRelativeLocation(GetActorForwardVector() * PeriapsisRadius); // Position OrbitingBody at the Periapsis
	}	
	InitializeOrbitingBody();
	
	// Aim acceleration arrow
	OrbitingBody->AimAccelerationArrow(CentralBody);
	FlushPersistentDebugLines(GetWorld());
}


void AOrbit::UpdateOrbitingBody(const float DeltaTime)
{
	OrbitingBody->CalculateAcceleration(CentralBody);
	OrbitingBody->UpdateVelocity(DeltaTime);
	OrbitingBody->UpdatePosition(DeltaTime);

	UpdateOrbitalDistance();
}

void AOrbit::UpdateOrbitalDistance()
{
	OrbitalRadius = FVector::Distance(CentralBody->GetActorLocation(), OrbitingBody->GetActorLocation());
}

// Called every frame
void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If either body is invalid, destroy Orbit and return
	if (!IsValid(CentralBody) || !IsValid(OrbitingBody)) { Destroy(); return;}
	
	// Update SpotLight
	OrbitingBody->OrientSpotLight(CentralBody);
}



void AOrbit::InitializeOrbitingBody()
{
	if (!IsValid(CentralBody) || !IsValid(OrbitingBody)) {return;}
	
	// Orient Orbiting Body so its forward vector is pointing towards the Central Body
	OrientOrbitingBodyTowardsCenter();
	
	// Scalar multiply Initial Orbital Speed with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector, and Tangent to the ellipse of the orbit
	FVector Velocity = OrbitingBody->GetActorRightVector() * InitialOrbitalSpeed * Sim::KM_TO_M;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity

	// Initialize arrows
	OrbitingBody->UpdateAccelerationArrow();
	OrbitingBody->UpdateVelocityArrow();

	// Orient Spotlight
	OrbitingBody->OrientSpotLight(CentralBody);
}

void AOrbit::OrientOrbitingBodyTowardsCenter()
{
	// Get direction vector from Orbiting Body to Central Body
	//FVector Direction = (CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation());
	// Get Rotator from Direction
	//FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, 180));
	// Set Rotation of Orbiting Body so that its Forward Vector is pointing at Central Body
	OrbitingBody->SetActorRelativeRotation(Rotation);
	
	// Aim acceleration arrow
	OrbitingBody->AimAccelerationArrow(CentralBody);
}


void AOrbit::OrientPeriapsisVector()
{
	// Calculate the direction vector of the periapsis
	const FVector ReferenceAxis = AscendingNodeVector;
	// The Up vector of the Orbit object should be orthogonal to the Orbital Plane
	PeriapsisVector = ReferenceAxis.RotateAngleAxis(-ArgumentOfPeriapsis, GetActorUpVector()); // Rotate vector around the Up Vector
	PeriapsisVector.Normalize();
}

void AOrbit::CalculateOrbit()
{
	const double CentralMass = CentralBody->GetMassOfBody();
	SpecificOrbitalEnergy = SpecificOrbitalEnergy::CalculateSpecificOrbitalEnergy(InitialOrbitalSpeed, CentralMass, PeriapsisRadius);
	SemiMajorAxis = SemiMajorAxis::CalculateFromSpecificOrbitalEnergy(CentralMass, SpecificOrbitalEnergy);
	OrbitalPeriod = OrbitalPeriod::CalculatePeriod(SemiMajorAxis, CentralMass);
	ApoapsisRadius = Apoapsis::RadiusFromPeriapsis(SemiMajorAxis, PeriapsisRadius);
	LinearEccentricity = LinearEccentricity::CalculateFromApsis(SemiMajorAxis, ApoapsisRadius); 
	//SpringArm->TargetArmLength = LinearEccentricity; // Set arm length to be equal to the linear eccentricity
	//SpringArm->TargetArmLength = 0.0;
	Eccentricity = Eccentricity::CalculateFromLinearEccentricity(SemiMajorAxis, LinearEccentricity);
	SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	//SemiLatusRectumArrow->ArrowLength = SemiLatusRectum;
	SemiMinorAxis = SemiMinorAxis::CalculateFromSemiLatusRectum(SemiMajorAxis, SemiLatusRectum);
	
}

void AOrbit::CalculateOrbitFromEccentricity()
{
	const double CentralMass = CentralBody->GetMassOfBody();

	// Calculate the Semi-Major Axis from the radius at Periapsis and the Eccentricity
	SemiMajorAxis = SemiMajorAxis::CalculateFromPeriapsis(Eccentricity, PeriapsisRadius);
	// Calculate the Semi-Latus Rectum
	SemiLatusRectum = SemiLatusRectum::CalculateSemiLatusRectum(SemiMajorAxis, Eccentricity);
	// Calculate the Semi-Minor Axis
	SemiMinorAxis = SemiMinorAxis::CalculateFromSemiLatusRectum(SemiMajorAxis, SemiLatusRectum);
	
	OrbitalPeriod = OrbitalPeriod::CalculatePeriod(SemiMajorAxis, CentralMass);
	ApoapsisRadius = Apoapsis::RadiusFromPeriapsis(SemiMajorAxis, PeriapsisRadius);
	LinearEccentricity = LinearEccentricity::CalculateFromEccentricity(SemiMajorAxis, Eccentricity);

	// Set Orbital Speed at Periapsis
	InitialOrbitalSpeed = OrbitalVelocity::CalculateAtPeriapsis(Eccentricity, SemiMajorAxis, CentralMass);
	
	//SpringArm->TargetArmLength = LinearEccentricity; // Set arm length to be equal to the linear eccentricity
	//SpringArm->TargetArmLength = 0.0;
	
	//SemiLatusRectumArrow->ArrowLength = SemiLatusRectum;

	
}

void AOrbit::OrientOrbit()
{
	if(!IsValid(Trajectory)) return;
	
	SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	Trajectory->SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);  // Reset Rotation
	
	OrientAscendingNode();
	OrientInclination();
	OrientArgumentOfPeriapsis();

}

void AOrbit::OrientAscendingNode()
{
	// The Ascending Node lies in the reference plane
	
	// Get Forward Vector of Central Body (X-Axis)
	const FVector ReferenceAxis = CentralBody->GetActorForwardVector(); 
	// Rotate Vector by the Longitude of the Ascending Node around the Z-Axis
	AscendingNodeVector = ReferenceAxis.RotateAngleAxis(-LongitudeOfAscendingNode, CentralBody->GetActorUpVector()); 
	
	//FRotator Rotation = FRotationMatrix::MakeFromX(RotatedVector).Rotator(); // Get Rotator from Vector
	//FRotator Rotation = FRotator::MakeFromEuler(RotatedVector);
	//Trajectory->SetActorRotation(Rotation);

	/*Rotation = FRotationMatrix::MakeFromX(AscendingNodeVector).Rotator();
	Trajectory->SetAscendingNodeArrowRotation(Rotation);*/

	// Rotate the Orbit around its local Z-Axis
	//FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, -LongitudeOfAscendingNode));
	//AddActorLocalRotation(Rotation);
	//SetActorRelativeRotation(Rotation);
	
	//FRotator Rotation = FRotationMatrix::MakeFromX(AscendingNodeVector).Rotator(); // Get Rotator from Vector
	//FRotator Rotation = FRotator::MakeFromEuler(AscendingNodeVector);
	//Trajectory->SetActorRotation(Rotation);

	
	const double Distance = OrbitalRadius::CalculateRadiusAtTrueAnomaly(ArgumentOfPeriapsis, Eccentricity, SemiLatusRectum);
	Trajectory->PositionAscendingNodeMarker(ArgumentOfPeriapsis, Distance);
	
	// Orient Arrow
	AscendingNodeArrow->SetRelativeRotation(FRotator::ZeroRotator);
	AscendingNodeArrow->ArrowLength = Distance;
}

void AOrbit::OrientArgumentOfPeriapsis()
{
	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, -ArgumentOfPeriapsis));
	Trajectory->SetActorRelativeRotation(Rotation);
}

void AOrbit::OrientInclination()
{
	// Rotate Trajectory using the Ascending Node Vector as the axis to rotate around
	//FVector ReferenceAxis = AscendingNodeVector;
	//FVector Forward = GetActorForwardVector();
	//FVector RotatedVector = Forward.RotateAngleAxis(Inclination, ReferenceAxis);

	//FRotator Rotation = FRotator::MakeFromEuler(RotatedVector); // Get Rotator from Vector
	//FRotator Rotation = FRotationMatrix::MakeFromX(RotatedVector).Rotator(); // Get Rotator from Vector

	//FRotator OrientAscendingNode = FRotationMatrix::MakeFromX(AscendingNodeVector).Rotator();
	
	//Trajectory->AddActorLocalRotation(Rotation);


	/*FVector Forward = FVector::XAxisVector;
	FVector Up = FVector::YAxisVector;
	FVector RotatedVector = Up.RotateAngleAxis(Inclination, Forward);*/
	//FRotator Rotation = FRotator::MakeFromEuler(FVector(Inclination, 0.0, -LongitudeOfAscendingNode)); // Get Rotator from Vector
	//SpringArm->SetWorldRotation(Rotation);

	// The inclination is rotated around the axis of the Ascending Node, so long as the Orbit's forward vector is
	// pointing towards the Ascending Node, we can simply rotate the orbit around its local X-axis
	FRotator Rotation = FRotator::MakeFromEuler(FVector(Inclination, 0.0, -LongitudeOfAscendingNode)); // Get Rotator from Vector
	//AddActorLocalRotation(Rotation);
	SetActorRelativeRotation(Rotation);

	//SpringArm->SetRelativeRotation(FRotator::ZeroRotator); // Set SpringArm rotation to keep it consistent with the Orbit object
	//Trajectory->SetActorRelativeRotation(FRotator::ZeroRotator); // Set Trajectory rotation to keep it consistent with the Orbit object
}

void AOrbit::DrawTrajectory()
{
	if (!Trajectory  || !IsValid(CentralBody) || !IsValid(OrbitingBody))  { return; }
	Trajectory->SetClosedLoop(true);
	
	if (Eccentricity >= 1.0)
	{
		// If Eccentricity is greater than one, trajectory is either parabolic or hyperbolic
		Trajectory->SetClosedLoop(false);
	}
	else
	{
		Trajectory->SetClosedLoop(true);
	}

	// Set axes of Trajectory object
	//Trajectory->SetActorLocation(EllipticalCenter);
	Trajectory->SetAxes(SemiMajorAxis, SemiMinorAxis);

	// Draw Trajectory
	Trajectory->Draw();

}

void AOrbit::PostInitProperties()
{
	Super::PostInitProperties();
}


#if WITH_EDITOR
void AOrbit::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(!PropertyChangedEvent.Property) return;
	
	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	
	/*if(PropertyName == TEXT("Eccentricity"))
	{
		CalculateOrbitFromEccentricity();
	}*/
}
#endif