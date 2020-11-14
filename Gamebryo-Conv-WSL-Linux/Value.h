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
 */
struct ValType {
	// Enumerate valid types
	enum class TYPE {
		NONE,			// none/error
		ALL,			// all (output)
		unit,			// units
		imperial,		// feet
		imperial_inch,	// inch
		metric,			// meter
		metric_cm,		// centimeters
		metric_mm,		// millimeters
		metric_um,		// micrometers
	};

	// This instance's type
	TYPE _t;

	/**
	 * symbol()
	 * Returns this ValType as its official representative symbol.
	 *
	 * @returns const char*
	 */
	inline std::string symbol()
	{
		switch ( _t ) {
		case TYPE::unit:			return ("u");	// u
		case TYPE::imperial:		return ("\'");	// '
		case TYPE::imperial_inch:	return ("\"");	// "
		case TYPE::metric:			return ("m");	// m
		case TYPE::metric_cm:		return ("cm");	// cm
		case TYPE::metric_mm:		return ("mm");	// mm
		case TYPE::metric_um:		return ("um");	// um
		default:					return ("");	// <blank>
		}
	}
	/** STATIC **
	 * symbol()
	 * Returns a ValType as its official representative symbol.
	 *
	 * @returns const char*
	 */
	static inline const char* symbol(TYPE t)
	{
		switch ( t ) {
		case TYPE::unit:			return (const char*)("u");	// u
		case TYPE::imperial:		return (const char*)("\'");	// '
		case TYPE::imperial_inch:	return (const char*)("\"");	// "
		case TYPE::metric:			return (const char*)("m");	// m
		case TYPE::metric_cm:		return (const char*)("cm");	// cm
		case TYPE::metric_mm:		return (const char*)("mm");	// mm
		case TYPE::metric_um:		return (const char*)("um");	// um
		default:					return (const char*)("");	// <blank>
		}
	}

	/**
	 * asString()
	 * Returns this ValType as its full name, of type string
	 * 
	 * @returns string
	 */
	inline std::string asString()
	{
		switch ( _t ) {
		case TYPE::unit:			return ("(Gamebryo Units)");
		case TYPE::imperial:		return ("(Feet)");
		case TYPE::imperial_inch:	return ("(Inches)");
		case TYPE::metric:			return ("(Meters)");
		case TYPE::metric_cm:		return ("(Centimeters)");
		case TYPE::metric_mm:		return ("(Millimeters)");
		case TYPE::metric_um:		return ("(Micrometers)");
		default:					return ("(Error-Type)");
		}
	}
	/** STATIC **
	 * asString()
	 * Returns a ValType as its full name, of type string
	 *
	 * @returns string
	 */
	static inline std::string asString(TYPE t)
	{
		switch ( t ) {
		case TYPE::unit:			return ("(Gamebryo Units)");
		case TYPE::imperial:		return ("(Feet)");
		case TYPE::imperial_inch:	return ("(Inches)");
		case TYPE::metric:			return ("(Meters)");
		case TYPE::metric_cm:		return ("(Centimeters)");
		case TYPE::metric_mm:		return ("(Millimeters)");
		case TYPE::metric_um:		return ("(Micrometers)");
		default:					return ("(Error-Type)");
		}
	}

