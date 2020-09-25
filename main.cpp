/* Simple program to convert Gamebryo engine "units" to and from meters.
 * Made by radj307
 */
#include "user_input.h" // includes "globals.h" & "functions.h"
//#include <limits> // for extreme cin error checking, replace "cin.ignore(10000, '\n')" with "cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')"

/* main()
 * @argc	- Number of arguments included in program call (filepath argument is always included)
 * @argv	- Array of arguments included in program call
 * Valid arguments:
 * @arg 1	- Valid inputs are: "unit", & "meter" for conversion (requires arg 2)
 *		 OR - "-u" for user input mode (requires ONLY arg 1)
 * @arg 2	- Valid inputs are: Any value in meters or units
 */
int main(int argc = 0, char* argv[] = {}) // char * envp[] = {} // omit environment variables, program does not use them.
{
	if (argc > 2) { // if a valid amount of user arguments are detected:
		//std::string arg_path = argv[0] // convert arg 0 to string, defines root directory. (not used)
		std::string arg_unit = argv[1];  // convert arg 1 to string, defines unit type
		d arg_number = atof(argv[2]);	 // convert arg 2 to double, defines unit value
		// set all chars in string arg_unit to lowercase
		for (unsigned int i = 0; i < arg_unit.size(); i++) {
			arg_unit[i] = tolower(arg_unit[i]);
		}
		// check unit type arg
		if (arg_unit == "unit" || arg_unit == "units" || arg_unit == "u") {
			printResult(type::units, arg_number, toMeters(arg_number));
		}
		else if (arg_unit == "meter" || arg_unit == "meters" || arg_unit == "m") {
			printResult(type::meters, arg_number, toUnits(arg_number));
		}
		else { // if unit type is invalid
			std::cout << "'" << arg_unit << "' is not a valid unit type.\n\n";
			return 1; // return 1 if args could not be processed because of invalid format
		}
		return 0; // return 0 once args have been processed
	} // if user args are not sufficient for instant conversion:
	else if (argc == 2) { // if only 1 user arg is detected, check:
		if (argv[1] == "-u") { // if single argument matches expected arg to process user input
			proc_user_input(); // process user input
			return 0; // return 0 once user input has finished
		}
	}
	return 1; // return 1 if program ends for undefined reason
}