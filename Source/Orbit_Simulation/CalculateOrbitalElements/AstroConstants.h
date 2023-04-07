#pragma once


namespace Unit
{
	constexpr double Pi = 3.1415926535897932;
	constexpr double  AU = 1.495978707e11; // Astronomical Unit
	
	constexpr double GRAVITATIONAL_CONSTANT = 6.6743e-11; // m^3/kg/s^2
	constexpr double SOLAR_MASS = 1.989e30; // Mass of the sun in kg
	constexpr double G = 6.6743;
	constexpr double SM = 1.989;

	constexpr double MASS_MULT = 1e24;
	//constexpr double DISTANCE_MULT = 1e8; 1 in-editor unit = 10^8m
	constexpr double DISTANCE_MULT = 1e7;
	
	// Unit Conversion Multipliers
	constexpr double SECONDS_IN_DAY = 86400.0; // 1 second in simulation = 1 day
	constexpr double KM_TO_M = 1000.0;
	constexpr double M_TO_KM = 1.0 / 1000.0;
	
}

// Astronomical Bodies
// https://nssdc.gsfc.nasa.gov/planetary/planetfact.html
/*namespace Sun
{
	constexpr double Mass = 1988500.0; // x10^24 kg
	constexpr double Radius = 695700.0; // km
	constexpr double GM = 132712.0; // x10^6 km^3/s^2
	
}
namespace Mercury
{
	constexpr double Mass = 0.33010e24; // x10^24 kg
	constexpr double Radius = 2439.7; // km
	constexpr double GM = 0.022032; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = 579.09; // x10^5 km , 10^8 m
	
	constexpr double PeriapsisRadius = 460.0; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 58.98; // km/s
	
	constexpr double ApoapsisRadius = 698.18; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = 38.86; // km/s

	constexpr double Inclination = 7.004; // Degrees
	constexpr double Eccentricity = 0.2056; 
	constexpr double SiderealRotation = 0.2056; // hrs/ revolution
	constexpr double Period = 87.969; // Days (Sidereal Period)
	
	constexpr double LongitudeOfAscendingNode = 48.33167; // Degrees
	constexpr double LongitudeOfPeriapsis = 77.45645; // Degrees
	
}
namespace Venus
{
	constexpr double Mass = 4.8673; // x10^24 kg
	constexpr double Radius = 6051.8; // km
	constexpr double GM = 0.32486; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = 1082.10; // x10^5 km / 10^8 m
	
	constexpr double PeriapsisRadius = 1074.80; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 35.26; // km/s
	
	constexpr double ApoapsisRadius = 1089.41; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = 34.79; // km/s
	
	constexpr double Inclination = 3.395; // Degrees
	constexpr double Eccentricity = 0.00677323; 
	constexpr double SiderealRotation = -5832.6; // hrs/ revolution
	constexpr double Period = 224.701; // Days (Sidereal Period)
	
	constexpr double LongitudeOfAscendingNode = 76.68069; // Degrees
	constexpr double LongitudeOfPeriapsis = 131.53298; // Degrees
}
namespace Earth 
{
	constexpr double Mass = 5.9722; // x10^24 kg
	constexpr double Radius = 6371.0; // km
	constexpr double GM = 0.39860; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = 1495.98; // x10^5 km / 10^8 m
	
	constexpr double PeriapsisRadius = 1470.95; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 30.29; // km/s
	
	constexpr double ApoapsisRadius = 1521.0; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = 29.29; // km/s
	
	constexpr double Inclination = 0.0; // Degrees
	constexpr double Eccentricity = 0.01671022; 
	constexpr double SiderealRotation = 23.9345; // hrs/ revolution
	constexpr double Period = 365.256; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = -11.26064; // Degrees
	constexpr double LongitudeOfPeriapsis = 102.94719; // Degrees
}

namespace Mars
{
	constexpr double Mass = 0.64169; // x10^24 kg
	constexpr double Radius = 3398.5; // km
	constexpr double GM = 0.042828; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = 2279.56; // x10^5 km / 10^8 m
	
	constexpr double PeriapsisRadius = 2066.50; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 26.50; // km/s
	
	constexpr double ApoapsisRadius = 2492.61; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = 21.97; // km/s
	
	constexpr double Inclination = 1.85061; // Degrees
	constexpr double Eccentricity = 0.09341233; 
	constexpr double SiderealRotation = 24.6229; // hrs/ revolution
	constexpr double Period = 686.980; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = 49.57854; // Degrees
	constexpr double LongitudeOfPeriapsis = 336.04084; // Degrees
}

namespace Jupiter
{
	/*constexpr double Mass = ; // x10^24 kg
	constexpr double Radius = ; // km
	constexpr double GM = ; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = 7784.79; // x10^5 km / 10^8 m#1#

	constexpr double PeriapsisRadius = 7405.95; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 13.72; // km/s
	
	/*constexpr double ApoapsisRadius = ; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = ; // km/s

	constexpr double Inclination = ; // Degrees
	constexpr double Eccentricity = ; 
	constexpr double SiderealRotation = ; // hrs/ revolution
	constexpr double Period = ; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = ; // Degrees
	constexpr double LongitudeOfPeriapsis = 14.75385; // Degrees#1#
}

namespace Saturn
{
	/*constexpr double Mass = ; // x10^24 kg
	constexpr double Radius = ; // km
	constexpr double GM = ; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = ; // x10^5 km / 10^8 m#1#

	constexpr double PeriapsisRadius = 13575.54; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 10.18; // km/s
	
	/*constexpr double ApoapsisRadius = ; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = ; // km/s

	constexpr double Inclination = ; // Degrees
	constexpr double Eccentricity = ; 
	constexpr double SiderealRotation = ; // hrs/ revolution
	constexpr double Period = ; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = ; // Degrees
	constexpr double LongitudeOfPeriapsis = 92.43194; // Degrees#1#
}

namespace Uranus
{
	/*constexpr double Mass = ; // x10^24 kg
	constexpr double Radius = ; // km
	constexpr double GM = ; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = ; // x10^5 km / 10^8 m#1#

	constexpr double PeriapsisRadius = 27326.96; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 7.11; // km/s
	
	/*constexpr double ApoapsisRadius = ; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = ; // km/s

	constexpr double Inclination = ; // Degrees
	constexpr double Eccentricity = ; 
	constexpr double SiderealRotation = ; // hrs/ revolution
	constexpr double Period = ; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = ; // Degrees
	constexpr double LongitudeOfPeriapsis = 170.96424; // Degrees#1#
}

namespace Neptune
{
	/*constexpr double Mass = ; // x10^24 kg
	constexpr double Radius = ; // km
	constexpr double GM = ; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = ; // x10^5 km / 10^8 m#1#

	constexpr double PeriapsisRadius = 44710.50; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 5.50; // km/s
	
	/*constexpr double ApoapsisRadius = ; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = ; // km/s

	constexpr double Inclination = ; // Degrees
	constexpr double Eccentricity = ; 
	constexpr double SiderealRotation = ; // hrs/ revolution
	constexpr double Period = ; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = ; // Degrees
	constexpr double LongitudeOfPeriapsis = 44.97135; // Degrees#1#
}

namespace Pluto
{
	/*constexpr double Mass = ; // x10^24 kg
	constexpr double Radius = ; // km
	constexpr double GM = ; // x10^6 km^3/s^2
	
	constexpr double SemimajorAxis = ; // x10^5 km / 10^8 m#1#

	constexpr double PeriapsisRadius = 44349.87; // x10^5 km / 10^8 m
	constexpr double MaxOrbitalVelocity = 6.10; // km/s
	
	/*constexpr double ApoapsisRadius = ; // x10^5 km / 10^8 m
	constexpr double MinOrbitalVelocity = ; // km/s

	constexpr double Inclination = ; // Degrees
	constexpr double Eccentricity = ; 
	constexpr double SiderealRotation = ; // hrs/ revolution
	constexpr double Period = ; // Days (Sidereal Period)

	constexpr double LongitudeOfAscendingNode = ; // Degrees
	constexpr double LongitudeOfPeriapsis = ; // Degrees#1#
}*/