/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "info.h"
#include "Value.h"
#include "file.h"

/**
 * interpret(int, char*[])
 * Used to convert commandline arguments into usable commands.
 * 
 * @param argc		- The argument count from main()
 * @param argv		- The argument array from main()
 * @returns bool	- ( true = success ) ( false = failed because: invalid arguments, errors )
 */
inline bool interpret(int argc, char* argv[])
{
	// instantiate Param struct
	Param p;
	// iterate from argv[1] until argc
	for ( int i = 1; i < argc; i++ ) {
		// convert to string
		std::string arg = argv[i];
		// check if arg has at least 1 character
		if ( arg.size() > 0 ) {
			if ( arg.at(0) == '/' ) {
				// remove arg[0], '/'
				arg = arg.substr(1);
				// process file
				File process(arg);
				switch ( process._success ) {
				case true:
					info::msg(info::log, "Successfully processed '" + arg + "'");
					return true;
				default:
					return false;
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
		// init input Value
		Value in(p.in, p.val), out = in.convert_to(p.out);
		if ( in.valid() && out.valid() ) { // display the result to the console
			std::cout << '\t'; in.cout(); std::cout << std::fixed << "\t=  "; out.cout(true); std::cout << std::endl;
			//std::cout << "\t" << in.asString() << "\t=  " << out.asString(true) << std::endl;
			return true;
		}
		else
			info::msg(info::error, "Validated parameters caused an invalid result. Report this error to the dev!");
	}
	else
		info::msg(info::error, "Invalid Parameters.");
	// undefined returns false
	return false;
}