// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"
#include "AstroBody.h"
#include "Sim.h"
#include "Trajectory.h"

// Sets default values
AOrbit::AOrbit() :
CentralBody(nullptr),
OrbitingBody(nullptr),
PeriapsisRadius(1000),
Eccentricity(0),
SemimajorAxis(100),
Inclination(0),
SemiminorAxis(100),
Trajectory(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Trajectory = CreateDefaultSubobject<ATrajectory>(TEXT("Trajectory")); // Create Trajectory SubObject
	
}

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
	// If either body is invalid, destroy Orbit and return
	if (!CentralBody || !OrbitingBody) { Destroy(); }
	/*else
	{
		/*InitializeOrbitingBody();
		UpdateOrbitalDistance();
		CalculateOrbit();#1#

		
		//InitializeTrajectory();
		//CalculateTrajectory();
		//DrawTrajectory();
	}*/

}

void AOrbit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	

	/*if(Tags[0].Compare(FName(TEXT("Mercury"))))
	{
		PeriapsisRadius = Mercury::PeriapsisRadius;
		InitialOrbitalSpeed = Mercury::MaxOrbitalVelocity;
		
	}
	else if(Tags[0].Compare(FName(TEXT("Venus"))))
	{
		PeriapsisRadius = Venus::PeriapsisRadius;
		InitialOrbitalSpeed = Venus::MaxOrbitalVelocity;
	}
	else if(Tags[0].Compare(FName(TEXT("Earth"))))
	{
		PeriapsisRadius = 1470.95;
		InitialOrbitalSpeed = 30.29;
	}
	else if(Tags[0].Compare(FName(TEXT("Mars"))))
	{
		PeriapsisRadius = 2066.50;
		InitialOrbitalSpeed = 26.50;
	}*/
	
	
	/*else if(Tags[0].Compare(FName(TEXT("Saturn"))))
	{
		PeriapsisRadius = Saturn::PeriapsisRadius;
		InitialOrbitalSpeed = Saturn::MinOrbitalVelocity;
	}
	else if(Tags[0].Compare(FName(TEXT("Jupiter"))))
	{
		PeriapsisRadius = Jupiter::PeriapsisRadius;
		InitialOrbitalSpeed = Jupiter::MinOrbitalVelocity;
	}
	else if(Tags[0].Compare(FName(TEXT("Uranus"))))
	{
		PeriapsisRadius = Uranus::PeriapsisRadius;
		InitialOrbitalSpeed = Uranus::MinOrbitalVelocity;
	}
	else if(Tags[0].Compare(FName(TEXT("Neptune"))))
	{
		PeriapsisRadius = Neptune::PeriapsisRadius;
		InitialOrbitalSpeed = Neptune::MinOrbitalVelocity;
	}*/
	
	// If either body is invalid, return
	if (!CentralBody || !OrbitingBody) { return; }

	OrientOrbit();
	InitializeOrbitingBody();
	UpdateOrbitalDistance();
	//PeriapsisVector = OrbitingBody->GetActorLocation();
	CalculateOrbit();
	
	DrawTrajectory();

	
	// Orient Orbiting Body so its forward vector is pointing towards the Central Body
	OrientOrbitingBodyTowardsCenter();
	
	// Scalar multiply Initial Orbital Speed with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector
	FVector Velocity = OrbitingBody->GetActorRightVector() * InitialOrbitalSpeed * ASim::KM_TO_M;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity
}


void AOrbit::UpdateOrbit(const float DeltaTime)
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
	if (!CentralBody || !OrbitingBody) { Destroy(); return;}

	// Because increasing the Time Dilation just multiplies DeltaTime, which increase the Time Step and
	// results in a less accurate simulation, we can instead call the update function multiple times per tick
	// to emulate Unity's FixedUpdate();
	//Update(DeltaTime);
}


void AOrbit::PostInitProperties()
{
	Super::PostInitProperties();

}

void AOrbit::InitializeOrbitingBody()
{
	if (!CentralBody || ! OrbitingBody) {return;}

	// Set Orbiting Body's reference to Orbit
	OrbitingBody->Orbit = this;

	// Position body at Periapsis
	OrbitingBody->SetActorLocation(PeriapsisVector);
	
	// Orient Orbiting Body so its forward vector is pointing towards the Central Body
	OrientOrbitingBodyTowardsCenter();
	
	// Scalar multiply Initial Orbital Speed with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector
	FVector Velocity = OrbitingBody->GetActorRightVector() * InitialOrbitalSpeed * ASim::KM_TO_M;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity

	// Initialize arrows
	OrbitingBody->UpdateAccelerationArrow();
	OrbitingBody->UpdateVelocityArrow();
}

