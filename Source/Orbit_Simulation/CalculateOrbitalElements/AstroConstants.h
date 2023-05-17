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

	constexpr double EARTH_RADIUS = 6371.0084e3;
}

