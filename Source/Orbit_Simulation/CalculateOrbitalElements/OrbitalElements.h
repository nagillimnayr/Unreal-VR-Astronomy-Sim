#pragma once
#include "AstroConstants.h"
#include "Angles.h"

#include "Anomalies.h"
#include "Apsides.h"
#include "ArgumentOfPeriapsis.h"
#include "AscendingNode.h"
#include "Axes.h"
#include "Eccentricity.h"
#include "Inclination.h"
#include "OrbitalPeriod.h"
#include "OrbitalRadius.h"
#include "OrbitalVelocity.h"
#include "Position.h"
#include "SpecificAngularMomentum.h"
#include "SpecificOrbitalEnergy.h"
#include "Stumpff.h"

UENUM(BlueprintType)
enum EOrbitPreset
{
	None,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune
};

const TMap<EOrbitPreset, FString> PresetNames = {
	{None, "None"},
	{Mercury, "Mercury"},
	{Venus, "Venus"},
	{Earth, "Earth"},
	{Mars, "Mars"},
	{Jupiter, "Jupiter"},
	{Saturn, "Saturn"},
	{Uranus, "Uranus"},
	{Neptune, "Neptune"},
};