/* Simple program to convert Gamebryo engine "units" to and from meters.
 * Made by radj307
 */
#include "functions.h" // includes "globals.h"

/* main()
 * @argc	- Number of arguments included in program call (filepath argument is always included)
 * @argv	- Array of arguments included in program call
 */
int main(int argc = 0, char* argv[] = nullptr) // char * envp[] = {} // omit environment variables, program does not use them.
{
	if (argc > 3) { // if a valid amount of user arguments are detected (3)
		Value arg(getType(argv[1]), atof(argv[2]));
		type out = getType(argv[3]);

		switch (out) {
		case type::feet:
			printResult(arg, arg.getFeet());
			break;
		case type::meters:
			printResult(arg, arg.getMeters());
			break;
		case type::units:
			printResult(arg, arg.getUnits());
			break;
		default:
			std::cout << "Invalid Arguments.\n\tValid Arguments:\nArg1 = Units | Meters | Feet\nArg2 = Any number\nArg3 = Units | Meters | Feet\n\n";
			break;
		}

		return 0; // return 0 once args have been processed
	}
	return 1; // return 1 if program ends for undefined reason
}