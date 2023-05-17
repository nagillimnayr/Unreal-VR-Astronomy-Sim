#pragma once
#include "Anomalies.h"
#include "AstroConstants.h"
#include "Stumpff.h"

namespace OrbitalRadius
{
	// Angle should be in Radians
	inline double CalculateFromEccentricAnomaly(const double EccentricAnomaly, const double SemiMajorAxis, const double Eccentricity)
	{
		return SemiMajorAxis * (1 - (Eccentricity * FMath::Cos(EccentricAnomaly)));
	}

	// Angle should be in Radians
	inline double CalculateAtTrueAnomaly(const double TrueAnomaly, const double SemiMajorAxis, const double Eccentricity)
	{
		// Calculates the radius at a particular angle, using the elliptical formula in polar form, relative to the right focus
		return (SemiMajorAxis * (1 - (Eccentricity * Eccentricity))) / (1 + (Eccentricity * FMath::Cos(TrueAnomaly)));
	}
	
	// Angle should be in Radians
	inline double CalculateAtTrueAnomalySLR(const double Angle, const double SemiLatusRectum, const double Eccentricity)
	{
		// Calculates the radius at a particular angle, using the elliptical formula in polar form, relative to the right focus
		const double Radius = SemiLatusRectum / (1.0 - (Eccentricity * FMath::Cos(Angle)));
		return Radius;
	}

	
	// Generalized form of Kepler's Equation which works regardless of Eccentricity. Solves for r(t) radius at time.
	inline double UniversalVariableFormulation(const double CurrentRadius, const double DeltaTime, const double SemiMajorAxis, const double CentralMass)
	{
		
		const double UGP = (CentralMass * Unit::SOLAR_MASS * Unit::GRAVITATIONAL_CONSTANT);
		const double Alpha = UGP / (SemiMajorAxis * Unit::DISTANCE_MULT);

		return 0.0;
		
	}

}
