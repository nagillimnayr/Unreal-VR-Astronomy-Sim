// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"

#include "AstroBody.h"
#include "System.h"
#include "Trajectory.h"
#include "Components/ArrowComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Orbit_Simulation/CalculateOrbitalElements/Motion.h"

// Sets default values
AOrbit::AOrbit() :
PeriapsisRadius(1000.0),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0),
Inclination(0.0),
Eccentricity(0.0),
TrueAnomaly(0.0),
EccentricAnomaly(0.0),
MeanAnomaly(0.0),
MeanAngularMotion(0.0),
Trajectory(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	// Create Trajectory
	//Trajectory = CreateDefaultSubobject<ATrajectory>(FName(GetFName().ToString() + FString("_Trajectory")));
	
	// Initialize Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera->SetupAttachment(CameraBoom);
	
	// Initialize Spotlight
	Spotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	SpotlightBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spotlight Boom"));
	Spotlight->SetupAttachment(SpotlightBoom);

	// Initialize Arrows
	AscendingNodeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Ascending Node Arrow"));
	AscendingNodeArrow->SetupAttachment(SceneRoot);
	AscendingNodeArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	AscendingNodeArrow->SetArrowColor(FColor::Purple);
	AscendingNodeArrow->ArrowSize = 1.0;
	AscendingNodeArrow->bUseInEditorScaling = false;
	AscendingNodeArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	AscendingNodeArrow->SetVisibility(true);
	AscendingNodeArrow->SetHiddenInGame(true);
	
	TrueAnomalyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("True Anomaly Arrow"));
	TrueAnomalyArrow->SetupAttachment(SceneRoot);
	TrueAnomalyArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	TrueAnomalyArrow->SetArrowColor(FColor::White);
	TrueAnomalyArrow->ArrowSize = 1.0;
	TrueAnomalyArrow->bUseInEditorScaling = false;
	TrueAnomalyArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	TrueAnomalyArrow->SetVisibility(true);
	TrueAnomalyArrow->SetHiddenInGame(true);
	
	MeanAnomalyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Mean Anomaly Arrow"));
	MeanAnomalyArrow->SetupAttachment(SceneRoot);
	MeanAnomalyArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	MeanAnomalyArrow->SetArrowColor(FColor::Emerald);
	MeanAnomalyArrow->ArrowSize = 1.0;
	MeanAnomalyArrow->bUseInEditorScaling = false;
	MeanAnomalyArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	MeanAnomalyArrow->SetVisibility(true);
	MeanAnomalyArrow->SetHiddenInGame(true);
	
} // End of Constructor

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
	// If either body is invalid, destroy Orbit and return
	if ((!IsValid(CentralBody) || !IsValid(OrbitingBody))) { Destroy(); }
	
}

