#pragma once
#include <iostream>
#include <sstream>
#include <pugixml.hpp>
#include "info.h"
#include "const_defaults.h"

class Config {
protected:	// VARIABLES:

	std::string _filename;	// XML file name
	pugi::xml_document _xml;// XML document
	double 
		_factor_mi = -0.0f, // Metric<->Imperial
		_factor_um = -0.0f,	// Gamebryo<->Metric
		_factor_ui = -0.0f; // Gamebryo<->Imperial
	int _precision = -0;	// Decimal Precision

public:		// FUNCTIONS:

	/** CONSTRUCTOR **
	 * Config(string)
	 * Retrieve XML config file
	 * 
	 * @param filename	- Name of filename
	 */
	Config(std::string filename) : _filename(filename) 
	{
		_xml.load_file("config.xml");

		for ( pugi::xml_node node : _xml.child("Config").children("Setting") ) {
			// retrieve the setting name from XML doc
			std::string settingName = node.attribute("name").as_string();

			// check precision
			if ( settingName == "precision" ) {
				try { _precision = node.attribute("value").as_int(); }
				catch ( std::exception const & ex ) {
					_precision = ___PRECISION;
					info::msg(info::error, "XML file couldn't be read, exception thrown: '" + std::string(ex.what()) + "' -- Using default value");
				}
			}

			// check factor_mi
			else if ( settingName == "factor_mi" ) {
				try { _factor_mi = node.attribute("value").as_double(); }
				catch ( std::exception const& ex ) {
					_factor_mi = ___FACTOR_MI;
					info::msg(info::error, "XML file couldn't be read, exception thrown: '" + std::string(ex.what()) + "' -- Using default value");
				}
			}

			// check factor_um
			else if ( settingName == "factor_um" ) {
				try { _factor_um = node.attribute("value").as_double(); }
				catch ( std::exception const& ex ) {
					_factor_um = ___FACTOR_UM;
					info::msg(info::error, "XML file couldn't be read, exception thrown: '" + std::string(ex.what()) + "' -- Using default value");
				}
			}

			// check factor_ui
			else if ( settingName == "factor_ui" ) {
				try { _factor_ui = node.attribute("value").as_double(); }
				catch ( std::exception const& ex ) {
					_factor_ui = ___FACTOR_UI;
					info::msg(info::error, "XML file couldn't be read, exception thrown: '" + std::string(ex.what()) + "' -- Using default value");
				}
			}
		}
	}

	/**
	 * getPrecision()
	 * Returns the precision value
	 */
	int getPrecision()
	{
		return _precision;
	}

	/**
	 * getFactor_MI()
	 * Returns the metric<->imperial value
	 */
	double getFactor_MI()
	{
		return _factor_mi;
	}

	/**
	 * getFactor_UM()
	 * Returns the unit<->metric value
	 */
	double getFactor_UM()
	{
		return _factor_um;
	}

	/**
	 * getFactor_UI()
	 * Returns the unit<->imperial value
	 */
	double getFactor_UI()
	{
		return _factor_ui;
	}
};

static Config __CFG("config.xml");