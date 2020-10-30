/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "info.h"
#include "Value.h"

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
			// if arg is entirely alphabetical characters
			if ( std::all_of(arg.begin(), arg.end(), ::isalpha) ) {
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
	// init input Value
	Value in(p.in, p.val), *out = nullptr;
	// check if input is valid
	switch ( in.valid() ) {
	case true:
		if ( p.out != Value::TYPE::NONE ) {
			// init output Value as converted input val
			out = &in.convert_to(p.out);
		}
		else {

		}
		// check if output is valid
		switch ( out->valid() ) {
		case true:
			// display the result to the console
			std::cout << "\t" << in.asString() << "\t=  " << out->asString(true) << std::endl;
			return true;
		default:
			// display error
			info::msg(info::error, "There was an error during the conversion.");
			return false;
		}
	default:
		// display error
		info::msg(info::error, "There was an error processing the input parameters.");
		return false;
	}
}