#pragma once
#include "AstroConstants.h"

namespace SpecificOrbitalEnergy
{
	inline double CalculateSpecificOrbitalEnergy(const double OrbitalSpeed, const double CentralMass, const double PeriapsisRadius)
	{
		const double SpecificOrbitalEnergy = (FMath::Pow(OrbitalSpeed * Unit::KM_TO_M, 2.0) / 2.0) - ((Unit::GRAVITATIONAL_CONSTANT * (CentralMass * Unit::SOLAR_MASS)) / (PeriapsisRadius * Unit::DISTANCE_MULT));
		return SpecificOrbitalEnergy;
	}
}
