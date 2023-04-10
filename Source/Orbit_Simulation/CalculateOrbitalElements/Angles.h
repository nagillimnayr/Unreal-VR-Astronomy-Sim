#pragma once

// Adjust angle to be within range [0, 360) degrees
inline double AdjustAngle(double Angle)
{
	while(Angle > 360.0)
	{
		Angle -= 360.0;
	}
	while(Angle < 0.0)
	{
		Angle += 360.0;
	}

	return Angle;
}