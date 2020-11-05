#pragma once
#include "fileIO.h"
#include "factor.h"
#include "info.h"

static const std::string // INI setting names:
	__NAME_PRECISION = "iPrecision",		// Decimal precision
	__NAME_MI = "fRatioMetricImperial",		// metric <-> imperial
	__NAME_UM = "fRatioMetricGamebryo",		// unit   <-> metric
	__NAME_UI = "fRatioGamebryoImperial";	// unit	  <-> imperial

/**
 * init_factor(string)
 * Initializes the conversion factor values from file. If the file couldn't be found/opened, the default values above are used.
 *
 * @param filename				- Defines the file to get conversion factors from
 * @returns bool success state
 */
inline bool init_factor(std::string filename)
{
	// read file content
	std::vector<std::string> content = fileRead(filename);
	if ( !content.empty() ) {
		// iterate through content
		for ( auto it = content.begin(); it != content.end(); it++ ) {
			// remove all blank space
			it->erase(std::remove(it->begin(), it->end(), ' '), it->end());
			it->erase(std::remove(it->begin(), it->end(), '\t'), it->end());
			// if the line is entirely blank now, continue
			if ( it->size() > 0 ) {
				// find the index of the equals sign
				size_t index_of_equal = it->find('='), index_of_comment = it->size() - 1;
				if ( index_of_equal != std::string::npos ) {
					// find semicolon indicating comment
					size_t index = it->find(';');
					// if no comment was found, set index to line end
					if ( index != std::string::npos )
						index_of_comment = index;
				}
				switch ( it->at(0) ) {
				case 'f': // float
					/// check for matches
					// "fRatioMetricImperial"
					if ( it->substr(0, index_of_equal) == __NAME_MI ) {
						try {
							__FACTOR_MI = std::stod(it->substr(index_of_equal + 1, index_of_comment));
						}
						catch ( std::exception const & ex ) {
							info::msg(info::error, "INI file setting '" + __NAME_MI + "' couldn't be read, exception thrown: " + std::string(ex.what()));
						}
					}
					// "fRatioMetricGamebryo"
					else if ( it->substr(0, index_of_equal) == __NAME_UM ) {
						try {
							__FACTOR_UM = std::stod(it->substr(index_of_equal + 1, index_of_comment));
						}
						catch ( std::exception const& ex ) {
							info::msg(info::error, "INI file setting '" + __NAME_UM + "' couldn't be read, exception thrown: " + std::string(ex.what()));
						}
					}
					// "fRatioGamebryoImperial"
					else if ( it->substr(0, index_of_equal) == __NAME_UI ) {
						try {
							__FACTOR_UI = std::stod(it->substr(index_of_equal + 1, index_of_comment));
						}
						catch ( std::exception const& ex ) {
							info::msg(info::error, "INI file setting '" + __NAME_UI + "' couldn't be read, exception thrown: " + std::string(ex.what()));
						}
					}
					break;
				case 'i': // int
					if ( it->substr(0, index_of_equal) == __NAME_PRECISION ) {
						try {
							__PRECISION = std::stoi(it->substr(index_of_equal + 1, index_of_comment));
						}
						catch ( std::exception const& ex ) {
							info::msg(info::error, "INI file setting '" + __NAME_PRECISION + "' couldn't be read, exception thrown: " + std::string(ex.what()));
						}
					}
					break;
				case 'b': // bool
					break;
				default:break;
				}
			} // else continue
		}
		return true;
	}
	else return false;
}

/**
 * create_ini(string)
 * Write a new INI file with default values
 *
 * @param filename	- Target filename
 * @returns bool success state
 */
inline bool create_ini(std::string filename)
{
	// create stringstream
	std::stringstream ss;
	// set fixed standard notation
	ss << std::fixed;
	// set precision to 64-bit
	ss.precision(64);

	// add text to buffer
	ss << "; You can use all, or none, of these values. Simply comment them out (or delete this file) to ignore.\n; The first letter of each setting defines what type of variable it is, 'i' = integer, 'f' = floating-point, 'b' = boolean" 
		<< "\n\n; How many decimal points to display, affects both console & files.\n" << __NAME_PRECISION << "\t=\t" << __PRECISION
		<< "\n\n; Factor used for (Metric <-> Imperial) conversions:\n" << __NAME_MI <<	"\t=\t" << __FACTOR_MI << 
		"\n; Default U.S. Survey Foot (1893):	0.3048006096012191857\n; U.S.International Foot(1959) : 0.3048\n; The difference between these values is 2 ppm, or 2' for every 1 million feet" 
		<< "\n\n; Factor used for (Metric <-> Gamebryo Unit) conversions:\n" << __NAME_UM << "\t=\t" << __FACTOR_UM 
		<< "\n\n; Factor used for (Gamebryo Unit <-> Imperial) conversions:\n" << __NAME_UI << "\t=\t" << __FACTOR_UI;

	// return the result of writing to file
	return fileWrite(filename, ss);
}