void AOrbit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//FlushPersistentDebugLines(GetWorld());

	// Clamp the Eccentricity within the range [0, 1)
	Eccentricity = FMath::Clamp(Eccentricity, 0.0, 1.0);
	
	// If either body is invalid, return
	if ((!IsValid(CentralBody) || !IsValid(OrbitingBody))) { return; }

	// Attach Orbit to the CentralBody
	/*AttachToActor(CentralBody,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		*/
	SetActorRelativeLocation(FVector::ZeroVector); // Reset Position
	SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	
	CalculateOrbit(); // Calculate the Orbital Elements
	//CalculateOrbitFromEccentricity(); // Calculate the Orbital Elements

	if(IsValid(Trajectory))
	{
		//Trajectory->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		Trajectory->AttachToActor(this,
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));
		Trajectory->SetActorRelativeLocation(FVector::ZeroVector); // Reset Position
		Trajectory->SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
		//SemiLatusRectumArrow->AttachToComponent(Trajectory->GetRootComponent(),  FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false));

		// Attach OrbitingBody to the Trajectory
		OrbitingBody->AttachToActor(Trajectory,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		OrbitingBody->SetActorRelativeLocation(Trajectory->GetActorForwardVector() * PeriapsisRadius); // Position OrbitingBody at the Periapsis

		// Attach Anomaly Arrows to Trajectory
		TrueAnomalyArrow->AttachToComponent(Trajectory->GetRootComponent(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		MeanAnomalyArrow->AttachToComponent(Trajectory->GetRootComponent(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		/*// position EccentricAnomalyArrow at the center of the ellipse
		EccentricAnomalyArrow->SetRelativeLocation(FVector(-LinearEccentricity, 0.0, 0.0));*/
		
		
		DrawTrajectory();
		OrientOrbit();

		// Setup Spotlight
		Spotlight->AttenuationRadius = 400.0 * OrbitingBody->GetSize();
		SpotlightBoom->TargetArmLength = 400.0 * OrbitingBody->GetSize();
		Spotlight->SetCastShadows(false);
		Spotlight->SetCastDeepShadow(false);
		Spotlight->SetCastRaytracedShadow(false);
		Spotlight->SetCastVolumetricShadow(false);

		// Set Colors
		if(Trajectory) Trajectory->SetColor(Color);
		OrbitingBody->SetColor(Color);
	}
	else
	{
		// Attach OrbitingBody to the Orbit
		OrbitingBody->AttachToActor(this,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
		OrbitingBody->SetActorRelativeLocation(GetActorForwardVector() * PeriapsisRadius); // Position OrbitingBody at the Periapsis
	}

	// Attach Camera to Orbiting Body
	CameraBoom->AttachToComponent(OrbitingBody->GetRootComponent(), 
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));
	CameraBoom->TargetArmLength = 400.0 * OrbitingBody->GetSize();
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 0.0)));

	// Attach Spotlight to OrbitingBody
	SpotlightBoom->AttachToComponent(OrbitingBody->GetRootComponent(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false));
	SpotlightBoom->TargetArmLength = 400.0 * OrbitingBody->GetSize();
	OrientSpotlight();
	
	InitializeOrbitingBody();
	
	// Aim acceleration arrow
	OrbitingBody->AimAccelerationArrow(CentralBody);
}


void AOrbit::UpdateOrbitingBody(const float DeltaTime)
{
	// If either body is invalid, destroy Orbit and return
	if (!IsValid(CentralBody) || !IsValid(OrbitingBody)) { Destroy(); return;}

	// Update Orbiting Body
	//OrbitingBody->CalculateAcceleration(CentralBody);
	const FVector Acceleration = Motion::CalculateAcceleration(OrbitingBody, CentralBody);
	const FVector Velocity = Acceleration * DeltaTime;
	
	OrbitingBody->SetAcceleration(Acceleration);
	OrbitingBody->UpdateVelocity(Velocity);
	OrbitingBody->UpdatePosition(DeltaTime);

	OrbitalSpeed = Acceleration.Length();
	UpdateOrbitalDistance();
	
	// Orient Spotlight
	OrientSpotlight();
	
	// Rotate TrueAnomalyArrow
	MeanAnomaly += MeanAngularMotion * DeltaTime;
	SetTrueAnomalyArrow();
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

	
}



void AOrbit::InitializeOrbitingBody()
{
	if (!IsValid(CentralBody) || !IsValid(OrbitingBody)) {return;}
	
	// Orient Orbiting Body so its forward vector is pointing towards the Central Body
	OrientOrbitingBodyTowardsCenter();
	
	// Scalar multiply Initial Orbital Speed with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector, and Tangent to the ellipse of the orbit
	FVector Velocity = OrbitingBody->GetActorRightVector() * MaxOrbitalSpeed * Unit::KM_TO_M;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity

	// Initialize arrows
	OrbitingBody->UpdateAccelerationArrow();
	OrbitingBody->UpdateVelocityArrow();

	// Orient Spotlight
	OrientSpotlight();
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

void AOrbit::CalculateOrbit()
{
	const double CentralMass = CentralBody->GetMassOfBody();
	SpecificOrbitalEnergy = SpecificOrbitalEnergy::CalculateSpecificOrbitalEnergy(MaxOrbitalSpeed, CentralMass, PeriapsisRadius);
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


	EccentricAnomaly = EccentricAnomaly::CalculateFromTrueAnomaly(TrueAnomaly, Eccentricity);
	SetTrueAnomalyArrow();
	MeanAngularMotion = (360.0 / OrbitalPeriod);
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
	MaxOrbitalSpeed = OrbitalVelocity::CalculateAtPeriapsis(Eccentricity, SemiMajorAxis, CentralMass);
	
	
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
	const double Distance = OrbitalRadius::CalculateAtTrueAnomaly(FMath::DegreesToRadians(ArgumentOfPeriapsis), SemiMajorAxis, Eccentricity);
	AscendingNodeArrow->ArrowLength = Distance;
	
	// Orient Arrow
	AscendingNodeArrow->SetRelativeRotation(FRotator::ZeroRotator);
}

void AOrbit::OrientArgumentOfPeriapsis()
{
	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, -ArgumentOfPeriapsis));
	Trajectory->SetActorRelativeRotation(Rotation);
}

