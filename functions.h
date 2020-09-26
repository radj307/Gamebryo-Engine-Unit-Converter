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

// helper for readability
enum class type { units, meters };

/* printResult(char, double, double) - Prints formatted conversion results to console window.
 * @param iType		- boolean that defines type of input unit (true = meters, false = units)
 * @param input		- Unconverted original value
 * @param result	- Converted result (toMeters() or toUnits())
 */
inline void printResult(type iType, d input, d result)
{
	switch (iType) {
	case type::meters: // output result in units
		std::cout << "RESULT:\t" << input << "m = " << result << "u\n\n";
		break;
	case type::units: // output result in (c)m
		std::cout << "RESULT:\t" << input << "u = " << result << "m";
		if (result < 1.0f) { // if result is less than 1 meter, output result in centimeters as well:
			result *= 100; // convert result from meters into centimeters
			std::cout << "\t( " << result << "cm )";
		}
		std::cout << "\n\n"; // format
		break;
	}
}

/* proc_user_input() - User input mode processor, prompts user through console for input values
 */
inline void proc_user_input()
{
	// program header
	std::cout << "\n--------------------------------------------------\nradj307's Unit-Meter Converter for Gamebryo Engine\n--------------------------------------------------\n\n";

	bool exit; // declare next loop exit
	do { // do once, then check while to continue loop
		exit = true; // define next loop exit
		// prompt user for input
		std::cout << "Select type of input value:\n\t1 - Convert U -> M\n\t2 - Convert M -> U\n\t3 - Exit Now\nuser> ";
		// receive & process user input
		int input = 0; // input unit type
		d number = 0.0f; // input value
		if (std::cin >> input) {
			if (input == 1 || input == 'u') {
				std::cout << "units> ";
				if (std::cin >> number) {
					printResult(type::units, number, toMeters(number));
				}
			}
			else if (input == 2 || input == 'm') {
				std::cout << "meters> ";
				if (std::cin >> number) {
					printResult(type::meters, number, toUnits(number));
				}
			}
			else if (input == 3 || input == 'q') {
				return;
			}
			std::cin.clear(); // clear cin failbit
			std::cin.ignore(10000, '\n'); // clear stream
		}
		else { // if cin>> didn't work, print error
			std::cout << "Invalid Input. <1|2>\n";
		}
		std::cout << "Continue? <Y/n>\nuser> ";
		if (std::cin >> input) {
			if (tolower(input) == 'y') {
				exit = false; // don't exit
				std::cin.clear(); // clear cin failbit
				std::cin.ignore(10000, '\n'); // clear stream
			}// else, exit				
		}
	} while (!exit);
}