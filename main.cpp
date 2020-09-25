/* Simple program to convert Gamebryo engine "units" to and from meters.
 * Made by radj307
 */
#include "functions.h" // includes "globals.h"
#include <conio.h> // for getch() & clrscr()
#include <cmath> // for round
//#include <limits> // for extreme cin error checking, replace "cin.ignore(10000, '\n')" with "cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')"

/* main()
 * @argc	- Number of arguments included in program call
 * @argv	- Array of arguments included in program call
 * @envp	- Environment variables of host
 */
int main(int argc = 0, char* argv[] = {}) // omit environment variables, not used.
{
	if (argc > 2) { // if args are correct
		// argv[1] = file path
		std::string arg_unit = argv[1];
		d arg_number = atof(argv[2]);
		// set all chars in string arg_unit to lowercase
		for (unsigned int i = 0; i < arg_unit.size(); i++) {
			arg_unit[i] = tolower(arg_unit[i]);
		}
		// check unit type
		if (arg_unit == "unit" || arg_unit == "units") {
			std::cout << arg_number << "u = " << toMeters(arg_number) << "m\n\n";
		}
		else if (arg_unit == "meter" || arg_unit == "meters") {
			std::cout << arg_number << "m = " << toUnits(arg_number) << "u\n\n";
		}
		// if unit type is invalid
		else {
			std::cout << "Could not understand '" << arg_unit << "'\n\n";
		}
	}
	else { // if args are not included, fallback to user input method
		// program header
		std::cout << "\n--------------------------------------------------\nradj307's Unit-Meter Converter for Gamebryo Engine\n--------------------------------------------------\n\n";
		
		bool exit; // declare next loop exit
		do { // do once, then check while to continue loop
			exit = true; // define next loop exit
			// prompt user for input
			std::cout << "Press key:\n\t1 - Convert U -> M\n\t2 - Convert M -> U\n\t3 - Exit Now\nuser> ";
			// receive & process user input
			int input = 0; 
			d number = 0.0f;
			if (std::cin >> input) {
				if (input == 1) {
					std::cout << "units> ";
					if (std::cin >> number) {
						std::cout << number << "u = " << toMeters(number) << "m\n\n";
					}
				}
				else if (input == 2) {
					std::cout << "meters> ";
					if (std::cin >> number) {
						std::cout << number << "m = " << toUnits(number) << "u\n\n";
					}
				}
				else if (input == 3) {
					return 0;
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
	return 0;
}