// Fill out your copyright notice in the Description page of Project Settings.


#include "Sim.h"

// Initialize static constants
const double ASim::GRAVITATIONAL_CONSTANT = 60674e-11; // m^3/kg/s^2
const double ASim::ASTRONOMICAL_UNIT = 1.495978707e11; // AU in m
const double ASim::SOLAR_MASS = 1.989e30; // Mass of the sun in kg

const double ASim::SECONDS_IN_DAY = 86400;
const double ASim::DISTANCE_MULTIPLIER = 1e11;
const double ASim::KM_TO_M = 1000;

// Sets default values
ASim::ASim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASim::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASim::CalculateAcceleration(float DeltaTime)
{
	
}

// Called every frame
void ASim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

