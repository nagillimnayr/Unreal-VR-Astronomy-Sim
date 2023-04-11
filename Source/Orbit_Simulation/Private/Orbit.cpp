// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"

#include "AstroBody.h"
#include "EllipseMeshComponent.h"
#include "JsonParser.h"
//#include "Trajectory.h"
#include "Components/ArrowComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "JsonUtils/JsonPointer.h"
#include "Orbit_Simulation/CalculateOrbitalElements/Motion.h"
#include "Dom/JsonObject.h"
#include "OrbitalPathComponent.h"
#include "Selection.h"
#include "SimPlayerController.h"

// Sets default values
AOrbit::AOrbit() :
PeriapsisRadius(1000.0),
SemiMajorAxis(1000.0),
SemiMinorAxis(1000.0),
Eccentricity(0.0),
Inclination(0.0),
TrueAnomaly(0.0),
EccentricAnomaly(0.0),
MeanAnomaly(0.0),
MeanAngularMotion(0.0),
Color(FLinearColor::White)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	
	// Initialize Camera
	/*Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera->SetupAttachment(CameraBoom);*/
	
	// Create Orbital Plane Component
	OrbitalPlane = CreateDefaultSubobject<UEllipseMeshComponent>(TEXT("Orbital Plane"));
	OrbitalPlane->SetupAttachment(SceneRoot);
	OrbitalPlane->SetVisibility(false);
	
	/*#if WITH_EDITORONLY_DATA
	USelection::SelectObjectEvent.AddUObject(this, &AOrbit::OnObjectSelected);
	#endif*/

	// Create Orbital Path Component
	OrbitalPath = CreateDefaultSubobject<UOrbitalPathComponent>(TEXT("Orbital Path"));
	OrbitalPath->SetupAttachment(SceneRoot);

	// Initialize Arrows
	AscendingNodeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Ascending Node Arrow"));
	AscendingNodeArrow->SetupAttachment(SceneRoot);
	AscendingNodeArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	AscendingNodeArrow->SetArrowColor(FColor::Purple);
	AscendingNodeArrow->ArrowSize = 1.0;
	AscendingNodeArrow->bUseInEditorScaling = false;
	AscendingNodeArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	AscendingNodeArrow->SetVisibility(false);
	AscendingNodeArrow->SetHiddenInGame(true);
	
	TrueAnomalyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("True Anomaly Arrow"));
	TrueAnomalyArrow->SetupAttachment(SceneRoot);
	TrueAnomalyArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	TrueAnomalyArrow->SetArrowColor(FColor::White);
	TrueAnomalyArrow->ArrowSize = 1.0;
	TrueAnomalyArrow->bUseInEditorScaling = false;
	TrueAnomalyArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	TrueAnomalyArrow->SetVisibility(false);
	TrueAnomalyArrow->SetHiddenInGame(true);
	
	MeanAnomalyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Mean Anomaly Arrow"));
	MeanAnomalyArrow->SetupAttachment(SceneRoot);
	MeanAnomalyArrow->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	MeanAnomalyArrow->SetArrowColor(FColor::Emerald);
	MeanAnomalyArrow->ArrowSize = 1.0;
	MeanAnomalyArrow->bUseInEditorScaling = false;
	MeanAnomalyArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	MeanAnomalyArrow->SetVisibility(false);
	MeanAnomalyArrow->SetHiddenInGame(true);

	// Json File Reader
	JsonParser = CreateDefaultSubobject<UJsonParser>(FName("Json File Reader"));
	
} // End of Constructor

void AOrbit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Initialize();
}

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
	// If either body is invalid, destroy Orbit and return
	if ((!IsValid(CentralBody) || !IsValid(OrbitingBody))) { Destroy(); }
	InitializeOrbitingBody();
	
}


void AOrbit::Initialize()
{
	// Set Visibility of the Orbital Path
	OrbitalPath->SetMeshVisibility(false);

	// Set Orbiting Body's reference to Orbit
	if(IsValid(OrbitingBody))
	{
		OrbitingBody->SetOrbit(this);
	}
	
	// Load preset
	if(Preset != None) { LoadPreset(); }
	
	// Clamp the Eccentricity within the range [0, 1)
	Eccentricity = FMath::Clamp(Eccentricity, 0.0, 1.0);
	
	// Assertion
	check(IsValid(OrbitalPath));

	// Attach Anomaly Arrows to OrbitalPath so that they will continue to be pointing at
	// The periapsis after OrbitalPath is rotated
	TrueAnomalyArrow->AttachToComponent(OrbitalPath,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));
	MeanAnomalyArrow->AttachToComponent(OrbitalPath,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false));

	
	
	// Calculate the orbit
	if(!IsValid(CentralBody)) return;
	SetActorRelativeLocation(FVector::ZeroVector);
	if(OrbitingBody) OrbitingBody->SetColor(Color);
	CalculateOrbit(); // Calculate the Orbital Elements
	OrbitalPath->DrawPath(SemiMajorAxis, SemiMinorAxis);
	
	// Set Color
	OrbitalPath->SetColor(Color);
	
	// Create Orbital Plane
	OrbitalPlane->CreateMesh(64, SemiMajorAxis, SemiMinorAxis, Color);
	OrbitalPlane->AttachToComponent(OrbitalPath, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		false
	));
	
	
	OrientOrbit();

	if(!OrbitingBody) return;
	InitializeOrbitingBody();
}

