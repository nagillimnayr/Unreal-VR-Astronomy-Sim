#pragma once
#include "AstroBody.h"
#include "AstroConstants.h"

namespace Motion
{
	inline FVector CalculateAcceleration(const AAstroBody* const OrbitingBody, const AAstroBody* const CentralBody)
	{
		// If either body is invalid, throw an exception
		if (!OrbitingBody ) { throw std::exception("> Error: OrbitingBody is NULL"); }
		if (!CentralBody ) { throw std::exception("> Error: CentralBody is NULL"); }

		const FVector CentralPos = CentralBody->GetActorLocation();
		const FVector OrbitPos = OrbitingBody->GetActorLocation();
		const FVector Difference = CentralPos - OrbitPos;
		const double DistanceSquared = (Difference * Unit::DISTANCE_MULT).SquaredLength(); // Multiply by DISTANCE_MULTIPLIER
		//FVector Direction = Difference.GetSafeNormal(1.0); // Normalized Direction Vector pointing from this Body to the other Body
		FVector Direction = Difference;
		Direction.Normalize(); // Normalized Direction Vector pointing from Orbiting Body to the Central Body
	
		// Calculate gravitational force
		const double Force = (Unit::GRAVITATIONAL_CONSTANT * CentralBody->GetMassOfBody() * Unit::SOLAR_MASS) / DistanceSquared;

		// Return Acceleration Vector
		return Direction * Force;
	}
	
}
