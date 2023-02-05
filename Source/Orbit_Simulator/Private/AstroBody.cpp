// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"

// Sets default values
AAstroBody::AAstroBody() :
	mass(0.0),
	initialVelocity(0.0),
	velocity(ForceInitToZero),
	acceleration(ForceInitToZero)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAstroBody::CalculateAcceleration(float DeltaTime)
{
	
}

void AAstroBody::UpdateVelocity()
{
}

void AAstroBody::UpdatePosition()
{
}

// Called every frame
void AAstroBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	double RunningTime = GetGameTimeSinceCreation();
	double DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * 20.0;
	double DeltaRotation = DeltaTime * 20.0;
	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);

}

void AAstroBody::PostInitProperties()
{
	Super::PostInitProperties();
	//velocity = initialVelocity;
}

#if WITH_EDITOR
void AAstroBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
