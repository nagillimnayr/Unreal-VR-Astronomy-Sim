// Fill out your copyright notice in the Description page of Project Settings.


#include "Sim.h"

#include "Kismet/GameplayStatics.h"
//#include "AstroBody.h"
//#include "Orbit_Base.h"

// Initialize static constants
const double ASim::GRAVITATIONAL_CONSTANT = 6.674e-11; // m^3/kg/s^2
//const double ASim::ASTRONOMICAL_UNIT = 1.495978707e11; // AU in m
const double ASim::SOLAR_MASS = 1.989e30; // Mass of the sun in kg

// Unit Conversion Multipliers
const double ASim::SECONDS_IN_DAY = 86400.0; // 1 second in simulation = 1 day
const double ASim::DISTANCE_MULTIPLIER = 1e11 / 1000.0; // 1 in-editor unit = 1x10^8m, 1000 in-editor units = 1x10^11m
const double ASim::KM_TO_M = 1000.0;

// Sets default values
ASim::ASim() :
Timer(0.0),
TimeScale(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ASim::BeginPlay()
{
	Super::BeginPlay();
	GetWorldSettings()->SetTimeDilation(TimeScale);
}

// Called every frame
void ASim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer += DeltaTime;

	/*if(Timer > 365.25)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause Game after one year
	}*/

}



#if WITH_EDITOR
void ASim::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Set TimeScale
	GetWorldSettings()->SetTimeDilation(TimeScale);
}
#endif