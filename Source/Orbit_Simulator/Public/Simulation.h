// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Astrobody.h"

/**
 * 
 */
class ORBIT_SIMULATOR_API Simulation
{
public:
	Simulation();
	~Simulation();
protected:
	// Constants
	static const double GRAVITATIONAL_CONSTANT;
	static const double ASTRONOMICAL_UNIT;
	static const double SOLAR_MASS;

	static const double SECONDS_IN_DAY;
	static const double DISTANCE_MULTIPLIER;
	static const double KM_TO_M;

	TArray<AAstroBody*> bodies;

};