void AOrbit::OrientInclination()
{
	// The inclination is rotated around the axis of the Ascending Node, so long as the Orbit's forward vector is
	// pointing towards the Ascending Node, we can simply rotate the orbit around its local X-axis
	FRotator Rotation = FRotator::MakeFromEuler(FVector(Inclination, 0.0, -LongitudeOfAscendingNode)); // Get Rotator from Vecto
	SetActorRelativeRotation(Rotation);
}

void AOrbit::SetTrueAnomalyArrow()
{
	// Rotate TrueAnomalyArrow
	const FRotator Rotation = FRotator::MakeFromEuler(FVector::ZAxisVector * -TrueAnomaly);
	TrueAnomalyArrow->SetRelativeRotation(Rotation);
	TrueAnomalyArrow->ArrowLength = OrbitalRadius::CalculateAtTrueAnomaly(FMath::DegreesToRadians(TrueAnomaly), SemiMajorAxis, Eccentricity);
	TrueAnomalyArrow->SetVisibility(false);
	TrueAnomalyArrow->SetVisibility(true);
}

void AOrbit::SetMeanAnomalyArrow()
{
	// Rotate MeanAnomalyArrow
	const FRotator Rotation = FRotator::MakeFromEuler(FVector::ZAxisVector * -MeanAnomaly);
	MeanAnomalyArrow->SetRelativeRotation(Rotation);
	MeanAnomalyArrow->ArrowLength = OrbitalRadius::CalculateAtTrueAnomaly(FMath::DegreesToRadians(MeanAnomaly), SemiMajorAxis, Eccentricity);
	MeanAnomalyArrow->SetVisibility(false);
	MeanAnomalyArrow->SetVisibility(true);
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
	Trajectory->SetAxes(SemiMajorAxis, SemiMinorAxis);

	// Draw Trajectory
	Trajectory->Draw();
}

void AOrbit::OrientSpotlight()
{
	const FVector Direction = CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation(); // Get Direction Vector to CentralBody
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator(); // Get Rotator from Direction Vector
	SpotlightBoom->SetWorldRotation(Rotation); // Set new rotation
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
	if(PropertyName == TEXT("TrueAnomaly"))
	{
		// Adjust angle to be within range [0, 360) degrees
		TrueAnomaly = AdjustAngle(TrueAnomaly);
		SetTrueAnomalyArrow();
	}
	else if(PropertyName == TEXT("ArgumentOfPeriapsis"))
	{
		// Adjust angle to be within range [0, 360) degrees
		ArgumentOfPeriapsis = AdjustAngle(ArgumentOfPeriapsis);
	}
	else if(PropertyName == TEXT("Inclination"))
	{
		// Adjust angle to be within range [0, 360) degrees
		Inclination = AdjustAngle(Inclination);
	}
	else if(PropertyName == TEXT("LongitudeOfAscendingNode"))
	{
		// Adjust angle to be within range [0, 360) degrees
		LongitudeOfAscendingNode = AdjustAngle(LongitudeOfAscendingNode);
	}
}
#endif