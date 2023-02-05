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

void AAstroBody::CalculateAcceleration()
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
