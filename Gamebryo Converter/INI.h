/****************************************************************************************
 *									    INI.h											*
 *					Contains the INI class and all member functions.					*
 *								 	 by radj307											*
 *																						*
 *			  Include this header in your project to use the INI parser.				*
 *			(This file is not intended to be modified, see INI_Defaults.h)				*
 ****************************************************************************************/
#pragma once
#include "FileIO.h"	// for file i/o operations
#include "INI_Defaults.h"	// includes INI_Setting.h, required to set default values if ini wasn't found.

#include "sys.h"			// for sys::msg()

#define INI_H

/**
 * class INI
 * Parses data from an .ini file, and stores it for later use in the program.
 */
class INI {
	// the INI file name given at instantiation time.
	const std::string _filename;
	// contains all INI settings for use during runtime.
	std::vector<INI_Setting*> _vec;
	// When true, does not validate loaded settings against the default vector. Causes undefined behavior!
	const bool _OVERRIDE_DEFAULTS;

	/**
	 * find_comment(string)
	 * Returns the index of a valid comment character (';' or '#'), or the index of the last character in given string.
	 *
	 * @param in	- Input string
	 * @returns int	- Index of line ending, or comment character.
	 */
	inline int find_comment(std::string in)
	{
		// find index of semicolon and/or hash char
		int index_semicolon = in.find(';'), index_hash = in.find('#');
		// use ternary operator to determine return value
		return (index_semicolon != std::string::npos || index_hash != std::string::npos) ? ((index_semicolon < index_hash) ? (index_semicolon) : (index_hash)) : (in.size() - 1);
	}

