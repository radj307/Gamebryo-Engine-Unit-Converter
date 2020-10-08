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
int main(int argc, char* argv[]) // char * envp[] = {} // omit environment variables, program does not use them.
{
	if (argc == 4) {
		try {
			// Define values & types from arguments:
			Value in(getType(argv[1]), atof(argv[2]));
			type out = getType(argv[3]);
			printResult(in, out);
		}
		catch (...) {
			printHelp();
		}

	}
	else if (argc == 2) {
		std::string filepath = "t.txt";
		// get file content
		std::vector<std::string> fileContent = fileRead(filepath);
		std::stringstream ss, lineBuffer;
		for (auto it = fileContent.begin(); it != fileContent.end(); it++) {
			if (splitLine(*it).size() == 3) {
				// copy line to lineBuffer
				lineBuffer << *it;
				// create argument array
				std::string argv_line[3] = { "", "", "" };
				lineBuffer >> argv_line[0];
				lineBuffer >> argv_line[1];
				lineBuffer >> argv_line[2];
					
				// create values
				try {
					Value in(getType(argv_line[0]), std::stod(argv_line[1])), out(getType(argv_line[2]), getResult(getType(argv_line[2]), in));
					// push result string to ss
					ss << formatResult(in, out) + '\n';
				}
				catch (...) {
					ss << "\tERROR: Line Invalid" << std::endl;
				}

				// clear the lineBuffer for next loop
				lineBuffer.clear();
			}
			else {
				ss << "\tERROR: Line Invalid" << std::endl;
			}
		}
		fileWrite(fileRemoveExtension(filepath) + "-conv.txt", ss);
		std::cout << termcolor::green << "\tProcess is complete." << termcolor::reset << std::endl;
	}
	else
		printHelp();
	return 0;
}