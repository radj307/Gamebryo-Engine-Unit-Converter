/**
 * Value.h
 * Contains the Value class
 * by radj307
 */
#pragma once
#include <sstream>
#include <algorithm>
#include <INI.hpp>
#include <strmanip.hpp>
#include <utility>
#include <strconv.hpp>
#include <TermAPI.hpp>

namespace ck_unit_conv {
	/**
	 * struct ValType
	 * Wrapper for Value Unit Types, used by the Value class below.
	 */
	struct ValType {
		// Enumerate valid types
		enum class TYPE {
			NONE,			// none/error
			ALL,			// all (output)
			UNIT,			// units
			IMPERIAL,		// feet
			IMPERIAL_INCH,	// inch
			METRIC,			// meter
			METRIC_CM,		// centimeters
			METRIC_MM,		// millimeters
			METRIC_UM,		// micrometers
		};

		// This instance's type
		TYPE _t;

		/**
		 * symbol()
		 * @brief Returns this ValType as its official representative symbol.
		 * @returns const char*
		 */
		[[nodiscard]] std::string symbol() const
		{
			switch ( _t ) {
			case TYPE::UNIT:			return ( "u" );	// u
			case TYPE::IMPERIAL:		return ( "\'" );	// '
			case TYPE::IMPERIAL_INCH:	return ( "\"" );	// "
			case TYPE::METRIC:			return ( "m" );	// m
			case TYPE::METRIC_CM:		return ( "cm" );	// cm
			case TYPE::METRIC_MM:		return ( "mm" );	// mm
			case TYPE::METRIC_UM:		return ( "um" );	// um
			default:					return ( "" );	// <blank>
			}
		}
		/**
		 * symbol()
		 * @brief Returns a ValType as its official representative symbol.
		 * @returns const char*
		 */
		static const char* symbol(const TYPE t)
		{
			switch ( t ) {
			case TYPE::UNIT:			return "u";                    // u
			case TYPE::IMPERIAL:		return "\'";                   // '
			case TYPE::IMPERIAL_INCH:	return "\"";                   // "
			case TYPE::METRIC:			return "m";                    // m
			case TYPE::METRIC_CM:		return "cm";                   // cm
			case TYPE::METRIC_MM:		return "mm";                   // mm
			case TYPE::METRIC_UM:		return "um";                   // um
			case TYPE::NONE: break;
			case TYPE::ALL: break;
			default:					return ""; // <blank>
			}
			return nullptr;
		}

		/**
		 * asString()
		 * @brief Returns this ValType as its full name, of type string
		 * @returns string
		 */
		[[nodiscard]] std::string asString() const
		{
			switch ( _t ) {
			case TYPE::UNIT:			return ( "(Gamebryo Units)" );
			case TYPE::IMPERIAL:		return ( "(Feet)" );
			case TYPE::IMPERIAL_INCH:	return ( "(Inches)" );
			case TYPE::METRIC:			return ( "(Meters)" );
			case TYPE::METRIC_CM:		return ( "(Centimeters)" );
			case TYPE::METRIC_MM:		return ( "(Millimeters)" );
			case TYPE::METRIC_UM:		return ( "(Micrometers)" );
			case TYPE::NONE: break;
			case TYPE::ALL: break;
			default:					return ( "(Error-Type)" );
			}
			return {};
		}

		/**
		 * asString()
		 * @brief Returns a ValType as its full name, of type string
		 * @returns string
		 */
		static std::string asString(const TYPE t)
		{
			switch ( t ) {
			case TYPE::UNIT:			return ( "(Gamebryo Units)" );
			case TYPE::IMPERIAL:		return ( "(Feet)" );
			case TYPE::IMPERIAL_INCH:	return ( "(Inches)" );
			case TYPE::METRIC:			return ( "(Meters)" );
			case TYPE::METRIC_CM:		return ( "(Centimeters)" );
			case TYPE::METRIC_MM:		return ( "(Millimeters)" );
			case TYPE::METRIC_UM:		return ( "(Micrometers)" );
			case TYPE::NONE: break;
			case TYPE::ALL: break;
			default:					return ( "(Error-Type)" );
			}
			return {};
		}

		/** CONSTRUCTOR (DEFAULT) **
		 * ValType(enum type)
		 * Instantiate a new Value Type
		 */
		explicit ValType(const TYPE myType = TYPE::NONE) : _t(myType) {}

