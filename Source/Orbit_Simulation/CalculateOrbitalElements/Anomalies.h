#pragma once
#include "AstroConstants.h"
#include "Angles.h"

namespace EccentricAnomaly
{
	inline double CalculateFromTrueAnomaly(const double TrueAnomaly, const double Eccentricity)
	{
		const double EccentricAnomaly = FMath::Acos((Eccentricity + FMath::Cos(TrueAnomaly)) / (1 + (Eccentricity * FMath::Cos(TrueAnomaly))));
		return EccentricAnomaly;
	}
	
	/*###############################################################################################################*/
	/*
		* To find the position of an orbiting body at a given time, the Mean Anomaly must first be calculated from
		* the time. Then, we must solve Kepler's equation ( M = E - sin(E) ) to get the Eccentric Anomaly (E). We can then
		* easily calculate the radius and position from the Eccentric Anomaly.
		* However, solving for E has no closed-form solution, since sine is a transcendental function, it cannot
		* be solved algebraically.
		* The solution to E can be approximated by using Numerical Analysis, such as with Newton's Method.
		* At Higher eccentricities, the convergence slows down, and for hyperbolic trajectories, a different equation
		* must be used. 
	*/
	
	inline double NewtonsMethod(const double MeanAnomaly, const double Eccentricity)
	{
		// f(E) = E - sin(E)  - M(t)
		constexpr double Tolerance = 1e-6;
		constexpr int MaxIter = 10;
		
		double E = 0.0;
		if(Eccentricity > 0.8) // Set initial guess to E_0 = Pi
			E = PI;
		else // Set initial guess to E_0 = M(t)
			E = MeanAnomaly;

		// Iteratively apply Newton's Method until an arbitrary level of accuracy is reached
		for(int i = 0; i < MaxIter; i++)
		{
			const double Difference = ((Eccentricity * FMath::Sin(E)) - MeanAnomaly) / (1 - (Eccentricity * FMath::Cos(E)));
			E = E - Difference;

			if(Difference < Tolerance) break;
		}

		return E;
	}

	// Similar to Newton's Method, but iteratively substitutes M + sin(E) for E 
	inline double FixedPointIteration(const double MeanAnomaly, const double Eccentricity)
	{
		// f(E) = E - sin(E)  - M(t)
		constexpr int MaxIter = 10;
		
		double E = 0.0;
		if(Eccentricity > 0.8) // Set initial guess to E_0 = Pi
			E = PI;
		else // Set initial guess to E_0 = M(t)
			E = MeanAnomaly;

		for(int i = 0; i < MaxIter; i++)
		{
			E = MeanAnomaly + (Eccentricity * FMath::Sin(E));
		}

		return E;
	}


	/*###############################################################################################################*/
	
	inline double CalculateFromMeanAnomaly(const double MeanAnomaly, const double Eccentricity)
	{
		return NewtonsMethod(MeanAnomaly, Eccentricity);
	}

}
namespace MeanAnomaly
{
	inline double CalculateFromEccentricAnomaly(const double EccentricAnomaly, const double Eccentricity)
	{
		return EccentricAnomaly - FMath::Sin(EccentricAnomaly);
	}

	// If MeanAngularMotion is in degrees, function will return a value in degrees, if it is radians, then it will
	// return a value in radians
	inline double CalculateAtTime(const double TimeSincePeriapsis, const double MeanAngularMotion)
	{
		return AdjustAngle(MeanAngularMotion * TimeSincePeriapsis);
	}
	
	
	
}
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
