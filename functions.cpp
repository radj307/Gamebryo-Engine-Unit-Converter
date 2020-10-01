/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include <iostream>
#include <string>
#include "termcolor\termcolor.hpp"
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
 */
inline void printResult(Value input, Value result)
{
	// force standard notation to precision of 6 digits
	std::cout << std::fixed;
	std::cout.precision(6);

	// output input value block
	std::cout << "\t" << termcolor::green << input._v << termcolor::reset << " " << input.sym() << "\t=  " << termcolor::green << result._v << termcolor::reset << " " << result.sym();
	
	// if result is less than 1, output smaller units as well
	if (result._v < 1.0f && result._t != type::units) { 
		switch (result._t) {
		case type::meters:
			result._v *= 100; // convert meters to centimeters
			std::cout << "  ( " << termcolor::green << result._v << termcolor::reset << " cm )"; // output centimeters as well
			if (result._v < 1.0f) {
				result._v *= 10;
				std::cout << "  ( " << termcolor::green << result._v << termcolor::reset << " mm )"; // output millimeters as well
				if (result._v < 1.0f) {
					result._v *= 1000;
					std::cout << "  ( " << termcolor::green << result._v << termcolor::reset << " um )"; // output micrometers as well
				}
			}
			break;
		case type::feet:
			result._v *= 12; // convert feet to inches
			std::cout << "  ( " << termcolor::green << result._v << termcolor::reset << " \" )"; // output inches as well
			break;
		default:break;
		}
	}
	std::cout.flush(); // flush cout buffer
}