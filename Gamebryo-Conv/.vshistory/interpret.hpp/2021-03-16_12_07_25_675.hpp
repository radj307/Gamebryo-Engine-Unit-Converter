/**
 * main.hpp
 * Contains argument interpretation logic used by main.cpp
 * by radj307
 */
#pragma once
#include <sysapi.h>
#include <opt.hpp>

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
	void clear()
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
		if ((p.in._t != ValType::TYPE::ALL && p.out._t != ValType::TYPE::ALL) && (p.in != p.out)) {
			Value output = input.convert_to(p.out);
			if (!cfg.bGet("config", "disable-colors")) {
				std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
			}
			else std::cout << '\t' << input.asString() << "\t=  " << output.asString(true) << std::endl;
		}
		// if types don't match, and only output type is TYPE::ALL, convert to all types
		else if ((p.in._t != ValType::TYPE::ALL && p.out._t == ValType::TYPE::ALL) && (p.in != p.out)) {
			// when true, displays a blank space instead of the input type
			bool display_short = false;

			// if input type is not units, convert to units
			if (input._t != ValType::TYPE::UNIT) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if (!display_short || cfg.bGet("config", "always-show-input")) {
					if (!cfg.bGet("config", "disable-colors")) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for (unsigned int i = 0; i < input.asString().size() - 1; i++)
						std::cout << ' ';
				}

				// show output
				if (!cfg.bGet("config", "disable-colors")) { // check if INI disable color is on
					std::cout << Color::f_cyan << "\t=  " << Color::reset;
					input.convert_to(ValType::TYPE::UNIT).cout(true);
					std::cout << std::endl;
				}
				else {
					std::cout << "\t=  " << input.convert_to(ValType::TYPE::UNIT).asString(true) << std::endl;
				}
				// set flag
				display_short = true;
			}

			// if input type is not metric, convert to metric
			if (input._t != ValType::TYPE::METRIC) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if (!display_short || cfg.bGet("config", "always-show-input")) {
					if (!cfg.bGet("config", "disable-colors")) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for (unsigned int i = 0; i < input.asString().size() - 1; i++)
						std::cout << ' ';
				}

				// show output
				if (!cfg.bGet("config", "disable-colors")) { // check if INI disable color is on
					std::cout << Color::f_cyan << "\t=  " << Color::reset;
					input.convert_to(ValType::TYPE::METRIC).cout(true);
					std::cout << std::endl;
				}
				else std::cout << "\t=  " << input.convert_to(ValType::TYPE::UNIT).asString(true) << std::endl;
				// set flag
				display_short = true;
			}

			// if input type is not imperial, convert to imperial
			if (input._t != ValType::TYPE::IMPERIAL) {
				std::cout << '\t';

				// show input
				// check if input val has already been displayed, or if forced on from INI
				if (!display_short || cfg.bGet("config", "always-show-input")) {
					if (!cfg.bGet("config", "disable-colors")) // check if INI disable color is on
						input.cout();
					else std::cout << input.asString();
				}
				else { // show blank space instead
					for (unsigned int i = 0; i < input.asString().size() - 1; i++)
						std::cout << ' ';
				}

				// show output
				if (!cfg.bGet("config", "disable-colors")) { // check if INI disable color is on
					std::cout << Color::f_cyan << "\t=  " << Color::reset;
					input.convert_to(ValType::TYPE::IMPERIAL).cout(true);
					std::cout << std::endl;
				}
				else {
					std::cout << "\t=  " << input.convert_to(ValType::TYPE::UNIT).asString(true) << std::endl;
				}
				// set flag
				display_short = true;
			}
		}
		// else show error message
		else std::cout << sys::error << p.in.asString() << " to " << p.out.asString() << " isn't a valid conversion." << std::endl;
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
inline int interpret(opt::list& args)
{
	// check if arg count is valid
	if ( !args.empty() ) {
		Param p; // hold conversion arguments so they can be properly reset 

		// COMMANDS
		if (args.exists("file", "f")) {
			File proc(args.getParam("file"));
		}

		/*for ( auto it = args.com.begin(); it != args.com.end(); it++ ) {
			// check if arg has at least 1 character
			if ( (*it).checkName("ini") ) {
				cfg.write("Gamebryo Engine Unit Converter INI");
			}
			else if ( (*it).checkName("file") ) {
				File proc((*it)._opt);
				// check if file conversion succeeded
				switch ( proc._success ) {
				case true:
					sys::msg(sys::log, "Successfully processed file: \"" + (*it)._opt + "\"");
					break;
				default:
					sys::msg(sys::error, "Failed to process file: \"" + (*it)._opt + "\"");
					break;
				}
			}
		}*/

		// PARAMS
		for (auto& it : args.getKeywords()) {
			if (std::all_of(it.begin(), it.end(), isalpha)) {
				if (p.in._t == ValType::TYPE::NONE) {
					p.in._t = Value::stot(it);
				}
				else {
					p.out._t = Value::stot(it);
					convert(p);
				}
			}
			else {
				std::string tmp{ it };
				tmp.erase(std::remove_if(tmp.begin(), tmp.end(), ispunct), tmp.end());
				if (std::all_of(tmp.begin(), tmp.end(), isdigit))
					p.val = str::stod(it);
			}
		}
		/*for ( auto it = args.par.begin(); it != args.par.end(); it++ ) {
			// if arg is entirely alphabetical characters
			if ( std::all_of((*it)._name.begin(), (*it)._name.end(), ::isalpha) ) {
				// check if input type has not been set
				if ( p.in._t == ValType::TYPE::NONE )
					p.in._t = Value::stot((*it)._name);
				else { // else set output type & begin conversion
					p.out._t = Value::stot((*it)._name);
					convert(p);
				}
			}
			// check if arg is only digits
			else {
				// copy arg to temp string
				std::string tmp = (*it)._name;
				// remove all decimal points from temp string
				tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());

				// check if temp string is entirely digits
				if ( std::all_of(tmp.begin(), tmp.end(), ::isdigit) ) {
					// set val to arg as type double
					try { p.val = std::stod((*it)._name); }
					catch ( std::exception & exc ) { // catch stod exceptions
						sys::msg(sys::error, "\"" + std::string(exc.what()) + "\" was thrown while interpreting the value!");
					}
				}
				// else show invalid argument warning
				else sys::msg(sys::warning, "Invalid parameter: " + (*it)._name);
			}
		}*/
		return 0;
	} // else return error
	return 1;
}