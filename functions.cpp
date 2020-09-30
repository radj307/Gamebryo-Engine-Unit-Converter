/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include <iostream>
#include "globals.h"

 /* printHelp() - Outputs titlecard & parameters
  * Used if user-arguments are incorrect
  */
void printHelp()
{
	// print help, then flush cout buffer
	std::cout << " ARGUMENTS:\n\t<input unit> is the unit type you want to convert (Units|Meters|Feet)\n\t<value> is the value to convert (> 0.0)\n\t<output unit> is the converted unit type. (Units|Meters|Feet)" << std::endl;
}

// Member function of Value: Returns stored value in meters
inline Value Value::getMeters()
{
	// switch through input types:
	switch (_t) 
	{
	case type::meters: // if the desired type is the same as the type, return unmodified value
		return Value(type::meters, _v);
	case type::units: // units -> meters
		return Value(type::meters, _v * __ratio_um);
	case type::feet: // feet -> meters
		return Value(type::meters, _v / __ratio_mi);
	default:
		return Value(type::error, -0.0f);
	}
}
// Member function of Value: Returns stored value in units
inline Value Value::getUnits()
{
	// switch through input types:
	switch (_t) 
	{
	case type::units: // if the desired type is the same as the type, return unmodified value
		return Value(type::units, _v);
	case type::meters: // meters -> units
		return Value(type::units, _v / __ratio_um);
	case type::feet: // feet -> units
		return Value(type::units, _v / __ratio_ui);
	default:
		return Value(type::error, -0.0f);
	}
}
// Member function of Value: Returns stored value in feet
inline Value Value::getFeet()
{
	// switch through input types:
	switch (_t) 
	{
	case type::feet: // if the desired type is the same as the type, return unmodified value
		return Value(type::feet, _v);
	case type::meters: // meters -> feet
		return Value(type::feet, _v * __ratio_mi);
	case type::units: // units -> feet
		return Value(type::feet, _v * __ratio_ui);
	default:
		return Value(type::error, -0.0f);
	}
}

/* getType(string)	- Converts a string into type
 * @param arg		- String to convert
 */
inline type getType(std::string arg)
{
	// convert string to lowercase:
	for (unsigned int i = 0; i < arg.size(); i++) {
		arg[i] = tolower(arg[i]);
	}
	// process string:
	if (arg == "unit" || arg == "units" || arg == "u") {
		return type::units;
	}
	else if (arg == "meter" || arg == "meters" || arg == "m") {
		return type::meters;
	}
	else if (arg == "foot" || arg == "feet" || arg == "ft" || arg == "f" || arg == "i") {
		return type::feet;
	}
	return type::error;
}

/* (inline) printResult(Value, Value, unsigned int) - Prints conversion results to console in standard notation
 * @param input		- Original Value
 * @param result	- Converted Value
 * @param dec = 8	- Set decimal precision (default is 8)
 */
inline void printResult(Value input, Value result)
{
	// force standard notation
	std::cout << std::fixed;

	// round input to whole number and compare to original value
	if (std::floor(input._v) == input._v)
		std::cout.precision(0); // don't print decimal values if whole number

	// output input value block
	std::cout << "\t" << cl << input._v << rs  << input.sym() << " = ";

	// reset precision for next value
	std::cout.precision(6);

	// round result to whole number and compare to original value
	if (std::floor(result._v) == result._v)
		std::cout.precision(0); // don't print decimal values if whole number

	// output result value block
	std::cout << cl << result._v << rs << result.sym();

	// reset precision for next value
	std::cout.precision(6);
	
	// if result is less than 1, output smaller units as well
	if (result._v < 1.0f && result._t != type::units) { 
		switch (result._t) {
		case type::meters:
			result._v *= 100; // convert meters to centimeters
			std::cout << "\t( " << cl << result._v << rs << "cm )"; // output centimeters as well
			break;
		case type::feet:
			result._v *= 12; // convert feet to inches
			std::cout << "\t( " << cl << result._v << rs << "\" )"; // output inches as well
			break;
		default:break;
		}
	}
	std::cout.flush(); // flush cout buffer
}