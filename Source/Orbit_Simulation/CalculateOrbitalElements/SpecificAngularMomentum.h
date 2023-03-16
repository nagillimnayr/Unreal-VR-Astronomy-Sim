#pragma once
#include "AstroConstants.h"

namespace SpecificAngularMomentum
{
	inline FVector ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity)
	{
		// Cross product of Position and Velocity
		return Position.Cross(Velocity) / Sim::DISTANCE_MULT;
	}
}
