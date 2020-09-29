/* globals.h contains all the global data used throughout the program
 **/
#pragma once
#include <iostream>

// shortcut for double type
typedef double d; 

// define colorized console output in ANSI escape sequences:
static const std::string	cl = "",//"\x1B[92m", // Set console color. (ANSI 92= Bright Green)
							rs = "";//"\x1B[37m"; // Reset console color. (ANSI 37= White)

/* units <-> meters
 * METERS	= units * ratio_um
 * UNITS	= meters / ratio_um
 */
static const d __ratio_um = 0.01428222656;

/* meters <-> feet
 * METERS	= feet / ratio_mi
 * FEET		= meters * ratio_mi
 */
static const d __ratio_mi = 3.28086325;

/* units <-> feet
 * FEET		= units * ratio_ui
 * UNITS	= feet / ratio_ui
 */
static const d __ratio_ui = 0.046875;

/* enum type - Defines display character for measurement units
 * @type::error	 - !
 * @type::units	 - u
 * @type::meters - m
 * @type::feet	 - '
 * Note that cm & inches are not included as they are not valid input types.
 */
enum class type { error = '!', units = 'u', meters = 'm', feet = char(39) };

/* Value() - Simple wrapper struct that holds a value and it's type.
 */
struct Value {
	// The measurement unit. (Use sym() to display)
	type _t;
	// The value in measurement units
	d _v;

	/* Value(type, double) - CONSTRUCTOR
	 * @param type	 = The measurement unit of this value
	 * @param double = The value
	 */
	Value(type TypeOfValue, d Value) : _t(TypeOfValue), _v(Value) {}

	// converts type to char and returns it
	inline char sym()
	{
		return char(_t);
	}

	// Return stored value in meters
	inline Value getMeters();

	// Return stored value in units
	inline Value getUnits();

	// Return stored value in feet
	inline Value getFeet();
};