void AOrbit::OrientOrbitingBodyTowardsCenter()
{
	// Get direction vector from Orbiting Body to Central Body
	FVector Direction = (CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation());
	// Get Rotator from Direction
	FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	// Set Rotation of Orbiting Body so that its Forward Vector is pointing at Central Body
	OrbitingBody->SetActorRotation(Rotation);
}


// Compute Keplerian Elements from Orbital State Vectors

void AOrbit::CalculateOrbit()
{
	CalculateSpecifcOrbitalEnergy();
	CalculateSemiMajorAxisFromSpecifcOrbitalEnergy();
	CalculateApoapsisRadius(); // Also elliptical center and Apoapsis Vector
	CalculateLinearEccentricity(); // Also eccentricity, semi-latus rectum, and semi-minor axis.
	//CalculateEccentricityFromApsides();
	//CalculatePeriodFromSemiMajorAxis();
	
}

void AOrbit::OrientOrbit()
{
	// Get Forward Vector of Central Body (X-Axis)
	FVector ReferenceAxis = CentralBody->GetActorForwardVector(); 
	// Rotate Vector by the Argument of Periapsis around the Z-Axis
	FVector RotatedVector = ReferenceAxis.RotateAngleAxis(-ArgumentOfPeriapsis, FVector::ZAxisVector);
	// Set Position Vector of Periapsis
	PeriapsisVector = RotatedVector * PeriapsisRadius;

	/*// Rotate Trajectory
	FRotator Rotation = FRotator::MakeFromEuler(FVector::ZAxisVector * -ArgumentOfPeriapsis);
	Trajectory->SetActorRotation(Rotation);*/

}

void AOrbit::DrawTrajectory()
{
	if (!Trajectory || !OrbitingBody || !CentralBody) { return; }
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
	/*if (FMath::Abs(Eccentricity) < DBL_EPSILON)
	{
		// if Eccentricity is 0, Orbit is circular.
		Trajectory->SetSemiMajorAxis(SemiMajorAxis);
		Trajectory->SetSemiMinorAxis(SemiMajorAxis);
	}*/

	Trajectory->SetActorLocation(EllipticalCenter);
	Trajectory->SetSemimajorAxis(SemimajorAxis);
	Trajectory->SetSemiminorAxis(SemiminorAxis);
	
	Trajectory->Update();
	// Rotate Trajectory
	FRotator Rotation = FRotator::MakeFromEuler(FVector::ZAxisVector * -ArgumentOfPeriapsis);
	Trajectory->SetActorRotation(Rotation);
}

/*void AOrbit::CalculateTrajectory()
{
	if (!OrbitingBody || !CentralBody) { return; }
	// Get the Orbital State Vectors (Position and Velocity)
	const FVector Position = OrbitingBody->GetActorLocation() - CentralBody->GetActorLocation() * ASim::DISTANCE_MULTIPLIER;
	const FVector Velocity = OrbitingBody->GetVelocityVector();

	// If magnitude of Position Vector is Zero, abort
	//double OrbitalDistance = OrbitingBody->GetOrbitalDistance();
	if(FMath::Abs(OrbitalDistance * ASim::DISTANCE_MULTIPLIER) < DBL_EPSILON)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: Orbital Distance is Zero"));
		return;
	}

	// Get the Standard Gravitational Parameter
	const double SGP = ASim::GRAVITATIONAL_CONSTANT * CentralBody->GetMassOfBody() * ASim::SOLAR_MASS;
	if (abs(SGP) < DBL_EPSILON)
	{
		// Cannot continue if SGP is zero
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error: SGP is Zero"));
		return;
	} 

	// 1. Compute Specific Angular Momentum
	ComputeSpecificAngularMomentum(Position, Velocity);
	// 2. Compute Ascending Node Vector
	ComputeAscendingNodeVector();
	
	// 3. Compute Eccentricity Vector
	ComputeEccentricityVector(Velocity, Position, SGP);

	// 4. Compute Semi-Latus Rectum
	ComputeSemiLatusRectum(SGP);

	// 5. Compute the Inclination
	ComputeInclination();

	// If Inclination is zero, the following values will also be zero
	LongitudeOfAscendingNode = 0.0;
	ArgumentOfPeriapsis = 0.0;
	const double AscendingNodeMagnitude = AscendingNodeVector.Length() * ASim::DISTANCE_MULTIPLIER;
	if(FMath::Abs(Inclination) > DBL_EPSILON) 
	{
		// If Ascending Node Magnitude is Zero, abort
		if (FMath::Abs(AscendingNodeMagnitude) < DBL_EPSILON) { return; }
		
		// 6. Compute Longitude of the Ascending Node
		ComputeLongitudeOfAscendingNode(AscendingNodeMagnitude);

		// 7. Compute Argument of Periapsis
		ComputeArgumentOfPeriapsis(AscendingNodeMagnitude);
	}

	// If Eccentricity is Zero, use Argument of Latitude instead of True Anomaly
	if(FMath::Abs(Eccentricity) > DBL_EPSILON)
	{
		ComputeTrueAnomaly(Position, Velocity);
	}
	else if (FMath::Abs(Inclination) > DBL_EPSILON)
	{
		// If Eccentricity is Zero, use Argument of Latitude instead of True Anomaly
		ComputeArgumentOfLatitude(AscendingNodeMagnitude, Position);
	}
	else
	{
		// If Eccentricity and Inclination are both Zero,  we must use the True Longitude
		double TrueLongitude = Position.X / OrbitalDistance;
	}

	SemiminorAxis = FMath::Sqrt(SemimajorAxis * SemiLatusRectum);


	FVector AscendingNodeDirection = AscendingNodeVector / AscendingNodeMagnitude;
	EllipticalCenter = AscendingNodeVector - (AscendingNodeDirection * SemimajorAxis);

}*/

