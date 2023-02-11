// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbit.h"
#include "AstroBody.h"
#include "Sim.h"

// Sets default values
AOrbit::AOrbit() :
	CentralBody(nullptr),
	OrbitingBody(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	InitializeOrbitingBody();
	UpdateOrbitalDistance();
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
	OrbitalDistance = FVector::Distance(CentralBody->GetActorLocation(), OrbitingBody->GetActorLocation());
}

// Called every frame
void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If either body is invalid, destroy Orbit and return
	if (!CentralBody || !OrbitingBody) { Destroy(); return;}

	UpdateOrbitingBody(DeltaTime);
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
