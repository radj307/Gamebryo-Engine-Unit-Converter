/**
 * globals.h
 * Contains all the global data used throughout the program
 * by radj307(tjradj)
 */
#pragma once

 /// GLOBAL VARIABLES AND TYPES

// shortcut for double type
typedef double d;

/* units <-> meters conversion ratio
 * METERS	= units * ratio_um
 * UNITS	= meters / ratio_um
 */
static const d __ratio_um = 0.01428222656;

/* meters <-> feet conversion ratio
 * METERS	= feet / ratio_mi
 * FEET		= meters * ratio_mi
 */
static const d __ratio_mi = 3.281;

/* units <-> feet conversion ratio
 * FEET		= units * ratio_ui
 * UNITS	= feet / ratio_ui
 */
static const d __ratio_ui = 0.046875;

/**
 * enum type - Defines primary display character for measurement units
 * @type::error	 -  !
 * @type::units	 -  u
 * @type::meters -  m
 * @type::feet	 -  '
 */
enum class type 
{ 
	error = '!', 
	units = 'u', 
	meters = 'm', 
	feet = char(39) 
};


/// STRUCT DEFS

/** 
 * struct Value
 * Holds a single value and its type
 */
struct Value {
	// The measurement unit. (Use sym() to display)
	type _t;

	// The value in measurement units
	d _v;

	/**
	 * CONSTRUCTOR - Value(type, double)
	 * @param type	 = Valid unit type
	 * @param double = Value
	 */
	Value(type TypeOfValue, d Value) : _t(TypeOfValue), _v(Value) {}

	/**
	 * char sym()
	 * Returns the unit type as a char, used in output functions
	 */
	inline char sym()
	{
		return static_cast<char>(_t);
	}

	// Return a Value with stored value in meters
	inline Value getMeters();

	// Return a Value with stored value in units
	inline Value getUnits();

	// Return a Value with stored value in feet
	inline Value getFeet();
};

/**
 * struct NumberGrouping
 * Used to group large integrals by 3
 *
 * @see groupNumber()
 */
struct NumberGrouping : std::numpunct<char>
{
	std::string do_grouping() const { return "\3"; }
};