#pragma once
#include "AstroConstants.h"

namespace AscendingNode
{
	/*double ComputeLongitudeOfAscendingNode(const double AscendingNodeMagnitude)
	{
		if (abs(AscendingNodeMagnitude) < DBL_EPSILON)
		{
			throw std::exception("> Error: Cannot divide by AscendingNodeMagnitude if it is Zero.");
		}
		const double angle = FVector::DotProduct(FVector(1.0,0.0,0.0), AscendingNodeVector) / AscendingNodeMagnitude;
		LongitudeOfAscendingNode = FMath::Acos(angle);
		if (AscendingNodeVector.Y < 0.0)
		{
			LongitudeOfAscendingNode = 360.0 - LongitudeOfAscendingNode;
		}
	}*/

	
	/*FVector ComputeAscendingNodeVector()
	{
		// Cross product of the unit vector of the Z-axis, and the specific angular momentum
		return FVector(0.0, 0.0, 1.0).Cross(SpecificAngularMomentum);
	}*/
}
