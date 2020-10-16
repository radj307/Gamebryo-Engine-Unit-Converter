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
#include "functions.cpp" // includes all headers

/**
 * main()
 * 
 * @param argc	- Number of arguments included in program call (file-path argument is always included)
 * @param argv	- Array of arguments included in program call
 * @returns int	- 1 for success, 3 for failure.
 */
int main(int argc, char* argv[]) // char * envp[] = {} // omit environment variables, program does not use them.
{
	// set human-readable flag to false
	bool flag_readable = false, pauseTerminal = false;
	switch (argc) { // argc will always be at least 1 because the filepath is included as first argument
	case 1: // no arguments, assume user is not using a terminal
		pauseTerminal = true;
		break;
	case 5: // user included 4 arguments
		// check for readable flag
		if (*argv[4] == 'r')
			flag_readable = true;
	case 4: // user included 3 arguments
		try { // try to convert arguments to valid variables
			Value in(getType(argv[1]), atof(argv[2]));
			type out = getType(argv[3]);
			// print result & pass return to main
			if (printResult(in, out, flag_readable)) {
				return 0;
			}
		}
		catch (...) {
			std::cout << termcolor::red << "\tSomething went wrong\n" << termcolor::reset;
		}
	case 3: // user included 2 args
		// check for readable flag
		if (*argv[4] == 'r')
			flag_readable = true;
	case 2: // user included a single argument
		if (processFile(argv[1])) {
			return 0;
		}
	default:
		// display invalid warning
		std::cout << termcolor::red << "\tInvalid Arguments\n" << termcolor::reset;
		printHelp(pauseTerminal);
		break;
	}
	return 1;
}