void AOrbit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Initialize();
}

void AOrbit::PostLoad()
{
	Super::PostLoad();
	Initialize();
}


void AOrbit::UpdateOrbitingBody(const float DeltaTime)
{
	// If either body is invalid, destroy Orbit and return
	if (!IsValid(CentralBody) || !IsValid(OrbitingBody)) { Destroy(); return;}

	// Update Orbiting Body
	//OrbitingBody->CalculateAcceleration(CentralBody);
	const FVector Acceleration = Motion::CalculateAcceleration(OrbitingBody, CentralBody);
	const FVector NewVelocity = OrbitingBody->GetVelocityVector() + (Acceleration * (DeltaTime * Unit::SECONDS_IN_DAY));
	
	OrbitingBody->SetAcceleration(Acceleration);
	OrbitingBody->UpdateVelocity(NewVelocity);
	OrbitingBody->UpdatePosition(DeltaTime);

	OrbitalSpeed = Acceleration.Length();
	UpdateOrbitalDistance();
	
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

	OrbitingBody->Initialize();
	// Attach OrbitingBody to the OrbitalPath
	OrbitingBody->AttachToComponent(OrbitalPath,
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false));
	
	// Position OrbitingBody at the Periapsis
	OrbitingBody->SetActorRelativeLocation(FVector::XAxisVector * PeriapsisRadius); 
	
	// Orient Orbiting Body so its forward vector is pointing towards the Central Body
	OrbitingBody->SetActorRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, 0.0, 180)));
	
	// Scalar multiply Initial Orbital Speed with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector, and Tangent to the ellipse of the orbit
	FVector Velocity = OrbitingBody->GetActorRightVector() * MaxOrbitalSpeed * Unit::KM_TO_M;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity

	// Initialize arrows
	OrbitingBody->AimAccelerationArrow(CentralBody);
	OrbitingBody->UpdateAccelerationArrow();
	OrbitingBody->UpdateVelocityArrow();

}

void AOrbit::OrientOrbitingBodyTowardsCenter()
{
	// Get direction vector from Orbiting Body to Central Body
	//FVector Direction = (CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation());
	// Get Rotator from Direction
	//FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	const FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, 180));
	// Set Rotation of Orbiting Body so that its Forward Vector is pointing at Central Body
	OrbitingBody->SetActorRelativeRotation(Rotation);
	
	// Aim acceleration arrow
	OrbitingBody->AimAccelerationArrow(CentralBody);
}

