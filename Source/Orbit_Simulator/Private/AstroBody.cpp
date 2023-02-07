// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"
#include "Sim.h"
#include "GameProjectGeneration/Classes/TemplateProjectDefs.h"

// Sets default values
AAstroBody::AAstroBody() :
	mass(0.0),
	InitialVelocity(0.0),
	OrbitalVelocity(ForceInitToZero),
	Acceleration(ForceInitToZero)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAstroBody::CalculateAcceleration(AAstroBody* OtherBody)
{
	FVector OtherPos = OtherBody->GetActorLocation();
	FVector ThisPos = GetActorLocation();
	FVector Difference = OtherPos - ThisPos;
	double DistanceSquared = (Difference * ASim::DISTANCE_MULTIPLIER).SquaredLength(); // Multiply by DISTANCE_MULTIPLIER
	//FVector Direction = Difference.GetSafeNormal(1.0); // Normalized Direction Vector pointing from this Body to the other Body
	FVector Direction = Difference;
	Direction.Normalize(); // Normalized Direction Vector pointing from this Body to the other Body
	
	// Calculate gravitational force
	double Force = (ASim::GRAVITATIONAL_CONSTANT * OtherBody->mass * ASim::SOLAR_MASS) / DistanceSquared;
	Acceleration = Direction * Force;

	AccelerationMagnitude = Force;
	OrbitalDistance = (Difference/* * ASim::DISTANCE_MULTIPLIER*/).Length(); // Get distance between Bodies
}

void AAstroBody::UpdateVelocity(const double DeltaTime)
{
	OrbitalVelocity += Acceleration * (DeltaTime /** ASim::SECONDS_IN_DAY*/);
	VelocityMagnitude = OrbitalVelocity.Length() / ASim::DISTANCE_MULTIPLIER;
}

void AAstroBody::UpdatePosition(const double DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update Position based on Velocity
	// Divide by DISTANCE_MULTIPLIER so the result will be in in-editor units
	Position += OrbitalVelocity * (DeltaTime /** ASim::SECONDS_IN_DAY */ / ASim::DISTANCE_MULTIPLIER);
	SetActorLocation(Position);
}

// Called every frame
void AAstroBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AAstroBody::PostInitProperties()
{
	Super::PostInitProperties();

}

#if WITH_EDITOR
void AAstroBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
