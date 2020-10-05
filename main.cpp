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
 * @argc	- Number of arguments included in program call (filepath argument is always included)
 * @argv	- Array of arguments included in program call
 */
int main(int argc = 0, char* argv[] = nullptr) // char * envp[] = {} // omit environment variables, program does not use them.
{
	if (argc == 4) {
		// Define values & types from arguments:
		Value in(getType(argv[1]), atof(argv[2]));
		type out = getType(argv[3]);

		printResult(in, out);
	}
	else
		printHelp();
	return 0;
}