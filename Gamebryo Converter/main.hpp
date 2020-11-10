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
	bool canSet = true;
	// input types
	ValType *in = nullptr, *out = nullptr;
	// value
	double val = -0.0f;

	// iterate arguments, skip argv[0]
	for ( unsigned int i = startAt; i < (unsigned)argc; i++ ) {
		// if valid parameters were found, proceed with conversion
		if ( (in != nullptr && in->_t != ValType::TYPE::NONE) && (out != nullptr && out->_t != ValType::TYPE::NONE) && (val != -0.0f) ) {
			// init Values
			Value input(in->_t, val), output = input.convert_to(out->_t);

		#ifndef WSL // Non-virtual architecture
			// ANSI color codes from termcolor
			std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
		#else // WSL console doesn't appear to support ANSI so disable it
			std::cout << '\t' << input.asString() << "\t=  " << output.asString(true) << std::endl;
		#endif
			in = nullptr;
			out = nullptr;
			val = -0.0f;
		}

		// convert to string
		std::string arg = argv[i];
		
		// check if arg has at least 1 character
		if ( arg.size() > 0 ) {
			if ( arg.at(0) == '/' ) {
				std::stringstream ss;
				// remove arg[0], '/'
				ss << arg.substr(1) << ',';
				for (std::string parse; std::getline(ss, parse, ','); parse.clear()) {
					if ( parse.size() > 2 ) {
						if ( parse == "ini" )
							cfg.write();
						else if ( parse.at(0) == 'f' && parse.at(1) == '=' ) {
							File proc(parse.substr(2));
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
			else if ( std::all_of(arg.begin(), arg.end(), ::isalpha) ) {
				// check if input type was already set
				if (in != nullptr)
					out = new ValType(Value::stot(arg));
				else // else set input type
					in = new ValType(Value::stot(arg));
			}
			// if arg is entirely digits
			else {
				std::string tmp = arg;
				tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());
				if ( std::all_of(tmp.begin(), tmp.end(), ::isdigit) ) {
					try {
						val = std::stod(arg);
					}
					catch (std::exception & exc) { // catch stod exceptions
						sys::msg(sys::error, "\"" + std::string(exc.what()) + "\" was thrown while interpreting the value!");
					}
				}
			}
		}
		else continue;
	}

	// if valid parameters were found, proceed with conversion
	if ( (in != nullptr && in->_t != ValType::TYPE::NONE) && (out != nullptr && out->_t != ValType::TYPE::NONE) && (val != -0.0f) ) {
		// init Values
		Value input(in->_t, val), output = input.convert_to(out->_t);
		
	#ifndef WSL // Non-virtual architecture
		// ANSI color codes from termcolor
		std::cout << '\t'; input.cout(); std::cout << "\t=  "; output.cout(true); std::cout << std::endl;
	#else // WSL console doesn't appear to support ANSI so disable it
		std::cout << '\t' << input.asString() << "\t=  " << output.asString(true) << std::endl;
	#endif

		return true;
	}
	return false;
}