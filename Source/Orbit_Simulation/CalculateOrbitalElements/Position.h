#pragma once
#include "AstroConstants.h"

namespace Position
{
	// Convert Polar Coordinates to Cartesian Coordinates
	// TrueAnomaly should be in Radians
	inline FVector CalculateFromRadius(const double Radius, const double TrueAnomaly)
	{
		const double X = Radius * FMath::Cos(TrueAnomaly);
		const double Y = -(Radius * FMath::Sin(TrueAnomaly)); // Negative so that it will go counter-clockwise around the Z-axis
		return FVector(X, Y, 0.0);
	}
	
	
}