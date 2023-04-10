#pragma once

namespace Stumpff
{
	inline double c0(const double X)
	{
		const double SqrtX = FMath::Sqrt(X);
		return FMath::Cos(SqrtX);
	}
	
	inline double c1(const double X)
	{
		const double SqrtX = FMath::Sqrt(X);
		return FMath::Sin(SqrtX) / SqrtX;
	}
	
	inline double c2(const double X)
	{
		const double SqrtX = FMath::Sqrt(X);
		return (1.0 - FMath::Cos(SqrtX)) / X;
	}
	
	inline double c3(const double X)
	{
		const double SqrtX = FMath::Sqrt(X);
		return (SqrtX - FMath::Sin(SqrtX)) / FMath::Sqrt(X * X * X);
	}
	
}