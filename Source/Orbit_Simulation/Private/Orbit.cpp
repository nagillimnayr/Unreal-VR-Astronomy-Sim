// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"
#include "AstroBody.h"
#include "Sim.h"
#include "Trajectory.h"

// Sets default values
AOrbit::AOrbit() :
	CentralBody(nullptr),
	OrbitingBody(nullptr),
	Trajectory(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trajectory = CreateDefaultSubobject<ATrajectory>(TEXT("Trajectory")); // Create Trajectory SubObject
}

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
	// If either body is invalid, destroy Orbit and return
	if (!CentralBody || !OrbitingBody) { Destroy(); }
	else
	{
		InitializeOrbitingBody();
		UpdateOrbitalDistance();
	}

	// Calculate Semi-Major and Semi-Minor Axes
}


void AOrbit::Update(const float DeltaTime)
{
	OrbitingBody->CalculateAcceleration(CentralBody);
	OrbitingBody->UpdateVelocity(DeltaTime);
	OrbitingBody->UpdatePosition(DeltaTime);

	UpdateOrbitalDistance();
}

void AOrbit::UpdateOrbitalDistance()
{
	OrbitalDistance = FVector::Distance(CentralBody->GetActorLocation(), OrbitingBody->GetActorLocation());
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
	
	// Get direction vector from Orbiting Body to Central Body
	FVector Direction = (CentralBody->GetActorLocation() - OrbitingBody->GetActorLocation());
	// Get Rotator from Direction
	FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	// Set Rotation of Orbiting Body so that its Forward Vector is pointing at Central Body
	OrbitingBody->SetActorRotation(Rotation);
	
	// Scalar multiply Initial Velocity with Orbiting Body's Right Vector so that Velocity is Orthogonal
	// to the Direction Vector
	double InitialVelocity = OrbitingBody->GetInitialVelocity() * ASim::KM_TO_M; // Convert Initial Velocity from km to m
	FVector Velocity = OrbitingBody->GetActorRightVector() * InitialVelocity;
	OrbitingBody->InitializeVelocity(Velocity); // Set Orbiting Body's Velocity
}


// Compute Keplerian Elements from Orbital State Vectors

void AOrbit::DrawTrajectory()
{
}

void AOrbit::UpdateTrajectory()
{
	// Get the Orbital State Vectors (Position and Velocity)
	const FVector Position = OrbitingBody->GetActorLocation() - CentralBody->GetActorLocation();
	const FVector Velocity = OrbitingBody->GetVelocityVector();

	// If magnitude of Position Vector is Zero, abort
	//double OrbitalDistance = OrbitingBody->GetOrbitalDistance();
	if(FMath::Abs(OrbitalDistance) < DBL_EPSILON) { return; }

	// Get the Standard Gravitational Parameter
	const double SGP = ASim::GRAVITATIONAL_CONSTANT * CentralBody->GetMassOfBody();
	if (abs(SGP) < DBL_EPSILON) { return; } // Cannot continue if SGP is zero

	// 1. Compute Specific Angular Momentum
	const FVector SpecificAngularMomentum = ComputeSpecificAngularMomentum(Position, Velocity);
	// 2. Compute Ascending Node Vector
	const FVector AscendingNodeVector = ComputeAscendingNodeVector(SpecificAngularMomentum);
	
	// 3. Compute Eccentricity Vector
	const FVector EccentricityVector = ComputeEccentricityVector(Velocity, Position, SpecificAngularMomentum, SGP);

	// 4. Compute Semi-Latus Rectum
	const double SemiLatusRectum = ComputeSemiLatusRectum(SpecificAngularMomentum, SGP);

	// 5. Compute the Inclination
	Inclination = ComputeInclination(SpecificAngularMomentum);

	// If Inclination is zero, the following values will also be zero
	LongitudeOfAscendingNode = 0.0;
	ArgumentOfPeriapsis = 0.0;
	if(FMath::Abs(Inclination) > DBL_EPSILON) 
	{
		const double AscendingNodeMagnitude = AscendingNodeVector.Length();
		// If Ascending Node Magnitude is Zero, abort
		if (FMath::Abs(AscendingNodeMagnitude) < DBL_EPSILON) { return; }
		
		// 6. Compute Longitude of the Ascending Node
		LongitudeOfAscendingNode = ComputeLongitudeOfAscendingNode(AscendingNodeVector, AscendingNodeMagnitude);

		// 7. Compute Argument of Periapsis
		ArgumentOfPeriapsis = ComputeArgumentOfPeriapsis(AscendingNodeVector, AscendingNodeMagnitude, EccentricityVector);
		
		// If Eccentricity is Zero, use Argument of Latitude instead of True Anomaly
		if(FMath::Abs(Eccentricity) < DBL_EPSILON)
		{
			double ArgumentOfLatitude = ComputeArgumentOfLatitude(AscendingNodeVector, AscendingNodeMagnitude, Position);
		}
	}

	// If Eccentricity and Inclination are both Zero,  we must use the True Longitude
	double TrueLongitude = Position.X / OrbitalDistance;
}

