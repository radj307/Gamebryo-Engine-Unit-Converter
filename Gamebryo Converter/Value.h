/**
 * Value.h
 * Contains the Value class
 * by radj307
 */
#pragma once
#include <sstream>
#include <algorithm>
#include "INI.h"
#include "termcolor.hpp"

/**
 * struct ValType
 * Wrapper for Value Unit Types, used by the Value class below.
 * NOT FOR DIRECT USE
 * 
 * @param none		( ! )	- No type/Error type.
 * @param all		( @ )	- All types
 * @param unit		( u )	- Gamebryo Units
 * @param imperial	( ' )	- Feet & inches
 * @param metric	( m )	- Meters & subunits
 */
struct ValType {
	// Enumerate valid types
	enum class TYPE {
		NONE = '!',			// ! for none/error
		ALL = 'a',			// @ for output type: all
		unit = 'u',			// u for units
		imperial = '\'',	// ' for feet
		metric = 'm'		// m for meter
	};

	// This instance's type
	const TYPE _t;

	/** CONSTRUCTOR **
	 * ValType(enum type)
	 * Instantiate a new Value Type
	 */
	ValType(TYPE myType) : _t(myType) {}

	/**
	 * asString()
	 * Returns this type as a string.
	 * 
	 * @returns string
	 */
	inline std::string asString()
	{
		switch ( _t ) {
		case TYPE::ALL:
			return "(All)";
		case TYPE::unit:
			return "(Units)";
		case TYPE::metric:
			return "(Meters)";
		case TYPE::imperial:
			return "(Feet)";
		default:return "(ERROR_TYPE)";
		}
	}

#pragma region OPERATORS
	/**
	 * operator==
	 * Comparison operator.
	 * 
	 * @param compare	- Another ValType to compare against
	 * @returns bool	- ( true = types match ) ( false = types do not match )
	 */
	inline bool operator==(ValType& compare)
	{
		if (compare._t == _t)
			return true;
		return false;
	}
	/**
	 * operator!=
	 * Comparison operator.
	 *
	 * @param compare	- Another ValType to compare against
	 * @returns bool	- ( true = types do not match ) ( false = types match )
	 */
	inline bool operator!=(ValType::TYPE& compare)
	{
		if (_t != compare)
			return true;
		return false;
	}
	/**
	 * operator!=
	 * Comparison operator.
	 *
	 * @param compare	- Another ValType to compare against
	 * @returns bool	- ( true = types do not match ) ( false = types match )
	 */
	inline bool operator!=(ValType& compare)
	{
		if (_t != compare._t)
			return true;
		return false;
	}
#pragma endregion OPERATORS

};

/**
 * class Value
 * Contains a floating-point number & its unit type, as well as the conversion logic.
 */
class Value : public ValType {
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
		if ( s == "a" || s == "all" )
			return TYPE::ALL;
		else if ( s == "u" || s == "unit" || s == "units" || s == "beth" || s == "bethesda" || s == "gamebryo" || s == "game" )
			return TYPE::unit;
		else if ( s == "m" || s == "meter" || s == "meters" || s == "metric" || s == "met" )
			return TYPE::metric;
		else if ( s == "i" || s == "f" || s == "ft" || s == "feet" || s == "foot" || s == "imperial" || s == "imp" )
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
	Value(ValType::TYPE myType, double myValue) : ValType(myType), _v(myValue) {}

	/** CONSTRUCTOR **
	 * Value(string, string)
	 *
	 * @param myType	- Unit type
	 * @param myValue	- Value in units of myType
	 */
	Value(std::string myType, std::string myValue) : ValType(stot(myType)), _v(Value::stod(myValue)) {}

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
	inline Value convert_to(ValType::TYPE type)
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
	#ifndef WSL
		std::cout << termcolor::green << _v << termcolor::reset << ' ' << char(_t);
	#else
		std::cout << _v << ' ' << char(_t);
	#endif
		switch ( smallUnits ) {
		case true:
			switch ( _t ) {
			case TYPE::metric: // metric
				if ( _v < 1.0f ) {
					_v *= 100; // convert meters to centimeters
				#ifndef WSL
					std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " cm )";
				#else
					std::cout << "  ( " << _v << " cm )";
				#endif
					// check if result val is still less than 1
					if ( _v < 1.0f ) {
						_v *= 10; // convert centimeters to millimeters
					#ifndef WSL
						std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " mm )";
					#else
						std::cout << "  ( " << _v << " cm )";
					#endif
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 1000; // convert millimeters to micrometers
						#ifndef WSL
							std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " um )";
						#else
							std::cout << "  ( " << _v << " cm )";
						#endif
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
	/**
	 * cout(bool)
	 * Directly outputs value as string to console with termcolor lib
	 *
	 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until result is >1.0
	 */
	inline void cout_all(bool smallUnits = false)
	{
		std::cout.precision(cfg.iGet("Precision"));
		std::cout << std::fixed;
		std::cout << termcolor::yellow << _v << termcolor::reset << ' ' << char(_t);
		switch ( smallUnits ) {
		case true:
			switch ( _t ) {
			case TYPE::metric: // metric
				if ( _v < 1.0f ) {
					_v *= 100; // convert meters to centimeters
					std::cout << "  ( " << termcolor::yellow << _v << termcolor::reset << " cm )";
					// check if result val is still less than 1
					if ( _v < 1.0f ) {
						_v *= 10; // convert centimeters to millimeters
						std::cout << "  ( " << termcolor::yellow << _v << termcolor::reset << " mm )";
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 1000; // convert millimeters to micrometers
							std::cout << "  ( " << termcolor::yellow << _v << termcolor::reset << " um )";
						}
					}
				}
				break;
			case TYPE::imperial: // imperial
				if ( _v < 1.0f ) {
					_v *= 12; // convert feet to inches
					std::cout << "  ( " << termcolor::yellow << _v << termcolor::reset << " \" )"; // output inches as well
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
};