	/**
	 * load(string)
	 * Loads INI settings from file given at instantiation time.
	 *
	 * @returns bool success state
	 */
	inline bool load()
	{
		// read file to stringstream
		std::stringstream ss = FileIO::readToStream(_filename);
		// CHECK IF FILE CONTENT WAS READ SUCCESSFULLY
		if ( !ss.str().empty() ) {
			// PARSE FILE CONTENT
			for ( std::string parse; std::getline(ss, parse); ) {
				parse.erase(std::remove(parse.begin(), parse.end(), ' '), parse.end());// remove spaces
				parse.erase(std::remove(parse.begin(), parse.end(), '\t'), parse.end());// remove tabs

				// find index of '=' and either ';'/'#' or line ending
				int index_comment = find_comment(parse), index_equal = parse.find('=');

				// CHECK IF LINE IS NOT A COMMENT AND NOT BLANK
				if ( ((unsigned)index_comment == parse.size() - 1) && !parse.empty() ) {
					// set name
					std::string name = parse.substr(0, index_equal);

					// CHECK IF SETTING EXISTS IN _DEF
					bool exists = false; // flag used to determine if a setting exists in _DEF
					switch ( _OVERRIDE_DEFAULTS ) {
					case true: // if override is enabled, always set exists flag to true
						exists = true;
						break;
					default:
						for ( auto it = _DEF.begin(); it != _DEF.end(); it++ ) {
							if ( name == (*it)->_name ) {
								exists = true; // set flag true
								break; // break early
							}
						}
						break;
					}
					// IF SETTING DOES EXIST, VALIDATE IT
					if ( exists && index_equal != std::string::npos ) {
						// switch prefix character to determine type (whitespace is already gone)
						switch ( name[0] ) {
						case 'f': // float
							try {
								_vec.push_back(new fINI_Setting(name, std::stod(parse.substr(index_equal + 1, index_comment))));
								sys::msg(sys::debug, "Loaded '" + _filename + "' -> " + name + " = " + parse.substr(index_equal + 1, index_comment));
							}
							catch ( std::exception const& ex ) {  // catch stod exceptions
								sys::msg(sys::debug, "Invalid INI setting '" + name + "' was not loaded because of " + ex.what());
							}
							break;
						case 'i': // integer
							try {
								_vec.push_back(new iINI_Setting(name, std::stoi(parse.substr(index_equal + 1, index_comment))));
								sys::msg(sys::debug, "Loaded '" + _filename + "' -> " + name + " = " + parse.substr(index_equal + 1, index_comment));
							}
							catch ( std::exception const& ex ) { // catch stoi exceptions
								sys::msg(sys::debug, "Invalid INI setting '" + name + "' was not loaded because of " + ex.what());
							}
							break;
						case 'b': // boolean
							if ( parse.substr(index_equal + 1, index_comment) == "true" || parse.substr(index_equal + 1, index_comment) == "1" ) {
								_vec.push_back(new bINI_Setting(name, true));
								sys::msg(sys::debug, "Loaded '" + _filename + "' -> " + name + " = " + parse.substr(index_equal + 1, index_comment));
							}
							else if ( parse.substr(index_equal + 1, index_comment) == "false" || parse.substr(index_equal + 1, index_comment) == "0" ) {
								_vec.push_back(new bINI_Setting(name, false));
								sys::msg(sys::debug, "Loaded '" + _filename + "' -> " + name + " = " + parse.substr(index_equal + 1, index_comment));
							}
							else sys::msg(sys::debug, "Invalid INI setting '" + name + "' was not loaded.");
							break;
						default:break;
						}
					}
					else sys::msg(sys::debug, "Unexpected INI setting '" + name + "' was not loaded.");
				} // else ignore comment line & continue
			}
			switch ( _OVERRIDE_DEFAULTS ) {
			case true:break; // if override is enabled, skip checking defaults for missing settings
			default:
				// ADD MISSING SETTINGS FROM _DEF
				for ( auto it = _DEF.begin(); it != _DEF.end(); it++ ) { // iterate defaults vector
					bool exists = false;
					for ( auto _it = _vec.begin(); _it != _vec.end(); _it++ ) { // iterate member vector
						if ( (*it)->_name == (*_it)->_name ) { // check for match
							exists = true; // set exists flag
							break; // break early
						}
					}
					// if a default setting wasn't found in the file, add it to member vector
					if ( !exists ) {
						_vec.push_back(*it);
						sys::msg(sys::debug, "Loaded 'DEFAULTS' -> " + (*it)->_name);
					}
				}
				break;
			}
			return true;
		}
		return false;
	}

public:
	/** CONSTRUCTOR **
	 * INI(string)
	 *
	 * @param _filename				- The name/location of the target INI file
	 * @param writeNew				- (Default: false) When true, overwrites/creates INI file from defaults before loading it.
	 * @param override_default_list	- (Default: false) When true, the defaults list is not checked. This allows unexpected INI settings to be loaded & used. Recommended to disable.
	 */
	INI(const std::string filename, const bool writeNew = false, const bool override_default_list = false) : _filename(filename), _OVERRIDE_DEFAULTS(override_default_list)
	{
		// if write new flag is enabled, create a new INI file before attempting to load it.
		if ( writeNew )
			write();
		// if file did not load successfully
		if ( !load() ) {
			sys::msg(sys::debug, "INI::load(" + _filename + ") failed, using default values.");
			// set setting vector to default
			_vec = _DEF;
		}
		else sys::msg(sys::debug, "Successfully loaded settings from " + _filename);
	}