void AOrbit::ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity)
{
	// Cross product of Position and Velocity
	SpecificAngularMomentum = Position.Cross(Velocity) / ASim::DISTANCE_MULTIPLIER;
}

void AOrbit::ComputeAscendingNodeVector()
{
	// Cross product of the unit vector of the Z-axis, and the specific angular momentum
	AscendingNodeVector = FVector(0.0, 0.0, 1.0).Cross(SpecificAngularMomentum);
}

void AOrbit::ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, const double SGP)
{
	
	// Cross product of Velocity and Specific Angular Momentum, divided by the Universal Gravitational Parameter,
	// Minus the Position Vector divided by its magnitude
	EccentricityVector = (Velocity.Cross(SpecificAngularMomentum) / SGP) - (Position / Position.Length());
	EccentricityVector /= ASim::DISTANCE_MULTIPLIER;
	Eccentricity = EccentricityVector.Length();
}

void AOrbit::ComputeSemiLatusRectum( double SGP)
{
	SemilatusRectum = (SpecificAngularMomentum * ASim::DISTANCE_MULTIPLIER).SquaredLength()  / SGP;
	
	if (abs(1 - Eccentricity) < DBL_EPSILON) // Dont divide by zero
	{ // If Eccentricity is 1, Orbit is Parabolic, if greater than 1, it is Hyperbolic
		SemimajorAxis = 0.0;
	}
	else
	{
		SemimajorAxis = SemilatusRectum / (1 - pow(Eccentricity, 2));
	}
	SemilatusRectum /= ASim::DISTANCE_MULTIPLIER;
	SemimajorAxis /= ASim::DISTANCE_MULTIPLIER;
}

void AOrbit::ComputeInclination()
{
	const double angle = FVector::DotProduct(FVector(0.0, 0.0, 1.0), SpecificAngularMomentum) / SpecificAngularMomentum.Length();
	Inclination = FMath::Acos(angle);
}

void AOrbit::ComputeLongitudeOfAscendingNode(const double AscendingNodeMagnitude)
{
	if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
	{
		throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
	}
	const double angle = FVector::DotProduct(FVector(1.0,0.0,0.0), AscendingNodeVector) / AscendingNodeMagnitude;
	LongitudeOfAscendingNode = FMath::Acos(angle);
	if (AscendingNodeVector.Y < 0.0)
	{
		LongitudeOfAscendingNode = 360.0 - LongitudeOfAscendingNode;
	}
}

void AOrbit::ComputeArgumentOfPeriapsis(const double AscendingNodeMagnitude)
{
	if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
	{
		throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
	}
	if (abs(Eccentricity) < DBL_EPSILON)
	{
		ArgumentOfPeriapsis = 0.0;
		return;
	}
	
	const double angle = FVector::DotProduct(EccentricityVector, AscendingNodeVector) / (AscendingNodeMagnitude * Eccentricity);
	ArgumentOfPeriapsis = FMath::Acos(angle);
	if (EccentricityVector.Z < 0)
	{
		ArgumentOfPeriapsis = 360.0 - ArgumentOfPeriapsis;
	}
}

