#pragma once
#include "AstroConstants.h"

namespace OrbitalPeriod
{
	inline double CalculatePeriod(const double SemiMajorAxis, const double CentralMass)
	{
		double const OrbitalPeriod = 2.0 * Unit::Pi * FMath::Pow(SemiMajorAxis, 1.5) * FMath::Sqrt( 1e5 / (Unit::G * CentralMass * Unit::SM)) / Unit::SECONDS_IN_DAY;
		return OrbitalPeriod;
	}
}
