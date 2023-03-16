#pragma once
#include "AstroConstants.h"

namespace Apoapsis
{
	inline double RadiusFromEccentricity(const double SemiMajorAxis, const double Eccentricity)
	{
		// Calculate orbital radius at Apoapsis from the Semi-major axis and the Eccentricity
		return (1.0 + Eccentricity) * SemiMajorAxis;
	}
	inline double RadiusFromPeriapsis(const double SemiMajorAxis, const double PeriapsisRadius)
	{
		// The length of the orbital radius at Apoapsis is the length of the major axis - the length of the radius at periapsis
		return (2.0 * SemiMajorAxis) - PeriapsisRadius;
	}
}

namespace Periapsis
{
	inline double RadiusFromEccentricity(const double SemiMajorAxis, const double Eccentricity)
	{
		// Calculate orbital radius at periapsis from the Semi-major axis and the Eccentricity
		return (1.0 - Eccentricity) * SemiMajorAxis;
	}
	inline double RadiusFromApoapsis(const double SemiMajorAxis, const double ApoapsisRadius)
	{
		// The length of the orbital radius at periapsis is the length of the major axis - the length of the radius at apoapsis
		return (2.0 * SemiMajorAxis) - ApoapsisRadius;
	}
}