#pragma once
#include "AstroConstants.h"

namespace Eccentricity
{
	inline double CalculateFromLinearEccentricity(const double SemiMajorAxis, const double LinearEccentricity)
	{
		const double Eccentricity = LinearEccentricity / SemiMajorAxis;
		return Eccentricity;
	}
	
	inline double CalculateFromAxes(const double SemiMajorAxis, const double SemiMinorAxis)
	{
		const double Eccentricity = FMath::Sqrt(1.0 - ((SemiMinorAxis * SemiMinorAxis) / (SemiMajorAxis * SemiMajorAxis)));
		return Eccentricity;
	}

	inline double CalculateFromSpecificAngularMomentum(const double SpecificOrbitalEnergy, const double CentralMass, const FVector SpecificAngularMomentum)
	{
		const double Eccentricity = FMath::Sqrt((1.0 + (2.0 * SpecificOrbitalEnergy * std::pow(SpecificAngularMomentum.Length(), 2.0))) / FMath::Pow(Unit::GRAVITATIONAL_CONSTANT * CentralMass, 2));
		return Eccentricity;
	}

	inline double CalculateFromApsides(const double ApoapsisRadius, const double PeriapsisRadius)
	{
		const double Eccentricity = (ApoapsisRadius - PeriapsisRadius) / (ApoapsisRadius + PeriapsisRadius);
		return Eccentricity;
	}
	
	/*FVector ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, const double SGP)
	{
		// Cross product of Velocity and Specific Angular Momentum, divided by the Universal Gravitational Parameter,
		// Minus the Position Vector divided by its magnitude
		return (Velocity.Cross(SpecificAngularMomentum) / SGP) - (Position / Position.Length());

	}*/
	
}

namespace LinearEccentricity
{
	inline double CalculateFromEccentricity(double SemiMajorAxis, double Eccentricity)
	{
		return SemiMajorAxis * Eccentricity;
	}
	
	inline double CalculateFromApsis(const double SemiMajorAxis, const double Apsis)
	{
		// Linear eccentricity is the distance between the center of the ellipse and either of the foci, it can be
		// calculated either from SemiMajorAxis - (PeriapsisRadius) or from (ApoapsisRadius - SemiMajorAxis)
		return SemiMajorAxis > Apsis ? SemiMajorAxis - Apsis : Apsis - SemiMajorAxis;
		
	}
	
	inline double CalculateFromAxes(const double SemiMajorAxis, const double SemiMinorAxis)
	{
		return FMath::Sqrt((SemiMajorAxis * SemiMajorAxis) - (SemiMinorAxis * SemiMinorAxis));
		
	}
}