void AOrbit::LoadPreset()
{
	if(!IsValid(JsonParser)) return;

	const FString ObjectName = PresetNames.Contains(Preset) ? PresetNames[Preset] : "None";
	if(ObjectName == "None" || ObjectName == "")
	{
		GLog->Log(FString("Error: No Preset found"));
		return;
	}
	GLog->Log("> Loading Preset: " + ObjectName);
	
	TSharedPtr<FJsonObject> JsonObject = JsonParser->DeserializeFile(FString("Planets"), ObjectName);
	
	// Check that pointer is valid
	if(!JsonObject.IsValid())
	{
		GLog->Log(FString("Error: No Preset found"));
		return;
	}

	// Load data from Preset
	
	// Get Color as string
	FString FieldName = "Color";
	const FString ColorHexString = JsonObject->HasField(FieldName) ? JsonObject->GetStringField(FieldName) : "FFFFFF";
	GLog->Log("Color: " + ColorHexString);
	try
	{
		Color = FColor::FromHex(ColorHexString);
	}
	catch(...)
	{
		GLog->Log("> Error: Could not load color: " + ColorHexString);
		Color = FLinearColor::White;
	}

	// Orbital Elements
	FieldName = "SemiMajorAxis_M";
	SemiMajorAxis = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) / Unit::DISTANCE_MULT : 100.0;
	
	FieldName = "Periapsis_M";
	PeriapsisRadius = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) / Unit::DISTANCE_MULT : 1000.0;
	
	FieldName = "Eccentricity";
	Eccentricity = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;
	
	FieldName = "MaxVelocity_KMs";
	MaxOrbitalSpeed = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;

	// Angular Parameters
	FieldName = "Inclination_Deg";
	Inclination = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;
	FieldName = "LongitudeOfPeriapsis_Deg";
	LongitudeOfPeriapsis = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;
	FieldName = "LongitudeOfAscendingNode_Deg";
	LongitudeOfAscendingNode = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;
	ArgumentOfPeriapsis = LongitudeOfPeriapsis - LongitudeOfAscendingNode;

	FieldName = "AxialTilt_Deg";
	AxialTilt = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 0.0;

	// Physical Parameters
	if(!IsValid(OrbitingBody)) return;
	FieldName = "Mass_KG";
	const double Mass = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) / Unit::SOLAR_MASS : 0.0;
	OrbitingBody->SetMass(Mass);
	
	FieldName = "MeanRadius_M"; // Get Mean Radius as ratio to Earth's
	//const double MeanRadius = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) / 6371.0084e3 : 1.0;
	const double MeanRadius = JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 1.0;
	//const double MeanRadius = (JsonObject->HasField(FieldName) ? JsonObject->GetNumberField(FieldName) : 100000.0);
	
	OrbitingBody->SetMeanRadius(MeanRadius);
	/*const double MeanRadiusEditorUnits = MeanRadius / (Unit::DISTANCE_MULT * 100.0);
	AscendingNodeArrow->SetWorldScale3D(FVector(1.0, MeanRadiusEditorUnits, MeanRadiusEditorUnits));
	TrueAnomalyArrow->SetWorldScale3D(FVector(1.0, MeanRadiusEditorUnits, MeanRadiusEditorUnits));
	MeanAnomalyArrow->SetWorldScale3D(FVector(1.0, MeanRadiusEditorUnits, MeanRadiusEditorUnits));*/

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
	if(!IsValid(OrbitalPath)) return;
	
	SetActorRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	OrbitalPath->SetRelativeRotation(FRotator::ZeroRotator); // Reset Rotation
	
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
	const FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 0.0, -ArgumentOfPeriapsis));
	OrbitalPath->SetRelativeRotation(Rotation);
}

void AOrbit::OrientInclination()
{
	// The inclination is rotated around the axis of the Ascending Node, so long as the Orbit's forward vector is
	// pointing towards the Ascending Node, we can simply rotate the orbit around its local X-axis
	const FRotator Rotation = FRotator::MakeFromEuler(FVector(Inclination, 0.0, -LongitudeOfAscendingNode)); // Get Rotator from Vector
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
	//if (!IsValid(CentralBody) || !IsValid(OrbitingBody))  { return; }
	
	if (Eccentricity >= 1.0)
	{
		// If Eccentricity is greater than one, trajectory is either parabolic or hyperbolic
		OrbitalPath->SetClosedLoop(false);
	}
	else
	{
		OrbitalPath->SetClosedLoop(true);
	}

	// Set axes of Trajectory object
	//Trajectory->SetAxes(SemiMajorAxis, SemiMinorAxis);

	// Draw Trajectory
	OrbitalPath->DrawPath(SemiMajorAxis, SemiMinorAxis);
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
	
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	
	/*if(PropertyName == TEXT("Eccentricity"))
	{
		CalculateOrbitFromEccentricity();
	}*/
	if(PropertyName == FName("TrueAnomaly"))
	{
		// Adjust angle to be within range [0, 360) degrees
		TrueAnomaly = AdjustAngle(TrueAnomaly);
		SetTrueAnomalyArrow();
	}
	else if(PropertyName == FName("ArgumentOfPeriapsis"))
	{
		// Adjust angle to be within range [0, 360) degrees
		ArgumentOfPeriapsis = AdjustAngle(ArgumentOfPeriapsis);
	}
	else if(PropertyName == FName("Inclination"))
	{
		// Adjust angle to be within range [0, 360) degrees
		Inclination = AdjustAngle(Inclination);
	}
	else if(PropertyName == FName("LongitudeOfAscendingNode"))
	{
		// Adjust angle to be within range [0, 360) degrees
		LongitudeOfAscendingNode = AdjustAngle(LongitudeOfAscendingNode);
	}
	else if(PropertyName == FName("Color"))
	{
		if(IsValid(OrbitalPath)) OrbitalPath->SetColor(Color);
		if(IsValid(OrbitingBody)) OrbitingBody->SetColor(Color);
	}
}

#endif

/*#if WITH_EDITORONLY_DATA
void AOrbit::OnObjectSelected(UObject* Object)
{
	
	if (Object == this)
	{
		SelectedInEditor = true;
		OrbitalPlane->SetVisibility(true);
	}
	else if (SelectedInEditor && !IsSelected())
	{
		SelectedInEditor = false;
		OrbitalPlane->SetVisibility(false);
	}
}
#endif*/