	/** CONSTRUCTOR (DEFAULT) **
	 * ValType(enum type)
	 * Instantiate a new Value Type
	 */
	ValType(TYPE myType = TYPE::NONE) : _t(myType) {}
	/** CONSTRUCTOR (MOVE) **
	 * ValType(ValType&& other)
	 * Move constructor
	 */
	ValType(ValType&& other) noexcept : _t(std::move(other._t)) {}

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
	inline bool operator!=(ValType& compare)
	{
		if (_t != compare._t)
			return true;
		return false;
	}
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
	template <class EnumType = TYPE>
	inline Value toMetric(const EnumType metric_sub_type = TYPE::metric)
	{
		int modifier = 1;
		switch ( metric_sub_type ) {
		case TYPE::metric_cm:
			modifier = 100;
			break;
		case TYPE::metric_mm:
			modifier = 1000;
			break;
		case TYPE::metric_um:
			modifier = 1000000;
			break;
		default:break;
		}
		// switch through input types:
		switch ( _t )
		{
		case TYPE::unit: // units -> meters
			return Value(metric_sub_type, (_v * cfg.fGet("Factor_UM")) * modifier);
		case TYPE::imperial: // feet -> meters
			return Value(metric_sub_type, (_v * cfg.fGet("Factor_MI")) * modifier);
		case TYPE::imperial_inch: // inch -> feet -> meters
			return Value(metric_sub_type, ((_v / 12) * cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_cm: // cm -> m
			return Value(metric_sub_type, ((_v / 100) * cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_mm: // mm -> m
			return Value(metric_sub_type, ((_v / 1000) * cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_um: // um -> m
			return Value(metric_sub_type, ((_v / 1000000) * cfg.fGet("Factor_MI")) * modifier);
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
		case TYPE::metric_cm: // cm -> m -> units
			return Value(TYPE::unit, (_v / 100) / cfg.fGet("Factor_UM"));
		case TYPE::metric_mm: // mm -> m -> units
			return Value(TYPE::unit, (_v / 1000) / cfg.fGet("Factor_UM"));
		case TYPE::metric_um: // um -> m -> units
			return Value(TYPE::unit, (_v / 1000000) / cfg.fGet("Factor_UM"));
		case TYPE::imperial: // feet -> units
			return Value(TYPE::unit, _v / cfg.fGet("Factor_UI"));
		case TYPE::imperial_inch: // inch -> feet -> units
			return Value(TYPE::unit, ((_v / 12) / cfg.fGet("Factor_UI")));
		default:
			return *this;
		}
	}

	/**
	 * toImperial()
	 * Returns member value converted to imperial feet
	 *
	 * @returns Value		- Returns *this if the requested type == this type
	 */
	template <class EnumType = TYPE>
	inline Value toImperial(const EnumType imperial_sub_type = TYPE::imperial)
	{
		int modifier = 1;
		switch ( imperial_sub_type ) {
		case TYPE::imperial_inch:
			modifier = 12;
			break;
		default:break;
		}
		// switch through input types:
		switch ( _t )
		{
		case TYPE::metric: // meters -> feet
			return Value(imperial_sub_type, (_v / cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_cm: // cm -> m -> feet
			return Value(imperial_sub_type, ((_v / 100) / cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_mm: // mm -> m -> feet
			return Value(imperial_sub_type, ((_v / 1000) / cfg.fGet("Factor_MI")) * modifier);
		case TYPE::metric_um: // um -> m -> feet
			return Value(imperial_sub_type, ((_v / 1000000) / cfg.fGet("Factor_MI")) * modifier);
		case TYPE::unit: // units -> feet
			return Value(imperial_sub_type, (_v * cfg.fGet("Factor_UI")) * modifier);
		case TYPE::imperial_inch: // inch -> feet
			return Value(imperial_sub_type, ((_v / 12) * cfg.fGet("Factor_UI")) * modifier);
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
		// Convert input string to lowercase
		for ( auto it = s.begin(); it != s.end(); it++ ) { *it = std::tolower(*it); }

		// GAMEBRYO UNIT
		if ( s == "u" || s == "unit" || s == "units" || s == "beth" || s == "bethesda" || s == "gamebryo" || s == "game" )
			return TYPE::unit;

		// METRIC
		else if ( s == "m" || s == "meter" || s == "meters" || s == "metric" || s == "met" )
			return TYPE::metric;
		else if ( s == "cm" || s == "centimeter" || s == "centimeters" )
			return TYPE::metric_cm;
		else if ( s == "mm" || s == "millimeter" || s == "millimeters" )
			return TYPE::metric_mm;
		else if ( s == "um" || s == "micrometer" || s == "micrometers" )
			return TYPE::metric_um;

		// IMPERIAL
		else if ( s == "f" || s == "ft" || s == "feet" || s == "foot" || s == "imperial" || s == "imp" )
			return TYPE::imperial;
		else if ( s == "i" || s == "in" || s == "inch" || s == "inches" )
			return TYPE::imperial_inch;

		// MISC
		else if ( s == "a" || s == "all" ) return TYPE::ALL;
		else							   return TYPE::NONE;
	}

	/** STATIC **
	 * stod(string)
	 * (String TO Double) Same as std::stod() but returns a -0.0 if an exception is thrown
	 *
	 * @returns double
	 */
	static inline double stod(std::string s)
	{
		try { return std::stod(s); }
		catch ( ... ) { return -0.0f; }
	}

	/** CONSTRUCTOR **
	 * Value(TYPE, double)
	 * 
	 * @param myType	- Unit type
	 * @param myValue	- Value in units of myType
	 */
	// NEW FROM TYPE
	Value(ValType::TYPE myType, double myValue) : ValType(myType), _v(myValue) {}
	// NEW FROM STRING
	Value(std::string myType, std::string myValue) : ValType(stot(myType)), _v(Value::stod(myValue)) {}
	// COPY FROM OTHER
	Value(const Value& other) : ValType(other._t), _v(other._v) {}
	// MOVE FROM OTHER
	Value(Value&& other) noexcept : ValType(std::move(other)), _v(std::move(other._v)) { *this = std::move(other); }

	inline Value& operator=(Value&& other) noexcept
	{
		_t = std::move(other._t);
		_v = std::move(other._v);
		delete[] &other;
		return *this;
	}


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
	inline Value convert_to(TYPE otherType)
	{
		// check if types don't match
		if ( _t != otherType ) {
			switch ( otherType ) {
			case TYPE::unit: // return this in units
				return toUnit();
			case TYPE::imperial: // return this in feet
				return toImperial();
			case TYPE::imperial_inch: // return this in feet
				return toImperial(TYPE::imperial_inch);
			case TYPE::metric: // return this in meters
				return toMetric();
			case TYPE::metric_cm: // return this in meters
				return toMetric(TYPE::metric_cm);
			case TYPE::metric_mm: // return this in meters
				return toMetric(TYPE::metric_mm);
			case TYPE::metric_um: // return this in meters
				return toMetric(TYPE::metric_um);
			default:break;
			}
		} // else:
		return *this;
	}
	inline Value convert_to(ValType& other)
	{
		return Value::convert_to(other._t);
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
		ss << std::fixed << _v << ' ' << symbol();

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
		std::cout << termcolor::green << _v << termcolor::reset << ' ' << symbol();
	#else
		std::cout << _v << ' ' << symbol();
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
						std::cout << "  ( " << _v << " mm )";
					#endif
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 1000; // convert millimeters to micrometers
						#ifndef WSL
							std::cout << "  ( " << termcolor::green << _v << termcolor::reset << " um )";
						#else
							std::cout << "  ( " << _v << " um )";
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
		std::cout << termcolor::yellow << _v << termcolor::reset << ' ' << symbol();
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