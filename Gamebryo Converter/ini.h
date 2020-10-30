#pragma once
#include "fileIO.h"
#include "factor.h"
static const std::string 
	name_mi = "fRatioMetricImperial",	// metric <-> imperial
	name_um = "fRatioMetricGamebryo",	// unit   <-> metric
	name_ui = "fRatioGamebryoImperial";	// unit	  <-> imperial

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
				size_t index_of_equal, index_of_comment;
				switch ( it->at(0) ) {
				case 'f': // float
					// find the index of the equals sign
					index_of_equal = it->find('=');
					if ( index_of_equal != std::string::npos ) {
						// find semicolon indicating comment
						index_of_comment = it->find(';');
						// if no comment was found, set index to line end
						if ( index_of_comment == std::string::npos )
							index_of_comment = it->size() - 1;

						/// check for matches
						// "fRatioMetricImperial"
						if ( it->substr(0, index_of_equal) == name_mi ) {
							try {
								__ratio_mi = std::stod(it->substr(index_of_equal + 1, index_of_comment));
							}
							catch ( ... ) {} // leave as default
						}
						// "fRatioMetricGamebryo"
						else if ( it->substr(0, index_of_equal) == name_um ) {
							try {
								__ratio_um = std::stod(it->substr(index_of_equal + 1, index_of_comment));
							}
							catch ( ... ) {} // leave as default
						}
						// "fRatioGamebryoImperial"
						else if ( it->substr(0, index_of_equal) == name_ui ) {
							try {
								__ratio_ui = std::stod(it->substr(index_of_equal + 1, index_of_comment));
							}
							catch ( ... ) {} // leave as default
						}
					}
					break;
				case 'i': // int
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