	/**
	 * write()
	 * Creates a new INI file with all default values.
	 *
	 * @param header			- (Default: none) When not empty, this string appears as a comment at the beginning of the file.
	 *							  If a newline is included, make sure to include a semicolon to indicate the line as a comment.
	 * @param suppress_output	- (Default: false) When true, does not display confirmation/error when saving INI file to disk.
	 * @returns bool success state
	 */
	bool write(std::string header = "", bool suppress_output = false)
	{
		// create stream to save to file
		std::stringstream ss;
		if ( header != "" )
			ss << "; " << header << std::endl << std::endl;
		// iterate through default settings vec
		for ( auto it = _DEF.begin(); it != _DEF.end(); it++ ) {
			// check if setting is a float
			if ( (*it)->_type == INI_Setting::type::f ) {
				fINI_Setting *ptr = static_cast<fINI_Setting*>(*it);
				if ( !ptr->_desc.empty() )
					ss << "; " << ptr->_desc << std::endl;
				ss << ptr->_name;
				if ( ptr->_name.size() - 1 < 15 ) {
					ss << "\t";
					if ( ptr->_name.size() - 1 < 11 ) {
						ss << "\t";
						if ( ptr->_name.size() - 1 < 7 ) {
							ss << "\t";
							if ( ptr->_name.size() - 1 < 3 )
								ss << "\t";
						}
					}
				}
				ss << "\t=\t" << ptr->value() << std::endl;
			}
			// check if setting is an int
			else if ( (*it)->_type == INI_Setting::type::i ) {
				iINI_Setting *ptr = static_cast<iINI_Setting*>(*it);
				if ( !ptr->_desc.empty() )
					ss << "; " << ptr->_desc << std::endl;
				ss << ptr->_name;
				if ( ptr->_name.size() - 1 < 15 ) {
					ss << "\t";
					if ( ptr->_name.size() - 1 < 11 ) {
						ss << "\t";
						if ( ptr->_name.size() - 1 < 7 ) {
							ss << "\t";
							if ( ptr->_name.size() - 1 < 3 )
								ss << "\t";
						}
					}
				}
				ss << "\t=\t" << ptr->value() << std::endl;
			}
			// check if setting is a boolean
			else if ( (*it)->_type == INI_Setting::type::b ) {
				bINI_Setting *ptr = static_cast<bINI_Setting*>(*it);
				if ( !ptr->_desc.empty() ) // If description is not empty, add description to line
					ss << "; " << ptr->_desc << std::endl;
				ss << ptr->_name;
				if ( ptr->_name.size() - 1 < 15 ) {
					ss << "\t";
					if ( ptr->_name.size() - 1 < 11 ) {
						ss << "\t";
						if ( ptr->_name.size() - 1 < 7 ) {
							ss << "\t";
							if ( ptr->_name.size() - 1 < 3 )
								ss << "\t";
						}
					}
				}
				ss << "\t=\t";
				if ( ptr->value() == true )
					ss << "true" << std::endl;
				else if ( ptr->value() == false )
					ss << "false" << std::endl;
			}
			else sys::msg(sys::debug, "Invalid INI setting \"" + (*it)->_name + "\" was not added to the write list. (Check setting type)");
		} // end of default settings vec
		// write to file & determine return code
		if ( !suppress_output ) { // inform user:
			switch ( FileIO::write(_filename, ss, FileIO::save_type::overwrite) ) {
			case true: // successfully saved to file
				sys::msg(sys::log, "Successfully created \"" + _filename + "\" with default values.");
				return true;
			default: // failed to save to file
				sys::msg(sys::error, "Failed to save \"" + _filename + "\" -- Check write permissions.");
				return false;
			}
		} // else don't inform user:
		else return FileIO::write(_filename, ss, FileIO::save_type::overwrite);
	}

	template <class returntype = double>
	returntype getValOf(std::string name)
	{
		for ( auto it = _vec.begin(); it != _vec.end(); it++ ) {
			if ( name == (*it)->_name || name == (*it)->_name.substr(1) ) {
				switch ( (*it)->_type ) {
				case INI_Setting::type::f:
					return static_cast<double>(static_cast<fINI_Setting*>(*it)->value());
				case INI_Setting::type::i:
					return static_cast<long int>(static_cast<iINI_Setting*>(*it)->value());
				case INI_Setting::type::b:
					return static_cast<bool>(static_cast<bINI_Setting*>(*it)->value());
				default:return NULL;
				}
			}
		}
		return NULL;
	}

