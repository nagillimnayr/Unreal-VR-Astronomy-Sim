#pragma once
#include "AstroConstants.h"

namespace SemiMajorAxis
{
	inline double CalculateFromPeriapsis(const double Eccentricity, const double PeriapsisRadius)
	{
		return PeriapsisRadius / (1 - Eccentricity);
	}
	inline double CalculateFromApoapsis(const double Eccentricity, const double ApoapsisRadius)
	{
		return ApoapsisRadius / (1 + Eccentricity);
	}
	
	inline double CalculateFromLinearEccentricity(const double Eccentricity, const double LinearEccentricity)
	{
		return LinearEccentricity / Eccentricity;
	}
	
	inline double CalculateFromSpecificOrbitalEnergy(const double CentralMass, const double SpecificOrbitalEnergy)
	{
		double SemiMajorAxis = -(Sim::GRAVITATIONAL_CONSTANT * (CentralMass * Sim::SOLAR_MASS) / (2.0 * SpecificOrbitalEnergy));
		SemiMajorAxis /= Sim::DISTANCE_MULT; // Get value in in-editor units
		return SemiMajorAxis;
	}
	inline double CalculateFromPeriod(const double Period, const double CentralMass)
	{
		double const SemiMajorAxis = FMath::Pow( (Sim::GRAVITATIONAL_CONSTANT * CentralMass  * Sim::SOLAR_MASS * FMath::Pow(Period * Sim::SECONDS_IN_DAY, 2.0) /
											   4.0 * std::pow(Sim::Pi, 2.0)), 1.0 / 3.0);

		return SemiMajorAxis;
	}
	inline double CalculateFromApsides(const double Apoapsis, const double Periapsis)
	{
		return (Periapsis + Apoapsis) / 2.0;
	}
}

namespace SemiMinorAxis
{
	
	inline double CalculateFromSemiLatusRectum(const double SemiMajorAxis, const double SemiLatusRectum)
	{
		// From the semi-major axis and the semi-latus rectum we can calculate the Semi-minor axis
		const double SemiMinorAxis = FMath::Sqrt(SemiMajorAxis * SemiLatusRectum);
		return SemiMinorAxis;
	}
	inline double CalculateFromApsides(const double Apoapsis, const double Periapsis)
	{
		return FMath::Sqrt(Apoapsis * Periapsis);
	}
	
}

namespace SemiLatusRectum
{
	
	inline double CalculateSemiLatusRectum(double SemiMajorAxis, double Eccentricity)
	{
		// From the semi-major axis and the eccentricity, we can calculate the Semi-latus rectum 
		const double SemiLatusRectum = SemiMajorAxis * (1.0 - FMath::Pow(Eccentricity, 2.0));
		return SemiLatusRectum;
	}
	inline double CalculateSemiLatusRectumFromApsides(const double Apoapsis, const double Periapsis)
	{
		return (2.0 * Apoapsis * Periapsis) / (Apoapsis + Periapsis);
	}
	
	/*double ComputeSemiLatusRectum( double SGP)
	{
		SemiLatusRectum = (SpecificAngularMomentum * ASim::DISTANCE_MULTIPLIER).SquaredLength()  / SGP;
	
		if (abs(1 - Eccentricity) < DBL_EPSILON) // Dont divide by zero
			{ // If Eccentricity is 1, Orbit is Parabolic, if greater than 1, it is Hyperbolic
			SemiMajorAxis = 0.0;
			}
		else
		{
			SemiMajorAxis = SemiLatusRectum / (1 - pow(Eccentricity, 2));
		}
		SemiLatusRectum /= ASim::DISTANCE_MULTIPLIER;
		return SemiLatusRectum;
	}*/
	
}