#pragma once
#include "AstroConstants.h"

namespace SpecificOrbitalEnergy
{
	inline double CalculateSpecificOrbitalEnergy(const double OrbitalSpeed, const double CentralMass, const double PeriapsisRadius)
	{
		const double SpecificOrbitalEnergy = (FMath::Pow(OrbitalSpeed * Sim::KM_TO_M, 2.0) / 2.0) - ((Sim::GRAVITATIONAL_CONSTANT * (CentralMass * Sim::SOLAR_MASS)) / (PeriapsisRadius * Sim::DISTANCE_MULT));
		return SpecificOrbitalEnergy;
	}
}