		/**
		 * operator==
		 * @brief Comparison operator.
		 * @param compare	- Another ValType to compare against
		 * @return true		- Types match.
		 * @return false	- Types do not match.
		 */
		bool operator==(ValType& compare) const
		{
			if ( compare._t == _t )
				return true;
			return false;
		}

		/**
		 * operator!=
		 * @brief Comparison operator.
		 * @param compare	- Another ValType to compare against
		 * @return true		- Types do not match.
		 * @return false	- Types match.
		 */
		bool operator!=(ValType& compare) const
		{
			if ( _t != compare._t )
				return true;
			return false;
		}
	};

	/**
	 * @class Value
	 * @brief Contains a floating-point number & its unit type, as well as the conversion logic.
	 */
	class Value final : public ValType {
		/**
		 * toMetric()
		 * @brief Returns member value converted to metric meters.
		 * @returns Value		- Returns *this if the requested type == this type
		 */
		template <class EnumType = TYPE>
		Value toMetric(const EnumType metric_sub_type = TYPE::METRIC) const
		{
			auto modifier{ 1 };
			switch ( metric_sub_type ) {
			case TYPE::METRIC_CM:
				modifier = 100;
				break;
			case TYPE::METRIC_MM:
				modifier = 1000;
				break;
			case TYPE::METRIC_UM:
				modifier = 1000000;
				break;
			default:break;
			}
			// switch through input types:
			switch ( _t ) 		{
			case TYPE::UNIT: // units -> meters
				return Value(metric_sub_type, ( _v * cfg->fGet("conversions", "metric_to_unit") ) * modifier);
			case TYPE::IMPERIAL: // feet -> meters
				return Value(metric_sub_type, ( _v * cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::IMPERIAL_INCH: // inch -> feet -> meters
				return Value(metric_sub_type, ( ( _v / 12 ) * cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_CM: // cm -> m
				return Value(metric_sub_type, ( ( _v / 100 ) * cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_MM: // mm -> m
				return Value(metric_sub_type, ( ( _v / 1000 ) * cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_UM: // um -> m
				return Value(metric_sub_type, ( ( _v / 1000000 ) * cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			default:
				return *this;
			}
		}

		/**
		 * toUnit()
		 * @brief Returns member value converted to Bethesda's Units.
		 * @returns Value		- Returns *this if the requested type == this type
		 */
		Value toUnit() const
		{
			// switch through input types:
			switch ( _t ) 		{
			case TYPE::METRIC: // meters -> units
				return Value(TYPE::UNIT, _v / cfg->fGet("conversions", "metric_to_unit"));
			case TYPE::METRIC_CM: // cm -> m -> units
				return Value(TYPE::UNIT, ( _v / 100 ) / cfg->fGet("conversions", "metric_to_unit"));
			case TYPE::METRIC_MM: // mm -> m -> units
				return Value(TYPE::UNIT, ( _v / 1000 ) / cfg->fGet("conversions", "metric_to_unit"));
			case TYPE::METRIC_UM: // um -> m -> units
				return Value(TYPE::UNIT, ( _v / 1000000 ) / cfg->fGet("conversions", "metric_to_unit"));
			case TYPE::IMPERIAL: // feet -> units
				return Value(TYPE::UNIT, _v / cfg->fGet("conversions", "imperial_to_unit"));
			case TYPE::IMPERIAL_INCH: // inch -> feet -> units
				return Value(TYPE::UNIT, ( ( _v / 12 ) / cfg->fGet("conversions", "imperial_to_unit") ));
			default:
				return *this;
			}
		}

		/**
		 * toImperial()
		 * @brief Returns member value converted to imperial feet.
		 * @returns Value		- Returns *this if the requested type == this type
		 */
		template <class EnumType = TYPE>
		Value toImperial(const EnumType imperial_sub_type = TYPE::IMPERIAL) const
		{
			auto modifier{ 1 };
			switch ( imperial_sub_type ) {
			case TYPE::IMPERIAL_INCH:
				modifier = 12;
				break;
			default:break;
			}
			// switch through input types:
			switch ( _t ) 		{
			case TYPE::METRIC: // meters -> feet
				return Value(imperial_sub_type, ( _v / cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_CM: // cm -> m -> feet
				return Value(imperial_sub_type, ( ( _v / 100 ) / cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_MM: // mm -> m -> feet
				return Value(imperial_sub_type, ( ( _v / 1000 ) / cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::METRIC_UM: // um -> m -> feet
				return Value(imperial_sub_type, ( ( _v / 1000000 ) / cfg->fGet("conversions", "imperial_to_metric") ) * modifier);
			case TYPE::UNIT: // units -> feet
				return Value(imperial_sub_type, ( _v * cfg->fGet("conversions", "imperial_to_unit") ) * modifier);
			case TYPE::IMPERIAL_INCH: // inch -> feet
				return Value(imperial_sub_type, ( ( _v / 12 ) * cfg->fGet("conversions", "imperial_to_unit") ) * modifier);
			case TYPE::NONE: break;
			case TYPE::ALL: break;
			case TYPE::IMPERIAL: break;
			default:
				return *this;
			}
			return {};
		}

	public:
		/** STATIC **
		 * stot(string)
		 * @brief (String TO Type) converts string to TYPE.
		 * @returns TYPE
		 */
		static TYPE stot(std::string s)
		{
			// Convert input string to lowercase
			s = str::tolower(s);

			// GAMEBRYO UNIT
			if ( s == "u" || s == "unit" || s == "units" || s == "beth" || s == "bethesda" || s == "gamebryo" || s == "game" )
				return TYPE::UNIT;

			// METRIC
			if ( s == "m" || s == "meter" || s == "meters" || s == "metric" || s == "met" )
				return TYPE::METRIC;
			if ( s == "cm" || s == "centimeter" || s == "centimeters" )
				return TYPE::METRIC_CM;
			if ( s == "mm" || s == "millimeter" || s == "millimeters" )
				return TYPE::METRIC_MM;
			if ( s == "um" || s == "micrometer" || s == "micrometers" )
				return TYPE::METRIC_UM;

			// IMPERIAL
			if ( s == "f" || s == "ft" || s == "feet" || s == "foot" || s == "imperial" || s == "imp" )
				return TYPE::IMPERIAL;
			if ( s == "i" || s == "in" || s == "inch" || s == "inches" )
				return TYPE::IMPERIAL_INCH;

			// MISC
			if ( s == "a" || s == "all" ) return TYPE::ALL;
			return TYPE::NONE;
		}

		/**
		 * Value(const TYPE, const double)
		 * @brief Value constructor that takes a const double.
		 * @param myType	- Unit type.
		 * @param myValue	- Value in units of myType.
		 */
		Value(const TYPE myType, const double myValue) : ValType(myType), _v(myValue) {}
		/**
		 * Value(const TYPE, const string&)
		 * @brief Value constructor that takes a const string&.
		 * @param myType	- Unit type.
		 * @param myValue	- Value in units of myType.
		 */
		Value(std::string myType, const std::string& myValue) : ValType(stot(std::move(myType))), _v(str::stod(myValue)) {}

		Value(Value&&) = default;
		Value(const Value&) = default;
		Value() : ValType(TYPE::NONE), _v{ 0.0 } {}


		/**
		 * invalid()
		 * Returns a Value with TYPE::NONE, and -0.0f
		 * Used for error checking
		 *
		 * @returns Value
		 */
		static Value invalid()
		{
			return Value(TYPE::NONE, -0.0f);
		}

		/**
		 * valid()
		 * @brief Returns true when this Value is valid, false when invalid.
		 * @returns bool
		 */
		bool valid()
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
		bool operator==(const Value& compare) const
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
		 * @param otherType	- The requested TYPE
		 * @returns Value	- Returns *this if param type == this type
		 */
		Value convert_to(const TYPE otherType)
		{
			// check if types don't match
			if ( _t != otherType ) {
				switch ( otherType ) {
				case TYPE::UNIT: // return this in units
					return toUnit();
				case TYPE::IMPERIAL: // return this in feet
					return toImperial();
				case TYPE::IMPERIAL_INCH: // return this in feet
					return toImperial(TYPE::IMPERIAL_INCH);
				case TYPE::METRIC: // return this in meters
					return toMetric();
				case TYPE::METRIC_CM: // return this in meters
					return toMetric(TYPE::METRIC_CM);
				case TYPE::METRIC_MM: // return this in meters
					return toMetric(TYPE::METRIC_MM);
				case TYPE::METRIC_UM: // return this in meters
					return toMetric(TYPE::METRIC_UM);
				default:break;
				}
			} // else:
			return *this;
		}

		Value convert_to(ValType& other)
		{
			return Value::convert_to(other._t);
		}

		/**
		 * asString()
		 * Returns a string with value (forced standard notation) & type in the format: ("<value> <type>")
		 *
		 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until ( result < 1.0 )
		 * @returns string
		 */
		std::string asString(const bool smallUnits = false)
		{
			// create stringstream to force standard notation
			std::stringstream ss;
			ss.precision(cfg->iGet("config", "precision"));
			ss << std::fixed << _v << ' ' << symbol();

			// check if smallUnits is enabled
			if ( smallUnits ) {
				// switch type
				switch ( _t ) {
				case TYPE::METRIC: // metric
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
				case TYPE::IMPERIAL: // imperial
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
		 * Directly outputs value as string to console with Color lib
		 *
		 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until ( result < 1.0 )
		 */
		void cout(bool smallUnits = false)
		{
			std::cout.precision(cfg->iGet("config", "precision"));
			std::cout << std::fixed;
		#ifndef WSL
			std::cout << color::setcolor(color::green) << _v << Color::reset << ' ' << symbol();
		#else
			std::cout << _v << ' ' << symbol();
		#endif
			switch ( smallUnits ) {
			case true:
				switch ( _t ) {
				case TYPE::METRIC: // metric
					if ( _v < 1.0f ) {
						_v *= 100; // convert meters to centimeters
					#ifndef WSL
						std::cout << "  ( " << color::setcolor(color::green) << _v << Color::reset << " cm )";
					#else
						std::cout << "  ( " << _v << " cm )";
					#endif
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 10; // convert centimeters to millimeters
						#ifndef WSL
							std::cout << "  ( " << color::setcolor(color::green) << _v << Color::reset << " mm )";
						#else
							std::cout << "  ( " << _v << " mm )";
						#endif
							// check if result val is still less than 1
							if ( _v < 1.0f ) {
								_v *= 1000; // convert millimeters to micrometers
							#ifndef WSL
								std::cout << "  ( " << color::setcolor(color::green) << _v << Color::reset << " um )";
							#else
								std::cout << "  ( " << _v << " um )";
							#endif
							}
						}
					}
					break;
				case TYPE::IMPERIAL: // imperial
					if ( _v < 1.0 ) {
						_v *= 12; // convert feet to inches
						std::cout << "  ( " << color::setcolor(color::green) << _v << Color::reset << " \" )"; // output inches as well
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
		 * Directly outputs value as string to console with Color lib
		 *
		 * @param smallUnits	- (Default: false) Set to true to keep converting to smaller units until result is >1.0
		 */
		void cout_all(bool smallUnits = false)
		{
			std::cout.precision(cfg->iGet("config", "precision"));
			std::cout << std::fixed;
			std::cout << color::setcolor(color::yellow) << _v << Color::reset << ' ' << symbol();
			switch ( smallUnits ) {
			case true:
				switch ( _t ) {
				case TYPE::METRIC: // metric
					if ( _v < 1.0f ) {
						_v *= 100; // convert meters to centimeters
						std::cout << "  ( " << color::setcolor(color::yellow) << _v << Color::reset << " cm )";
						// check if result val is still less than 1
						if ( _v < 1.0f ) {
							_v *= 10; // convert centimeters to millimeters
							std::cout << "  ( " << color::setcolor(color::yellow) << _v << Color::reset << " mm )";
							// check if result val is still less than 1
							if ( _v < 1.0f ) {
								_v *= 1000; // convert millimeters to micrometers
								std::cout << "  ( " << color::setcolor(color::yellow) << _v << Color::reset << " um )";
							}
						}
					}
					break;
				case TYPE::IMPERIAL: // imperial
					if ( _v < 1.0f ) {
						_v *= 12; // convert feet to inches
						std::cout << "  ( " << color::setcolor(color::yellow) << _v << Color::reset << " \" )"; // output inches as well
					}
					break;
				default:break;
				}
				break;
			default:break;
			}
		}

	protected:
		double _v;	// value as a double
	};
}