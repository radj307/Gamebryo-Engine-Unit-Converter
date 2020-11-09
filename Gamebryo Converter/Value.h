/**
 * Value.h
 * Contains the Value class
 * by radj307
 */
#pragma once
#include <sstream>
#include <algorithm>
#include "INI/INI.h"
#include "termcolor/termcolor.hpp"

/**
 * class Value
 * Contains a floating-point number & its unit type, as well as the conversion logic.
 */
class Value {
	/**
	 * toMetric()
	 * Returns member value converted to metric meters
	 *
	 * @returns Value		- Returns *this if the requested type == this type
	 */
	inline Value toMetric()
	{
		// switch through input types:
		switch ( _t )
		{
		case TYPE::unit: // units -> meters
			return Value(TYPE::metric, _v * cfg.fGet("Factor_UM"));
		case TYPE::imperial: // feet -> meters
			return Value(TYPE::metric, _v * cfg.fGet("Factor_MI"));
		default:
			return *this;
		}
	}

	/**
	 * toUnit()
	 * Returns member value converted to Bethesda's Units
	 *
	 * @returns Value		- Returns *this if the requested type == this type
	 */
	inline Value toUnit()
	{
		// switch through input types:
		switch ( _t )
		{
		case TYPE::metric: // meters -> units
			return Value(TYPE::unit, _v / cfg.fGet("Factor_UM"));
		case TYPE::imperial: // feet -> units
			return Value(TYPE::unit, _v / cfg.fGet("Factor_UI"));
		default:
			return *this;
		}
	}

	/**
	 * toImperial()
	 * Returns member value converted to imperial feet
	 *
	 * @param useSurveyFoot	- (Default: true) Switches the output type between the U.S. Survey Foot or the U.S. International Foot (== 0.30)
	 * @returns Value		- Returns *this if the requested type == this type
	 */
	inline Value toImperial(bool useSurveyFoot = true)
	{
		// switch through input types:
		switch ( _t )
		{
		case TYPE::metric: // meters -> feet
			return Value(TYPE::imperial, _v / cfg.fGet("Factor_MI"));
		case TYPE::unit: // units -> feet
			return Value(TYPE::imperial, _v * cfg.fGet("Factor_UI"));
		default:
			return *this;
		}
	}

public:
	/**
	 * enum TYPE::
	 * NONE		- ! - Invalid Type
	 * unit		- u - Gamebryo Unit
	 * imperial	- ' - Imperial Foot
	 * metric	- m - Metric Meter
	 */
	const enum class TYPE {
		NONE = '!',			// ! for error/no type
		ALL = '@',			// @ for output type: all
		unit = 'u',			// u for units
		imperial = '\'',	// ' for feet
		metric = 'm'		// m for meter
	};

	/** STATIC **
	 * stot(string)
	 * (String TO Type) converts string to TYPE
	 *
	 * @returns TYPE
	 */
	static inline TYPE stot(std::string s)
	{
		// convert to lowercase
		for ( unsigned int i = 0; i < s.length() - 1; i++ ) { s[i] = std::tolower(s[i]); }
		if ( s == "u" || s == "unit" || s == "units" || s == "beth" || s == "bethesda" || s == "gamebryo" || s == "game" )
			return TYPE::unit;
		else if ( s == "m" || s == "meter" || s == "meters" || s == "metric" || s == "met" )
			return TYPE::metric;
		else if ( s == "i" || s == "f" || s == "ft" || s == "feet" || s == "foot" || s == "\'" || s == "imperial" || s == "imp" )
			return TYPE::imperial;
		else
			return TYPE::NONE;
	}

	/** STATIC **
	 * stod(string)
	 * (String TO Double) Same as std::stod() but returns a -0.0 if an exception is thrown
	 *
	 * @returns double
	 */
	static inline double stod(std::string s)
	{
		try {
			return std::stod(s);
		}
		catch ( ... ) {
			return -0.0f;
		}
	}

	/** CONSTRUCTOR **
	 * Value(TYPE, double)
	 * 
	 * @param myType	- Unit type
	 * @param myValue	- Value in units of myType
	 */
	Value(TYPE myType, double myValue) : _t(myType), _v(myValue) {}

	/** CONSTRUCTOR **
	 * Value(string, string)
	 *
	 * @param myType	- Unit type
	 * @param myValue	- Value in units of myType
	 */
	Value(std::string myType, std::string myValue) : _t(stot(myType)), _v(Value::stod(myValue)) {}

	/**
	 * invalid()
	 * Returns a Value with TYPE::NONE, and -0.0f
	 * Used for error checking
	 *
	 * @returns Value
	 */
	inline Value invalid()
	{
		return Value(TYPE::NONE, -0.0f);
	}

