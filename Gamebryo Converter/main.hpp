/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "file.h"

/**
 * interpret(int, char*[])
 * Used to convert commandline arguments into usable commands.
 * 
 * @param argc		- The argument count from main()
 * @param argv		- The argument array from main()
 * @returns bool	- ( true = success ) ( false = failed because: invalid arguments, errors )
 */
inline bool interpret(const int argc, const char* argv[], const unsigned int startAt = 1)
{
	// instantiate Param struct
	Param p;

	// iterate arguments, skip argv[0]
	for ( unsigned int i = startAt; i < (unsigned)argc; i++ ) {
		// convert to string
		std::string arg = argv[i];
		// check if arg has at least 1 character
		if ( arg.size() > 0 ) {
			if ( arg.at(0) == '/' ) {
				// remove arg[0], '/'
				arg = arg.substr(1);

				if ( arg == "ini" ) {
					cfg.write();
				}
				else {
					// process file
					File process(arg);
					switch ( process._success ) {
					case true:
						sys::msg(sys::log, "Successfully processed '" + arg + "'");
						return true;
					default:
						return false;
					}
				}
			}
			// if arg is entirely alphabetical characters
			else if ( std::all_of(arg.begin(), arg.end(), ::isalpha) ) {
				// check if input type has already been set
				switch ( p.in ) {
				case Value::TYPE::NONE: // input type was not set
					p.in = Value::stot(arg);
					break;
				default: // input type was set
					p.out = Value::stot(arg);
					break;
				}
			}
			// if arg is entirely digits
			else {
				std::string tmp = arg;
				tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());
				if ( std::all_of(tmp.begin(), tmp.end(), ::isdigit) ) {
					try { p.val = std::stod(arg); }
					catch ( std::exception const& error ) { // catch possible exception from std::stod()
						std::cout << "[ERROR]\t'" << error.what() << "' was thrown while interpreting the value!" << std::endl;
					}
				}
			}
		}
		else continue;
	}

	// if valid parameters were found, proceed with conversion
	if ( p.valid() ) {
		// init Values
		Value in(p.in, p.val), out = in.convert_to(p.out);
		// check if Values are valid
		if ( in.valid() && out.valid() ) { // display the result to the console
			// colored terminal output
			std::cout << '\t'; in.cout(); std::cout << "\t=  "; out.cout(true); std::cout << std::endl;

			// raw terminal output
			//std::cout << "\t" << in.asString() << "\t=  " << out.asString(true) << std::endl;

			return true;
		}
		else // display critical error: valid input produced invalid output
			sys::msg(sys::error, "Critical error occurred within the program - Valid input produced invalid output. (Report this error, including the input values, on github)");
	}
	return false;
}