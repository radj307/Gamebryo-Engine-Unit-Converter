/* functions.h contains the processing logic for unit conversions used in main.cpp
 */
#pragma once
#include <iostream>
#include <conio.h>
#include "termcolor\termcolor.hpp"
#include "globals.h"
#include "fileIO.h"

/**
 * struct NumberGrouping
 * Used to format large numbers in locale, uses the <xlocale> library.
 * Called by using stream imbue(locale(cout.getloc(), new NumberGrouping))
 */
struct NumberGrouping : std::numpunct<char>
{
	std::string do_grouping() const { return "\3"; }
};

/**
 * groupNumber(double)
 * Returns a string with commas inserted in the integral, every 3 digits.
 * 
 * @param val		- Floating-point value
 * @return string	- Param val as string with commas inserted in the integral to make values human-readable.
 */
std::string groupNumber(d val)
{
	// create stringstream
	std::stringstream ss;

	// imbue stringstream with number grouping
	ss.imbue(std::locale(std::cout.getloc(), new NumberGrouping));

	// pass value to stringstream
	ss << val;

	// return ss as a string
	return ss.str();
}

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
	std::cout << termcolor::red << "\tPress any key to continue...\n" << termcolor::reset;
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
	// check if the output value calculation returned an error code, if so just return a blank string
	if (out._v == -0)
		return "";

	// create a stringstream for results
	std::stringstream rs;

	// pass the main conversion results with formatting to stream
	rs << "\t" <<  in._v <<  " " << in.sym() << "\t=  " << out._v << " " << out.sym();
	
	// determine output type using switch table
	switch (out._t) {
	case type::meters: // metric
		if (out._v < 1.0f) {
			out._v *= 100; // convert meters to centimeters
			rs << "  ( " << out._v << " cm )";
			// check if result val is still less than 1
			if (out._v < 1.0f) {
				out._v *= 10; // convert centimeters to millimeters
				rs << "  ( " << out._v << " mm )";
				// check if result val is still less than 1
				if (out._v < 1.0f) {
					out._v *= 1000; // convert millimeters to micrometers
					rs << "  ( " << out._v << " um )";
				}
			}
		}
		break; // breaks if number is greater than 1
	case type::feet: // imperial
		if (out._v < 1.0f) {
			out._v *= 12; // convert feet to inches
			rs << "  ( " << out._v << " \" )"; // output inches as well
		}
		break; // breaks if number is greater than 1
	default:break; // breaks if output type is units
	}

	// appends a newline character to the stream
	if (endWithNewLine)
		rs << std::endl;

	// return the result stream
	return rs.str();
}

/**
 * printResult(Value, type)
 * Displays conversion and results in console window in human-readable format, with color.
 * Does not use formatResult as it would take more time to convert back for colorized output
 *
 * @param input		 - Original Value
 * @param outputType - Requested output unit
 * @returns bool	 - ( true = success ) ( false = fail )
 */
inline bool printResult(Value input, type outputType, bool grouping = false)
{
	// define result
	Value result(outputType, getResult(outputType, input));

	// check if result calculation returned an error code, and if so exit the program with error code
	if (result._v == -0.0)
		return false;

	// set decimal precision to 6 digits. (xEdit uses 6 digit precision)
	std::cout.precision(6);

	switch (grouping) {
	case true:
		// imbue stream with number grouping
		std::cout.imbue(std::locale(std::cout.getloc(), new NumberGrouping));
		// continue
	case false:
		// output text block, and use fixed standard notation values for results only.
		std::cout << "\t" << termcolor::green << input._v << termcolor::reset << " " << input.sym() << "     \t=  " << std::fixed << termcolor::green << result._v << termcolor::reset << " " << result.sym();

		// if result is less than 1, output smaller units as well
		if (result._v < 1.0f && result._t != type::units) {
			switch (result._t) {
			case type::meters:
				result._v *= 100; // convert meters to centimeters
				std::cout << "  = " << termcolor::green << result._v << termcolor::reset << " cm"; // output centimeters as well
				// check if result val is still less than 1
				if (result._v < 1.0f) {
					result._v *= 10;
					std::cout << "  = " << termcolor::green << result._v << termcolor::reset << " mm"; // output millimeters as well
					// check if result val is still less than 1
					if (result._v < 1.0f) {
						result._v *= 1000;
						std::cout << "  = " << termcolor::green << result._v << termcolor::reset << " um"; // output micrometers as well
						// check if result val is still less than 1
						if (result._v < 1.0f) {
							result._v *= 1000;
							std::cout << "  = " << termcolor::green << result._v << termcolor::reset << " nm"; // output nanometers as well
						}
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
		std::cout.unsetf(std::ios_base::floatfield); // unset cout flags
		std::cout.flush(); // flush cout buffer
		return true; // return success code
	}
	return false;
}

/**
 * processFile(string)
 * Mass-converts a correctly formatted text file, then creates an output file to store the result.
 * 
 * @param filename	- The full filepath including file name & extension
 * @returns bool	- ( true = success ) ( false = fail )
 * @param grouping	- (OPTIONAL) Groups large integral values by 3. (localization-dependant)
 */
inline bool processFile(std::string filename, bool grouping = false)
{
	// Get the contents of file separated by line
	std::vector<std::string> fileContent = fileRead(filename);

	// If fileRead returned an error (file doesn't exist), return false
	if (fileContent.empty())
		return false;

	// Create sstream to hold output
	std::stringstream outputStream;

	// check if the make readable flag is on, if it is, imbue the stream with number grouping.
	outputStream.imbue(std::locale(std::cout.getloc(), new NumberGrouping));

	// iterate through every line in the file
	for (auto it = fileContent.begin(); it != fileContent.end(); it++) {
		// use erase-remove idiom to remove all tab characters
		it->erase(std::remove(it->begin(), it->end(), '\t'), it->end());

		if (it->size() > 0) {
			// split the line by spaces & store each word in a vector
			std::vector<std::string> words_on_line = splitString(*it);

			// if the line has 3 arguments
			if (words_on_line.size() == 3) {
				try {
					// Create input & output values from arguments
					Value in(getType(words_on_line[0]), std::stod(words_on_line[1])), out(getType(words_on_line[2]), getResult(getType(words_on_line[2]), in));
					// Copy result string to stringstream
					outputStream << formatResult(in, out);
				}
				catch (...) { // if an exception occurs because of invalid arguments, replace this line with error text
					outputStream << "\tinvalid arguments\n";
				}
			}
			else { // if the line does not have 3 arguments, replace it with empty line
				outputStream << "\n";
			}
		}		
	}
	// write outputStream to file with -conv appended to the name, then return success/fail val from filewrite
	return fileWrite(fileExtendName(filename, "-conv"), outputStream);
}