void AOrbit::ComputeTrueAnomaly(const FVector& Position, const FVector& Velocity)
{
	
	const double angle = FVector::DotProduct(EccentricityVector, Position) / (Eccentricity * OrbitalRadius * ASim::DISTANCE_MULTIPLIER);
	if (FVector::DotProduct(Position, Velocity) < 0.0)
	{
		TrueAnomaly = 360 - FMath::Acos(angle);
	}
	else
	{
		TrueAnomaly = FMath::Acos(angle);
	}
}

void AOrbit::ComputeArgumentOfLatitude(const double AscendingNodeMagnitude, const FVector& Position)
{
	
}

void AOrbit::CalculateSpecifcOrbitalEnergy()
{
	SpecificOrbitalEnergy = (FMath::Pow(InitialOrbitalSpeed * ASim::KM_TO_M, 2.0) / 2.0) - ((ASim::GRAVITATIONAL_CONSTANT * (CentralBody->GetMassOfBody() * ASim::SOLAR_MASS)) / (PeriapsisRadius * ASim::DISTANCE_MULTIPLIER));
}

void AOrbit::CalculateSemiMajorAxisFromSpecifcOrbitalEnergy()
{
	SemimajorAxis = -(ASim::GRAVITATIONAL_CONSTANT * (CentralBody->GetMassOfBody() * ASim::SOLAR_MASS) / (2.0 * SpecificOrbitalEnergy));
	SemimajorAxis /= ASim::DISTANCE_MULTIPLIER; // Get value in in-editor units
}

void AOrbit::CalculateApoapsisRadius()
{
	FVector DirectionVector(CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation());
	DirectionVector.Normalize();
	ApoapsisVector = PeriapsisVector + (DirectionVector * (2.0 * SemimajorAxis));
	EllipticalCenter = PeriapsisVector + (DirectionVector * SemimajorAxis);

	ApoapsisRadius = FVector::Distance(CentralBody->GetActorLocation(), ApoapsisVector);
}

void AOrbit::CalculatePeriodFromSemiMajorAxis()
{
	OrbitalPeriod = 2.0L * Sim::Pi * FMath::Sqrt(FMath::Pow(SemimajorAxis * ASim::DISTANCE_MULTIPLIER, 3.0) / ASim::GRAVITATIONAL_CONSTANT * (CentralBody->GetMassOfBody() * ASim::SOLAR_MASS));
	OrbitalPeriod /= ASim::SECONDS_IN_DAY;
}

void AOrbit::CalculateSemiMajorAxisFromPeriod(const double Period)
{
	SemimajorAxis = FMath::Pow( (ASim::GRAVITATIONAL_CONSTANT * CentralBody->GetMassOfBody()  * ASim::SOLAR_MASS * FMath::Pow(Period * ASim::SECONDS_IN_DAY, 2.0) /
		4.0 * std::pow(Sim::Pi, 2.0)), 1.0 / 3.0);
}

void AOrbit::CalculateEccentricityFromSpecificAngularMomentum()
{
	Eccentricity = FMath::Sqrt((1 + (2.0 * SpecificOrbitalEnergy * std::pow(SpecificAngularMomentum.Length(), 2.0))) / FMath::Pow(ASim::GRAVITATIONAL_CONSTANT * CentralBody->GetMassOfBody(), 2));
}

void AOrbit::CalculateLinearEccentricity()
{
	// Linear eccentricity is the distance between the center of the ellipse and either of the foci
	// Since we know that the central body lies at one of the foci, we can use this to determine the eccentricity
	LinearEccentricity = (EllipticalCenter - CentralBody->GetActorLocation()).Length();

	Eccentricity = LinearEccentricity / SemimajorAxis;

	// Once we have the eccentricity, we can calculate the Semi-latus rectum and the Semi-minor axis
	SemilatusRectum = SemimajorAxis * (1 - FMath::Pow(Eccentricity, 2));
	SemiminorAxis = FMath::Sqrt(SemimajorAxis * SemilatusRectum);
}

void AOrbit::CalculateEccentricityFromApsides()
{
	Eccentricity = (ApoapsisRadius - PeriapsisRadius) / (ApoapsisRadius + PeriapsisRadius);
}