	/**
	 * valid()
	 * Returns true when this Value is valid, false when invalid.
	 *
	 * @returns bool
	 */
	inline bool valid()
	{
		if ( *this == invalid() )
			return false;
		return true;
	}

	/**
	 * Value::operator==(Value)
	 * Use with (Value == Value)
	 *
	 * @returns bool
	 */
	inline bool operator==(Value compare)
	{
		// compare the types & values of this, and compare
		if ( _t == compare._t && _v == compare._v )
			return true;
		return false;
	}

	/**
	 * convert_to(TYPE)
	 * Returns a converted Value
	 * 
	 * @param type		- The requested TYPE
	 * @returns Value	- Returns *this if param type == this type
	 */
	inline Value convert_to(TYPE type)
	{
		// check if types don't match
		if ( _t != type ) {
			switch ( type ) {
			case TYPE::unit: // return this in units
				return toUnit();
			case TYPE::imperial: // return this in feet
				return toImperial();
			case TYPE::metric: // return this in meters
				return toMetric();
			default:break;
			}
		} // else:
		return *this;
	}

	/**
	 * convert_to(string)
	 * Returns a converted Value
	 *
	 * @param strType	- The requested TYPE as a string
	 * @returns Value	- Returns *this if param type == this type
	 */
	inline Value convert_to(std::string strType)
	{
		TYPE type = stot(strType);
		// check if types don't match
		if ( _t != type ) {
			switch ( type ) {
			case TYPE::unit: // return this in units
				return toUnit();
			case TYPE::imperial: // return this in feet
				return toImperial();
			case TYPE::metric: // return this in meters
				return toMetric();
			default:break;
			}
		} // else:
		return *this;
	}

	/**
	 * asString()
	 * Returns a string with value (forced standard notation) & type in the format: ("<value> <type>")
	 * 
	 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until result is >1.0
	 * @returns string
	 */
	inline std::string asString(bool smallUnits = false)
	{
		// create stringstream to force standard notation
		std::stringstream ss;
		ss.precision(cfg.iGet("Precision"));
		ss << std::fixed << _v << ' ' << char(_t);

		// check if smallUnits is enabled
		if ( smallUnits ) {
			// switch type
			switch ( _t ) {
			case TYPE::metric: // metric
				if ( _v < 1.0f ) {
					_v *= 100; // convert meters to centimeters
					ss << "\t ( " << _v << " cm )";
					// check if result val is still less than 1
					if ( _v < 1.0f ) {
						_v *= 10; // convert centimeters to millimeters
						ss << "\t ( " << _v << " mm )";
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 1000; // convert millimeters to micrometers
							ss << "\t ( " << _v << " um )";
						}
					}
				}
				break;
			case TYPE::imperial: // imperial
				if ( _v < 1.0f ) {
					_v *= 12; // convert feet to inches
					ss << "\t ( " << _v << " \" )"; // output inches as well
				}
				break;
			default:break;
			}
		}
		return ss.str();
	}

	/**
	 * cout(bool)
	 * Directly outputs value as string to console with termcolor lib
	 *
	 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until result is >1.0
	 */
	inline void cout(bool smallUnits = false)
	{
		std::cout.precision(cfg.iGet("Precision"));
		std::cout << std::fixed;
		std::cout << termcolor::green << _v << termcolor::reset << ' ' << char(_t);
		switch ( smallUnits ) {
		case true:
			switch ( _t ) {
			case TYPE::metric: // metric
				if ( _v < 1.0f ) {
					_v *= 100; // convert meters to centimeters
					std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " cm )";
					// check if result val is still less than 1
					if ( _v < 1.0f ) {
						_v *= 10; // convert centimeters to millimeters
						std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " mm )";
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 1000; // convert millimeters to micrometers
							std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " um )";
						}
					}
				}
				break;
			case TYPE::imperial: // imperial
				if ( _v < 1.0f ) {
					_v *= 12; // convert feet to inches
					std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " \" )"; // output inches as well
				}
				break;
			default:break;
			}
			break;
		default:break;
		}
	}

protected:
	double _v;	// value as a double float
	TYPE _t;	// unit as a type
};
/**
 * struct Param
 * Contains the required variables to perform a single conversion.
 *
 * @member in
 * @member out
 * @member var
 */
struct Param {
	Value::TYPE
		in = Value::TYPE::NONE,		// Input Type
		out = Value::TYPE::NONE;	// Output Type
	double val = -0.0f;				// Input Value

	inline bool valid()
	{
		if ( in != Value::TYPE::NONE && out != Value::TYPE::NONE && val != -0.0f )
			return true;
		return false;
	}
};