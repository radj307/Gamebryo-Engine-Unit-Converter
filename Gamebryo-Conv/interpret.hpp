/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include "file-conv.h"

/**
 * convert(ValType*, ValType*, double*)
 * Performs a single conversion
 * 
 * @param in	- Ptr to input type
 * @param out	- Ptr to output type
 * @param val	- Ptr to value
 */
inline void convert(ValType* in, ValType* out, double* val, std::string& arg)
{
	// if input type is invalid, check if it is a valid subunit
	if ( in->_t == ValType::TYPE::NONE ) {
		switch ( Value::stost(arg) ) {
		case ValType::IN_TYPE::in: // inches -> feet
			*val /= 12;
			in = new ValType(ValType::TYPE::imperial);
			break;
		case ValType::IN_TYPE::cm: // centimeters -> meters
			*val /= 100;
			in = new ValType(ValType::TYPE::metric);
			break;
		case ValType::IN_TYPE::mm: // millimeters -> meters
			*val /= 1000;
			in = new ValType(ValType::TYPE::metric);
			break;
		case ValType::IN_TYPE::um: // micrometers -> meters
			*val /= 1000000;
			in = new ValType(ValType::TYPE::metric);
			break;
		default:break;
		}
	} // proceed with conversion
	// check if input & output types are valid, and if value has changed from default
	if ( (in != nullptr && in->_t != ValType::TYPE::NONE) && (out != nullptr && out->_t != ValType::TYPE::NONE) && (*val != -0.0f) ) {
		// init Value
		Value input((*in)._t, *val);
		// if types don't match, and neither types are TYPE::ALL, convert to target type
		if ( (in->_t != ValType::TYPE::ALL && out->_t != ValType::TYPE::ALL) && (in->_t != out->_t) ) {
			Value output = input.convert_to(out->_t);
			if ( !cfg.bGet("DisableColors") ) {
				std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
			}
			else std::cout << '\t' << input.asString() << "\t=  " << output.asString(true) << std::endl;
		}
		// if types don't match, and only output type is TYPE::ALL, convert to all types
		else if ( (in->_t != ValType::TYPE::ALL && out->_t == ValType::TYPE::ALL) && (in->_t != out->_t) ) {
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
		// 
		else sys::msg(sys::error, in->asString() + " to " + out->asString() + " isn't a valid conversion.");
	}
}

// holds the needed arguments for one conversion
struct Param {
	// argument given as input, used if input type is not a primary type & requires conversion to primary in convert()
	// ex: ( inches, cm, mm, um )
	std::string inputArg;
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
					if ( p.in == nullptr ) {
						p.in = new ValType(Value::stot(*it));
						p.inputArg = *it;
					}
					else { // else set output type & begin conversion
						p.out = new ValType(Value::stot(*it));
						convert(p.in, p.out, &p.val, p.inputArg);
						p.clear(); // reset arguments for next conversion
					}
				}
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