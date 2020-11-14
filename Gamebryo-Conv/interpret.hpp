/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "file-conv.h"

// holds the needed arguments for one conversion
struct Param {
	// input & output types
	ValType in, out;

	// value
	double val;

	/** CONSTRUCTOR **
	 */
	Param() : val(-0.0f) {}

	/**
	 * clear()
	 * Resets all member values.
	 */
	inline void clear()
	{
		in._t = ValType::TYPE::NONE;
		out._t = ValType::TYPE::NONE;
		val = -0.0f;
	}
};

/**
 * convert(Param&)
 * Performs a single conversion
 * 
 * @param p	- Parameter struct containing all conversion arguments.
 */
inline void convert(Param& p)
{
	// check if input & output types are valid, and if value has changed from default
	if ( (p.in._t != ValType::TYPE::NONE) && (p.out._t != ValType::TYPE::NONE) && (p.val != -0.0f) ) {
		// init Value
		Value input(p.in._t, p.val);
		// if types don't match, and neither types are TYPE::ALL, convert to target type
		if ( (p.in._t != ValType::TYPE::ALL && p.out._t != ValType::TYPE::ALL) && (p.in != p.out) ) {
			Value output = input.convert_to(p.out);
			if ( !cfg.bGet("DisableColors") ) {
				std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
			}
			else std::cout << '\t' << input.asString() << "\t=  " << output.asString(true) << std::endl;
		}
		// if types don't match, and only output type is TYPE::ALL, convert to all types
		else if ( (p.in._t != ValType::TYPE::ALL && p.out._t == ValType::TYPE::ALL) && (p.in != p.out) ) {
			// when true, displays a blank space instead of the input type
			bool display_short = false;

			// if input type is not units, convert to units
			if ( input._t != ValType::TYPE::unit ) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if ( !display_short || cfg.bGet("AlwaysShowInput") ) {
					if ( !cfg.bGet("DisableColors") ) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}

				// show output
				if ( !cfg.bGet("DisableColors") ) { // check if INI disable color is on
					std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
					input.convert_to(ValType::TYPE::unit).cout(true);
					std::cout << std::endl;
				}
				else {
					std::cout << "\t=  " << input.convert_to(ValType::TYPE::unit).asString(true) << std::endl;
				}
				// set flag
				display_short = true;
			}

			// if input type is not metric, convert to metric
			if ( input._t != ValType::TYPE::metric ) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if ( !display_short || cfg.bGet("AlwaysShowInput") ) {
					if ( !cfg.bGet("DisableColors") ) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}

				// show output
				if ( !cfg.bGet("DisableColors") ) { // check if INI disable color is on
					std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
					input.convert_to(ValType::TYPE::metric).cout(true); 
					std::cout << std::endl;
				}
				else std::cout << "\t=  " << input.convert_to(ValType::TYPE::unit).asString(true) << std::endl;
				// set flag
				display_short = true;
			}

			// if input type is not imperial, convert to imperial
			if ( input._t != ValType::TYPE::imperial ) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if ( !display_short || cfg.bGet("AlwaysShowInput") ) {
					if ( !cfg.bGet("DisableColors") ) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for ( unsigned int i = 0; i < input.asString().size() - 1; i++ )
						std::cout << ' ';
				}

				// show output
				if ( !cfg.bGet("DisableColors") ) { // check if INI disable color is on
					std::cout << termcolor::cyan << "\t=  " << termcolor::reset;
					input.convert_to(ValType::TYPE::imperial).cout(true);
					std::cout << std::endl;
				}
				else {
					std::cout << "\t=  " << input.convert_to(ValType::TYPE::unit).asString(true) << std::endl;
				}
				// set flag
				display_short = true;
			}
		}
		// else show error message
		else sys::msg(sys::error, p.in.asString() + " to " + p.out.asString() + " isn't a valid conversion.");
	}
	// clear params
	p.clear();
}

/**
 * interpret(int, char*[])
 * Used to convert commandline arguments into usable commands.
 * 
 * @param argc		- The argument count from main()
 * @param argv		- The argument array from main()
 * @returns int		- ( 0 = success ) ( 1 = failed because: invalid arguments, errors )
 */
inline int interpret(std::vector<std::string> arg)
{
	// check if arg count is valid
	if ( !arg.empty() ) {
		Param p; // hold conversion arguments so they can be properly reset 

		// iterate arguments, skip argv[0]
		for ( auto it = arg.begin(); it != arg.end(); it++ ) {
			// check if arg has at least 1 character
			if ( (*it).size() > 0 ) {
				// check if first char is a slash
				if ( (*it).at(0) == '/' ) {
					std::stringstream ss;
					// remove the slash
					ss << (*it).substr(1) << ',';
					// parse command argument
					for ( std::string parse; std::getline(ss, parse, ','); parse.clear() ) {
						// ensure checking arg won't cause exception
						if ( parse.size() > 2 ) {
							if ( parse == "ini" )
								cfg.write(/*File Header: */"Gamebryo Engine Unit Converter Settings\n; Changing the conversion factors is not recommended.");
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
							}
						} // else continue
					}
				}
				// if arg is entirely alphabetical characters
				else if ( std::all_of((*it).begin(), (*it).end(), ::isalpha) ) {
					// check if input type has not been set
					if ( p.in._t == ValType::TYPE::NONE )
						p.in._t = Value::stot(*it);
					else { // else set output type & begin conversion
						p.out._t = Value::stot(*it);
						convert(p);
					}
				}
				// check if arg is only digits
				else {
					// copy arg to temp string
					std::string tmp = *it;
					// remove all decimal points from temp string
					tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());

					// check if temp string is entirely digits
					if ( std::all_of(tmp.begin(), tmp.end(), ::isdigit) ) {
						// set val to arg as type double
						try { p.val = std::stod(*it); }
						catch ( std::exception & exc ) { // catch stod exceptions
							sys::msg(sys::error, "\"" + std::string(exc.what()) + "\" was thrown while interpreting the value!");
						}
					}

					// else show invalid argument warning
					else sys::msg(sys::warning, "Invalid argument: " + *it);
				}
			}
			else continue;
		}
		return 0;
	} // else return error
	return 1;
}