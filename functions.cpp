/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include <iostream>
#include <conio.h>
#include "termcolor\termcolor.hpp"
#include "globals.h"
#include "fileIO.h"

 /* printHelp() - Outputs title card & parameters
  * Used if user-arguments are incorrect
  */
void printHelp()
{
	// print help, then flush cout buffer
	std::cout << " ARGUMENTS:\n\t<input unit> is the unit type you want to convert (Units|Meters|Feet)\n\t<value> is the value to convert (> 0.0)\n\t<output unit> is the converted unit type. (Units|Meters|Feet)\n\tOR\n\t<filepath> to convert a text file.\n" << std::endl;
	
	std::cout << termcolor::red << "\n\tPress any key to exit..." << termcolor::reset << std::endl;
	std::cout.flush();
	char tmp = _getch();
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

/* getType(string)
 * Converts a string into type
 * 
 * @param arg		- String to convert
 * @returns type	- type::error is returned if string does not match any type
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

/* getResult(Value, type) - Main conversion function
 * Used by printResult
 * @param input			- Input value
 * @param outputType	- Requested result type
 * @returns double		- Result value, or -0.0 if requested type matches input type.
 */
inline d getResult(type outputType, Value input)
{
	// check if requested type is the same as current type & return error code
	if (input._t != outputType) {
		// select conversion type and return the result
		switch (outputType) {
		case type::feet:
			return input.getFeet()._v;
			break;
		case type::meters:
			return input.getMeters()._v;
			break;
		case type::units:
			return input.getUnits()._v;
			break;
		default:break;
		}
	}
	return -0.0f; // undefined behavior returns -0.0f
}


/* (inline) printResult(Value, type) - Prints conversion results to console in standard notation. Uses getResult()
 * @param input		 - Original Value
 * @param outputType - Requested output unit
 * @returns void
 */
inline void printResult(Value input, type outputType)
{
	// define result
	Value result(outputType, getResult(outputType, input));

	// set decimal precision to 6 digits. (xEdit uses 6 digit precision)
	std::cout.precision(6);

	// output text block, and use fixed standard notation values for results only.
	std::cout << "\t" << termcolor::green << input._v << termcolor::reset << " " << input.sym() << "\t=  " << std::fixed << termcolor::green << result._v << termcolor::reset << " " << result.sym();

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
		default:break; // else break
		}
	}
	std::cout.unsetf(std::ios_base::floatfield); // unset std::fixed flag
	std::cout.flush(); // flush cout buffer
}

/**
 * formatResult(Value, Value)
 * Returns a human-readable string from input & output Values for text file output
 * 
 * @param in	- The input stored in a Value
 * @param out	- The output stored in a Value
 * @returns string
 */
inline std::string formatResult(Value in, Value out)
{
	if (out._v == -0) {
		return "\tERROR: Invalid Format";
	}

	std::stringstream rs;
	rs << "\t" <<  in._v <<  " " << in.sym() << "\t=  " << out._v << " " << out.sym();

	// if result is less than 1, output smaller units as well
	if (out._v < 1.0f && out._t != type::units) {
		switch (out._t) {
		case type::meters:
			out._v *= 100; // convert meters to centimeters
			rs << "  ( " << out._v << " cm )"; // output centimeters as well
			if (out._v < 1.0f) {
				out._v *= 10;
				rs << "  ( " << out._v << " mm )"; // output millimeters as well
				if (out._v < 1.0f) {
					out._v *= 1000;
					rs << "  ( " << out._v << " um )"; // output micrometers as well
				}
			}
			break;
		case type::feet:
			out._v *= 12; // convert feet to inches
			rs << "  ( " << out._v << " \" )"; // output inches as well
			break;
		default:break; // else break
		}
		rs << std::endl;
	}
	return rs.str();
}

/**
 * processFile(string)
 * Mass-converts a correctly formatted text file, then creates an output file to store the result.
 * 
 * @param filename			- The full filepath including file name & extension
 * @returns int to be returned by main()
 */
inline int processFile(std::string filename)
{
	// Get the contents of file separated by line
	std::vector<std::string> fileContent = fileRead(filename);

	// Create sstream to hold results, and a buffer for processing each line
	std::stringstream outputStream;

	// iterate through every line in the file
	for (auto it = fileContent.begin(); it != fileContent.end(); it++) {
		// use erase-remove idiom to remove all tab characters
		it->erase(std::remove(it->begin(), it->end(), '\t'), it->end());

		// split the line by spaces & store each word in a vector
		std::vector<std::string> words_on_line = splitLine(*it);

		// if the line has 3 arguments
		if (words_on_line.size() == 3) {
			try {
				// Create input & output values from arguments
				Value in(getType(words_on_line[0]), std::stod(words_on_line[1])), out(getType(words_on_line[2]), getResult(getType(words_on_line[2]), in));
				// Copy result string to stringstream
				outputStream << formatResult(in, out) + '\n';
			}
			catch (...) { // if an exception occurs because of invalid arguments, replace this line with error text
				outputStream << std::endl;
			}
		}
		else { // if the line does not have 3 arguments, replace it with empty line
			outputStream << std::endl;
		}
	}
	// write outputStream to file with -conv appended to the name. return inverted bool result for main() return. final param true forces standard notation.
	return !fileWrite(fileGetName(filename) + "-conv" + fileGetExtension(filename), outputStream);
}