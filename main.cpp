/**
 * main.cpp
 * Execution begins here.
 * by radj307(tjradj)
 * 
 * Supported conversions as of v2.1:
 * Units<->Meters
 * Units<->Feet
 * Meters<->Feet
 */
#include "functions.cpp" // includes "globals.h" & "fileIO.h"

/**
 * main()
 * 
 * @param argc	- Number of arguments included in program call (file-path argument is always included)
 * @param argv	- Array of arguments included in program call
 * @returns int	- 1 for success, 3 for failure.
 */
int main(int argc, char* argv[]) // char * envp[] = {} // omit environment variables, program does not use them.
{
	bool flag_readable = false;
	switch (argc) { // argc will always be at least 1 because the filepath is included as first argument
	case 5: // user included 4 arguments
		// check for readable flag
		if (*argv[4] == 'r') {
			flag_readable = true;
		}
	case 4: // user included 3 arguments
		try { // try to convert arguments to valid variables
			Value in(getType(argv[1]), atof(argv[2]));
			type out = getType(argv[3]);
			// print result & pass return to main
			return printResult(in, out, flag_readable);
		}
		catch (...) { // if arguments caused an exception
			std::cout << termcolor::red << "\tCouldn't process arguments!\n" << termcolor::reset;
			// display help and do not wait to return - user is already in terminal, or is using a shortcut with arguments
			printHelp(0);
		}
		// break and return 1 for error
		break;
<<<<<<< Updated upstream
	case 2: // user included a single argument
		if (processFile(argv[1])) { // if successful
=======
	case 3: // user included 2 arguments
		// check for readable flag
		if (*argv[4] == 'r') {
			flag_readable = true;
		}
	case 2: // user included 1 argument
		if (processFile(argv[1], flag_readable)) { // if successful
>>>>>>> Stashed changes
			std::cout << termcolor::green << "\tDone processing file." << termcolor::reset << std::endl;
			return success();
		} // else print help
		std::cout << termcolor::red << "\tCouldn't open file '" << argv[1] << "' for reading" << termcolor::reset << std::endl;
		printHelp();
		return success();
	default:
		std::cout << termcolor::red << "\tInvalid Arguments\n" << termcolor::reset;
		printHelp(1);
		break;
	}
	return fail();
}