#pragma once
#include "AstroConstants.h"

namespace OrbitalPeriod
{
	inline double CalculatePeriod(const double SemiMajorAxis, const double CentralMass)
	{
		double const OrbitalPeriod = 2.0 * Sim::Pi * FMath::Pow(SemiMajorAxis, 1.5) * FMath::Sqrt( 1e5 / (Sim::G * CentralMass * Sim::SM)) / Sim::SECONDS_IN_DAY;
		return OrbitalPeriod;
	}
}