	/**
	 * fGet(string)
	 * Returns the float value of an INI setting with a given name
	 * 
	 * @param name	- The INI setting name (Including or excluding the type prefix)
	 * @returns double (NULL when invalid)
	 */
	double fGet(std::string name)
	{
		for ( auto it = _vec.begin(); it != _vec.end(); it++ ) {
			// check if name matches name of member at current iterator pos
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::f )
				return static_cast<double>(static_cast<fINI_Setting*>(*it)->value());
		}
		sys::msg(sys::debug, "Request for value of '" + name + "' failed because it doesn't exist.");
		return NULL;
	}
	/**
	 * fGet(fINI_Setting)
	 * 
	 * @param val	- Ref to a specific INI_Setting
	 * @returns double
	 */
	double fGet(fINI_Setting& val)
	{
		return val.value();
	}

	/**
	 * iGet(string)
	 * Returns the int value of an INI setting with a given name
	 * 
	 * @param name	- The INI setting name (Including or excluding the type prefix)
	 * @returns long int (NULL when invalid)
	 */
	long int iGet(std::string name)
	{
		for ( auto it = _vec.begin(); it != _vec.end(); it++ ) {
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::i )
				return static_cast<long int>(static_cast<iINI_Setting*>(*it)->value());
		}
		sys::msg(sys::debug, "Request for value of '" + name + "' failed because it doesn't exist.");
		return NULL;
	}
	/**
	 * iGet(iINI_Setting)
	 * 
	 * @param val	- Ref to a specific INI_Setting
	 * @returns double
	 */
	long int iGet(iINI_Setting& val)
	{
		return val.value();
	}

	/**
	 * bGet(string)
	 * Returns the bool value of an INI setting with a given name
	 * 
	 * @param name	- The INI setting name (Including or excluding the type prefix)
	 * @returns bool (NULL when invalid)
	 */
	bool bGet(std::string name)
	{
		for ( auto it = _vec.begin(); it != _vec.end(); it++ ) {
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::b )
				return static_cast<bool>(static_cast<bINI_Setting*>(*it)->value());
		}
		sys::msg(sys::debug, "Request for value of '" + name + "' failed because it doesn't exist.");
		return NULL;
	}
	/**
	 * bGet(bINI_Setting)
	 * 
	 * @param val	- Ref to a specific INI_Setting
	 * @returns bool
	 */
	bool bGet(bINI_Setting& val)
	{
		return val.value();
	}

	/**
	 * list()
	 * Lists all loaded INI settings in the console.
	 * Only functions in Debug configuration, else does nothing.
	 */
	inline void list()
	{
		#ifdef _DEBUG // auto-enable list in debug config
			const unsigned int maxNameLength = 24;
			std::cout << "(DEBUG) Currently loaded INI settings:" << std::endl;
			for ( auto it = _vec.begin(); it != _vec.end(); it++ ) {
				// concatenate long names, and format normal/short names with the correct-ish amount of tabs.
				std::cout << (((*it)->_name.size() < maxNameLength) ? (((*it)->_name.size() < maxNameLength / 4) ? ((*it)->_name + "\t\t\t") : ((*it)->_name + "\t\t")) : (((*it)->_name.size() > maxNameLength) ? ((*it)->_name.substr(0, maxNameLength - 4) + "...\t") : ((*it)->_name + "\t"))) << "=  ";
				switch ( (*it)->_type ) {
				case INI_Setting::type::f: // float
					std::cout << static_cast<fINI_Setting*>(*it)->value() << std::endl;
					break;
				case INI_Setting::type::i: // int
					std::cout << static_cast<iINI_Setting*>(*it)->value() << std::endl;
					break;
				case INI_Setting::type::b: // bool
					std::cout << static_cast<bINI_Setting*>(*it)->value() << std::endl;
					break;
				default:break;
				}
			}
		#endif
	}
};