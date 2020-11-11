/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "file.h"

/**
 * convert(ValType*, ValType*, double*)
 * Performs a single conversion
 * 
 * @param in	- Ptr to input type
 * @param out	- Ptr to output type
 * @param val	- Ptr to value
 */
inline void convert(ValType* in, ValType* out, double* val)
{
	if ( (in != nullptr && in->_t != ValType::TYPE::NONE) && (out != nullptr && out->_t != ValType::TYPE::NONE) && (*val != -0.0f) ) {
		// init Value
		Value input((*in)._t, *val);
		// if types don't match, and neither types are TYPE::ALL, convert to target type
		if ( (in->_t != ValType::TYPE::ALL && out->_t != ValType::TYPE::ALL) && (in->_t != out->_t) ) {
			Value output = input.convert_to(out->_t);
			std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
		}
		// if types don't match, and only output type is TYPE::ALL, convert to all types
		else if ( (in->_t != ValType::TYPE::ALL && out->_t == ValType::TYPE::ALL) && (in->_t != out->_t) ) {
			// when true, displays a blank space instead of the input type
			bool display_short = false;

			// if input type is not units, convert to units
			if ( input._t != ValType::TYPE::unit ) {
				// show input
				std::cout << '\t';
				if ( !display_short || cfg.bGet("AlwaysShowInput") )
					input.cout();
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}
				// show output
				std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
				input.convert_to(ValType::TYPE::unit).cout(true); 
				std::cout << std::endl;
				// set flag
				display_short = true;
			}

			// if input type is not metric, convert to metric
			if ( input._t != ValType::TYPE::metric ) {
				// show input
				std::cout << '\t';
				if ( !display_short || cfg.bGet("AlwaysShowInput") )
					input.cout();
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}
				// show output
				std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
				input.convert_to(ValType::TYPE::metric).cout(true); 
				std::cout << std::endl;
				// set flag
				display_short = true;
			}

			// if input type is not imperial, convert to imperial
			if ( input._t != ValType::TYPE::imperial ) {
				// show input
				std::cout << '\t';
				if ( !display_short || cfg.bGet("AlwaysShowInput") )
					input.cout();
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}
				// show output
				std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
				input.convert_to(ValType::TYPE::imperial).cout(true);
				std::cout << std::endl;
				// set flag
				display_short = true;
			}
		}
		// 
		else sys::msg(sys::error, in->asString() + " to " + out->asString() + " isn't a valid conversion.");
	}
}

// holds the needed arguments for one conversion
struct Param {
	// input types
	ValType *in, *out;
	// value
	double val;

	/** CONSTRUCTOR **
	 */
	Param() : in(nullptr), out(nullptr), val(-0.0f) {}

	/**
	 * clear()
	 * Resets all member values.
	 */
	inline void clear()
	{
		in = nullptr;
		out = nullptr;
		val = -0.0f;
	}
};

/**
 * interpret(int, char*[])
 * Used to convert commandline arguments into usable commands.
 * 
 * @param argc		- The argument count from main()
 * @param argv		- The argument array from main()
 * @returns int		- ( 0 = success ) ( 1 = failed because: invalid arguments, errors )
 */
inline int interpret(const int argc, const char* argv[], const unsigned int startAt = 1)
{
	// check if arg count is valid
	if ( argc > 1 ) {
		Param p; // hold conversion arguments so they can be properly reset 

		// iterate arguments, skip argv[0]
		for ( unsigned int i = startAt; i < (unsigned)argc; i++ ) {
			// convert to string
			std::string arg = argv[i];

			// check if arg has at least 1 character
			if ( arg.size() > 0 ) {
				// check if first char is a slash
				if ( arg.at(0) == '/' ) {
					std::stringstream ss;
					// remove the slash
					ss << arg.substr(1) << ',';
					// parse command argument
					for ( std::string parse; std::getline(ss, parse, ','); parse.clear() ) {
						// ensure checking arg won't cause exception
						if ( parse.size() > 2 ) {
							if ( parse == "ini" )
								cfg.write("Gamebryo Engine Unit Converter Settings\n; Changing the conversion factors is not recommended.");
							else if ( parse.at(0) == 'f' && parse.at(1) == '=' ) {
								File proc(parse.substr(2));
								// check if file conversion succeeded
								switch ( proc._success ) {
								case true:
									sys::msg(sys::log, "Successfully processed file: \"" + parse.substr(2) + "\"");
									break;
								default:
									sys::msg(sys::error, "Failed to process file: \"" + parse.substr(2) + "\"");
									break;
								}
								// File instance is deleted when out of scope
							}
						} // else continue
					}
				}
				// if arg is entirely alphabetical characters
				else if ( std::all_of(arg.begin(), arg.end(), ::isalpha) ) {
					// check if input type has not been set
					if ( p.in == nullptr )
						p.in = new ValType(Value::stot(arg));
					else { // else set output type & begin conversion
						p.out = new ValType(Value::stot(arg));
						convert(p.in, p.out, &p.val);
						p.clear(); // reset arguments for next conversion
					}
				}
				else {
					// copy arg to temp string
					std::string tmp = arg;
					// remove all decimal points from temp string
					tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());

					// check if temp string is entirely digits
					if ( std::all_of(tmp.begin(), tmp.end(), ::isdigit) ) {
						// set val to arg as type double
						try { p.val = std::stod(arg); }
						catch ( std::exception & exc ) { // catch stod exceptions
							sys::msg(sys::error, "\"" + std::string(exc.what()) + "\" was thrown while interpreting the value!");
						}
					}

					// else show invalid argument warning
					else sys::msg(sys::warning, "Invalid argument: " + arg);
				}
			}
			else continue;
		}
		return 0;
	} // else return error
	return 1;
}