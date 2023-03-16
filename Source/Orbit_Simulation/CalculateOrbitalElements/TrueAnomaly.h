#pragma once
#include "AstroConstants.h"

namespace TrueAnomaly
{
	/*double ComputeTrueAnomaly(const FVector& Position, const FVector& Velocity)
	{
		double TrueAnomaly;
		const double angle = FVector::DotProduct(EccentricityVector, Position) / (Eccentricity * OrbitalRadius * Sim::DISTANCE_MULT);
		if (FVector::DotProduct(Position, Velocity) < 0.0)
		{
			TrueAnomaly = 360 - FMath::Acos(angle);
		}
		else
		{
			TrueAnomaly = FMath::Acos(angle);
		}

		return TrueAnomaly;
	}*/
}
