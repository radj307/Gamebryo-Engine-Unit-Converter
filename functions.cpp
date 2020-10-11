/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include <iostream>
#include <conio.h>
#include "termcolor\termcolor.hpp"
#include "globals.h"
#include "fileIO.h"

/**
 * printHelp(bool)
 * Displays argument syntax in the console.
 * Flushes the cout buffer before & after printing.
 * 
 * @param pause	- If true the function will wait for user to press a key before returning.
 */
void printHelp(bool pause = false)
{
	// flush console output buffer
	std::cout.flush();
	// display help text
	std::cout << " ARGUMENTS:\n\t<input unit> is the unit type you want to convert (Units|Meters|Feet)\n\t<value> is the value to convert (> 0.0)\n\t<output unit> is the converted unit type. (Units|Meters|Feet)\n\tOR\n\t<filepath> to convert a text file.\n\n";
	// check if pause is true before proceeding.
	if (!pause)
		return;
	// prompt user to press a key before exiting.
	std::cout << termcolor::red << "\tPress any key to exit...\n" << termcolor::reset;
	// flush console output buffer
	std::cout.flush();
	// wait for user to press a key
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

/** 
 * getType(string)
 * Converts a string into class type
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

/** 
 * getResult(Value, type)
 * Converts parameters & returns the result as a double.
 * 
 * @param input			- Input value
 * @param outputType	- Requested result type
 * @returns double		- Result value, or -0.0 if error.
 */
inline d getResult(type outputType, Value input)
{
	// make sure input type & output type aren't the same
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
	// return error code:
	return -0.0f;
}

/**
 * formatResult(Value, Value)
 * Returns a human-readable string from input & output Values
 * 
 * @param in	- The input stored in a Value
 * @param out	- The output stored in a Value
 * @returns string
 */
inline std::string formatResult(Value in, Value out, bool endWithNewLine = true)
{
	if (out._v == -0)
		return "";

	std::stringstream rs; // result-stream

	// add primary conversion to stream
	rs << "\t" <<  in._v <<  " " << in.sym() << "\t=  " << out._v << " " << out.sym();

	// if result val is less than 1 & not in units, add smaller unit types for context
	if (out._v < 1.0f && out._t != type::units) {
		switch (out._t) {
		case type::meters: // metric
			out._v *= 100; // convert meters to centimeters
			rs << "  ( " << out._v << " cm )";
			if (out._v < 1.0f) {
				out._v *= 10; // convert centimeters to millimeters
				rs << "  ( " << out._v << " mm )";
				if (out._v < 1.0f) {
					out._v *= 1000; // convert millimeters to micrometers
					rs << "  ( " << out._v << " um )";
				}
			}
			break;
		case type::feet: // imperial
			out._v *= 12; // convert feet to inches
			rs << "  ( " << out._v << " \" )"; // output inches as well
			break;
		default:break; // undefined break
		}
	}
	if (endWithNewLine)
		rs << '\n';

	return rs.str();
}

/**
 * printResult(Value, type)
 * Displays conversion and results in console window in human-readable format, with color.
 * Does not use formatResult as it would take more time to convert back for colorized output
 *
 * @param input		 - Original Value
 * @param outputType - Requested output unit
 * @returns int 0 = success, 1 = error
 */
inline int printResult(Value input, type outputType)
{
	// define result
	Value result(outputType, getResult(outputType, input));

	if (result._v == -0.0)
		return 1; // return an error if invalid

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
	return 0;
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
				outputStream << formatResult(in, out);
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