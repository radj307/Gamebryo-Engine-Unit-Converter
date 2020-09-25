/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include "globals.h"

/* toMeters(double)	- Convert units into meters
 * @returns double	- Meters Output
 * @param units		- Units Input
 */
inline d toMeters(d units)
{
	// meters = convRatio * units
	return __cv * units;
}

/* toUnits(double)	- Convert meters into units
 * @returns double	- Units Output
 * @param units		- Meters Input
 */
inline d toUnits(d meters)
{
	// units = meters / convRatio
	return meters / __cv;
}