/**
 * INI.h
 * Contains the INI class.
 * by radj307
 * 
 ! Requires "INI_Setting.h", "INI_Defaults.h", "FileIO.h", and for Debug "sys.h"
 */
#pragma once
#include "radj307/FileIO.h"	// for file i/o operations
#include "radj307/sys.h"	// for cross-platform system API functions
#include "INI_Defaults.h"	// Required to set default values if ini wasn't found. (_OVERRIDE_DEFAULTS=true disables this.)

/**
 * class INI
 * Parses data from an .ini file, and stores it for later use in the program.
 */
class INI {
	// contains all INI settings for use during runtime.
	std::vector<INI_Setting*> _vec;
	// Name of INI file.
	const std::string _filename;
	// When true, loads all settings found in the INI file even if they aren't on the default list.
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
				if ( index_comment == parse.size() - 1 && !parse.empty()) {
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
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::log, "Loaded " + name + " from " + _filename);
								#endif
							}
							catch ( std::exception const& ex ) {  // catch stod exceptions
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::warning, "Invalid INI setting '" + name + "' was not loaded because of " + ex.what());
								#endif 
							}
							break;
						case 'i': // integer
							try {
								_vec.push_back(new iINI_Setting(name, std::stoi(parse.substr(index_equal + 1, index_comment))));
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::log, "Loaded " + name + " from " + _filename);
								#endif
							}
							catch ( std::exception const& ex ) { // catch stoi exceptions
								#ifdef _DEBUG // Only active on Debug configuration. 
									sys::msg(sys::warning, "Invalid INI setting '" + name + "' was not loaded because of " + ex.what());
								#endif
							}
							break;
						case 'b': // boolean
							if ( parse.substr(index_equal + 1, index_comment) == "true" ) {
								_vec.push_back(new bINI_Setting(name, true));
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::log, "Loaded " + name + " from " + _filename);
								#endif
							}
							else if ( parse.substr(index_equal + 1, index_comment) == "false" ) {
								_vec.push_back(new bINI_Setting(name, false));
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::log, "Loaded " + name + " from " + _filename);
								#endif
							}
							else {
								#ifdef _DEBUG // Only active on Debug configuration.
									sys::msg(sys::warning, "Invalid INI setting '" + name + "' was not loaded.");
								#endif
							}
							break;
						default:break;
						}
					}
					else {
						#ifdef _DEBUG // Only active on Debug configuration.
							sys::msg(sys::warning, "Unexpected INI setting '" + name + "' was not loaded.");
							continue;
						#else
							continue;
						#endif
					}
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
						#ifdef _DEBUG // Only active on Debug configuration.
							sys::msg(sys::log, "INI setting '" + (*it)->_name + "' was loaded from default list.");
						#endif
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
	 * INI(string, bool, bool)
	 *
	 * @param filename				- The name/location of the target INI file
	 * @param override_default_list	- (Default: false) When true, the defaults list is not checked. This allows unexpected INI settings to be loaded & used. Recommended to disable.
	 */
	INI(std::string filename, const bool override_default_list = false) : _filename(filename), _OVERRIDE_DEFAULTS(override_default_list)
	{
		// if file did not load successfully
		if ( !load() ) {
			sys::msg(sys::warning, "INI::load(" + _filename + ") failed! Using default values!");
			// set setting vector to default
			_vec = _DEF;
		}
		else {
		#ifdef _DEBUG // Only active on Debug configuration.
			sys::msg(sys::log, "Successfully loaded settings from " + _filename);
		#endif
		}
	}

	/**
	 * reload()
	 * Attempts to reload the INI file from filename given at instantiation time.
	 * 
	 * @returns bool success state
	 */
	bool reload()
	{
		// load from filename, check if successful
		if (load()) 
			return true;
		return false;
	}

	/**
	 * write()
	 * Writes a new INI file with default values to disk using the filename given at instantiation time.
	 *
	 * @returns bool
	 */
	bool write()
	{
		bool rc = false; // return code
		if (!_vec.empty()) {
			// create stringstream
			std::stringstream ss;
			// iterate through vec, populate stream
			for (auto it = _vec.begin(); it != _vec.end(); it++) {
				switch ((*it)->_type) { // switch setting type
				case INI_Setting::type::f: // add floating-point value
					ss << static_cast<fINI_Setting*>(*it)->_name << "\t=\t" << static_cast<fINI_Setting*>(*it)->value() << std::endl;
					break;
				case INI_Setting::type::i: // add integer value
					ss << static_cast<iINI_Setting*>(*it)->_name << "\t=\t" << static_cast<iINI_Setting*>(*it)->value() << std::endl;
					break;
				case INI_Setting::type::b: // add boolean value
					ss << static_cast<bINI_Setting*>(*it)->_name << "\t=\t" << static_cast<bINI_Setting*>(*it)->value() << std::endl;
					break;
				default:break;
				}
			} // reached end of vec
			// attempt to write stream to file, set return code to result
			rc = FileIO::write(_filename, ss, FileIO::save_type::overwrite);
			// inform user of result
			if (rc)	sys::msg(sys::log, "Successfully created new INI file: \"" + _filename + "\"");
			else	sys::msg(sys::error, "Failed to create new INI file -- Check write permissions.");
		}
		return rc;
	}

#pragma region GETTERS

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
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::f ) {
				return static_cast<double>(static_cast<fINI_Setting*>(*it)->value());
			}
		}
		#ifdef _DEBUG // Only active on Debug configuration.
			sys::msg(sys::error, "Request for value of '" + name + "' failed because it doesn't exist.");
		#endif
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
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::i ) {
				return static_cast<long int>(static_cast<iINI_Setting*>(*it)->value());
			}
		}
		#ifdef _DEBUG // Only active on Debug configuration.
			sys::msg(sys::error, "Request for value of '" + name + "' failed because it doesn't exist.");
		#endif
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
			if ( (name == (*it)->_name || name == (*it)->_name.substr(1)) && (*it)->_type == INI_Setting::type::b ) {
				return static_cast<bool>(static_cast<bINI_Setting*>(*it)->value());
			}
		}
		#ifdef _DEBUG // Only active on Debug configuration.
			sys::msg(sys::error, "Request for value of '" + name + "' failed because it doesn't exist.");
		#endif
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

#pragma endregion GETTERS

	/**
	 * list()
	 * Displays a list of all INI settings currently loaded. Only works in Debug configuration, unless override is specified.
	 * 
	 * @param override_debug	- (Default: false) Forces list to display when not in debug configuration
	 */
	inline void list(bool override_debug = false)
	{
	#ifdef _DEBUG
		if (true) {
	#else
		if (override_debug) {
	#endif
			const unsigned int maxNameLength = 24;
			for (auto it = _vec.begin(); it != _vec.end(); it++) {
				// concatenate long names, and format normal/short names with the correct-ish amount of tabs.
				std::cout << (((*it)->_name.size() < maxNameLength) ? (((*it)->_name.size() < maxNameLength / 4) ? ((*it)->_name + "\t\t\t") : ((*it)->_name + "\t\t")) : (((*it)->_name.size() > maxNameLength) ? ((*it)->_name.substr(0, maxNameLength - 4) + "...\t") : ((*it)->_name + "\t"))) << "=  ";
				switch ((*it)->_type) {
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
		}
	}
};