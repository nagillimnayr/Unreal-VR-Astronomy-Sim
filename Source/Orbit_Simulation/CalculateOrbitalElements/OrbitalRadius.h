#pragma once
#include "AstroConstants.h"

namespace OrbitalRadius
{
	// Polar Coordinates relative to Focus at Origin (Angle should be in Radians)
	inline double CalculateRadiusAtTrueAnomaly(const double Angle, const double Eccentricity, const double SemiLatusRectum)
	{
		// Calculates the radius at a particular angle, using the elliptical formula in polar form, relative to the right focus

		const double Radius = SemiLatusRectum / (1.0 - (Eccentricity * FMath::Cos(Angle)));
		return Radius;
	}
}
