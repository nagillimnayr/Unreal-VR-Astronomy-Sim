#pragma once
#include "AstroConstants.h"

namespace ArgumentOfPeriapsis
{
	/*double ComputeArgumentOfPeriapsis(const double AscendingNodeMagnitude)
	{
		if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
		{
			throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
		}
		if (abs(Eccentricity) < DBL_EPSILON)
		{
			ArgumentOfPeriapsis = 0.0;
			return;
		}
		
		const double angle = FVector::DotProduct(EccentricityVector, AscendingNodeVector) / (AscendingNodeMagnitude * Eccentricity);
		ArgumentOfPeriapsis = FMath::Acos(angle);
		if (EccentricityVector.Z < 0)
		{
			ArgumentOfPeriapsis = 360.0 - ArgumentOfPeriapsis;
		}
	}*/
}
