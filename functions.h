/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include "globals.h"

// helper for readability
enum class type { units = false, meters = true, };

/* printResult(char, double, double) - Prints formatted conversion results to console window.
 * @param iType		- boolean that defines type of input unit (true = meters, false = units)
 * @param input		- Unconverted original value
 * @param result	- Converted result (toMeters() or toUnits())
 */
inline void printResult(type iType, d input, d result)
{
	switch (iType) {
	case type::units:
		std::cout << "RESULT:\t" << input << "u = " << result << "m\n\n";
		break;
	case type::meters:
		std::cout << "RESULT:\t" << input << "m = " << result << "u\n\n";
		break;
	}
}

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