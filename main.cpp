/* Simple commandline program to convert between units from the Gamebryo Engine and real-life units
 * Made by radj307
 * 
 * Supported conversions as of v2.1:
 * Units<->Meters
 * Units<->Feet
 * Meters<->Feet
 */
#include "functions.cpp" // includes "globals.h"

/* main()
 * @argc	- Number of arguments included in program call (file-path argument is always included)
 * @argv	- Array of arguments included in program call
 */
int main(int argc, char* argv[]) // char * envp[] = {} // omit environment variables, program does not use them.
{
	// argument count is valid for commandline conversion
	if (argc == 4) {
		try {
			// Define values & types from arguments:
			Value in(getType(argv[1]), atof(argv[2]));
			type out = getType(argv[3]);
			// print the result to the console
			printResult(in, out);
			// set return value to success (0)
			return 0;
		}
		catch (...) { // catch all exceptions, print help
			printHelp();
		}
	}
	// argument count is valid for file conversion
	else if (argc == 2) {
		// set return val & process file
		return processFile(argv[1]);
	}
	else {
		printHelp();
	}
	return 1;
}