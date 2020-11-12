/**
 * main.cpp
 * Contains main()
 * by radj307
 */

//#define WSL // Disable termcolor when using Windows Subsystem for Linux

#include "termcolor.hpp"
#include "INI.h"					// For INI settings
INI cfg("conv.ini");				// init INI
#include "argument_processor.hpp"	// For all functions

/** MAIN **
 * main(const int, const char*[])
 * 
 * @param argc	- The number of indexes in argv[]
 * @param argv	- List of commandline arguments given when calling the program.
 */
int main(const int argc, const char* argv[])
{
	// call the argument interpreter from argument_processor.hpp and return the result
	return interpret(argc, argv);
}