FVector AOrbit::ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity)
{
	// Cross product of Position and Velocity
	return Position.Cross(Velocity);
}

FVector AOrbit::ComputeAscendingNodeVector(const FVector& SpecificAngularMomentum)
{
	// Cross product of the unit vector of the Z-axis, and the specific angular momentum
	return FVector(0.0, 0.0, 1.0).Cross(SpecificAngularMomentum);
}

FVector AOrbit::ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, const FVector& SpecificAngularMomentum, const double SGP)
{
	// Cross product of Velocity and Specific Angular Momentum, divided by the Universal Gravitational Parameter,
	// Minus the Position Vector divided by its magnitude
	const FVector EccentricityVector = (Velocity.Cross(SpecificAngularMomentum) / SGP) - (Position / Position.Length());
	Eccentricity = EccentricityVector.Length();
	return EccentricityVector;
}

double AOrbit::ComputeSemiLatusRectum( const FVector& SpecificAngularMomentum, double SGP)
{
	double SemiLatusRectum = SpecificAngularMomentum.SquaredLength() / SGP;
	if (abs(1 - Eccentricity) < DBL_EPSILON) // Dont divide by zero
	{ // If Eccentricity is 1, Orbit is Parabolic, if greater than 1, it is Hyperbolic
		SemiMajorAxis = 0;
	}
	else
	{
		SemiMajorAxis = SemiLatusRectum / (1 - pow(Eccentricity, 2));
	}
	return SemiLatusRectum;
}

double AOrbit::ComputeInclination(const FVector& SpecificAngularMomentum)
{
	const double angle = FVector::DotProduct(FVector(0.0, 0.0, 1.0), SpecificAngularMomentum) / SpecificAngularMomentum.Length();
	return FMath::Acos(angle);
}

double AOrbit::ComputeLongitudeOfAscendingNode(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude)
{
	if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
	{
		throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
	}
	const double angle = FVector::DotProduct(FVector(1.0,0.0,0.0), AscendingNodeVector) / AscendingNodeMagnitude;
	LongitudeOfAscendingNode = FMath::Acos(angle);
	if (AscendingNodeVector.Y < 0.0)
	{
		return 360.0 - LongitudeOfAscendingNode;
	}
	return LongitudeOfAscendingNode;
}

double AOrbit::ComputeArgumentOfPeriapsis(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude, const FVector& EccentricityVector)
{
	if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
	{
		throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
	}
	if (abs(Eccentricity) < DBL_EPSILON)
	{
		return 0.0;
	}
	
	const double angle = FVector::DotProduct(EccentricityVector, AscendingNodeVector) / (AscendingNodeMagnitude * Eccentricity);
	ArgumentOfPeriapsis = FMath::Acos(angle);
	if (EccentricityVector.Z < 0)
	{
		return 360.0 - ArgumentOfPeriapsis;
	}
	return ArgumentOfPeriapsis;
}

double AOrbit::ComputeTrueAnomaly(const FVector& Position, const FVector& Velocity)
{
	return 0.0;
}

double AOrbit::ComputeArgumentOfLatitude(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude, const FVector& Position)
{
	return 0.0;
}