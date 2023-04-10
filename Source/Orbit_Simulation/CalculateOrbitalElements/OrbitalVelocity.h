#pragma once
#include "AstroConstants.h"

namespace OrbitalVelocity
{
	// Calculate the Orbital Speed at Apoapsis
	inline double CalculateAtApoapsis(const double Eccentricity, const double SemiMajorAxis, const double CentralMass)
	{
		const double StandardGravitationalParameter = Unit::GRAVITATIONAL_CONSTANT * CentralMass * Unit::SOLAR_MASS;
		double MinimumSpeed = FMath::Sqrt(((1 - Eccentricity) * StandardGravitationalParameter) / ((1 + Eccentricity) * (SemiMajorAxis * Unit::DISTANCE_MULT)));
		MinimumSpeed /= Unit::DISTANCE_MULT;
		return MinimumSpeed;
	}

	// Calculate the Orbital Speed at Periapsis
	inline double CalculateAtPeriapsis(const double Eccentricity, const double SemiMajorAxis, const double CentralMass)
	{
		const double StandardGravitationalParameter = Unit::GRAVITATIONAL_CONSTANT * CentralMass * Unit::SOLAR_MASS;
		double MaximumSpeed = FMath::Sqrt(((1 + Eccentricity) * StandardGravitationalParameter) / ((1 - Eccentricity) * (SemiMajorAxis * Unit::DISTANCE_MULT)));
		MaximumSpeed /= Unit::DISTANCE_MULT;
		return MaximumSpeed;
	}

	/*inline double CalculateAtRadius()
	{
		
	}*/
}