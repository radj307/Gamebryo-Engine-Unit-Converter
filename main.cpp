/**
 * main.cpp
 * Contains main() logic
 * by radj307(tjradj)
 * 
 * Supported conversions as of v2.1:
 * Units<->Meters
 * Units<->Feet
 * Meters<->Feet
 */
// Timer:
//#define CHRONO
//#include <chrono>
#include "inline.cpp" // includes all headers

/**
 * main()
 * 
 * @param argc	- Number of arguments included in program call (file-path argument is always included)
 * @param argv	- Array of arguments included in program call
 * @returns int	- 1 for success, 3 for failure.
 */
int main(int argc, char* argv[]) // char * envp[] = {} // omit environment variables, program does not use them.
{
	std::ostringstream output;

	// set default flags
	bool
		success = false,			// defines if process was successful
		groupNumbers = false; // defines if large integrals will be grouped or not. set by including an extra argument 'r' after valid terminal or file conversion args.

	#ifdef CHRONO
		std::chrono::steady_clock::time_point tStart = std::chrono::steady_clock::now(); /// start time
	#endif

	// switch argument count
	switch (argc) {
	case 1:break; // break instantly if no arguments were included

	case 5:
		// Check if extra argument is the readable flag, if it is, set it
		if (tolower(*argv[4]) == 'r')
			groupNumbers = true;
		else output << help();
	case 4:
		// try to convert arguments to variables
		try {
			// Set input type and value
			Value in(getType(argv[1]), atof(argv[2]));

			// Set output type
			type out = getType(argv[3]);

			// Convert, and print results to terminal.
			if (printResult(in, out, groupNumbers))
				success = true;
			else
				output << "[ERROR]\tConversion failed" << help();
		}
		// catch all exceptions
		catch (...) {
			output << "[ERROR]\tConversion caused an exception" << help();
		}
		break; // break after terminal conversion

	case 3:
		// Check if extra argument is the readable flag, if it is, set it
		if (tolower(*argv[4]) == 'r')
			groupNumbers = true;
		else output << help();
	case 2:
		if (processFile(argv[1], groupNumbers)) 
			success = true;
		else 
			output << "[ERROR]\tCouldn't find file '" << argv[1] << "'" << std::endl;
		break; // break after file conversion

	default:break;
	}

	#ifdef CHRONO
		std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now(); /// end time
		output << "\n[LOG]\tSwitch completed after " << std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count() << "us\t( " << (tEnd - tStart).count() << "ns )\n";
	#endif

	// display output buffer
	std::cout << output.str();

	switch (success) {
	case true:
		return 0;
	case false:
		return 1;
	}
}