/****************************************************************************************
 *								    INI_Setting.h										*
 *				Contains the INI_Setting classes used by the INI parser.				*
 *									 by radj307											*
 *																						*
 *			(This file is not intended to be modified, see INI_Defaults.h)				*
 ****************************************************************************************/
#pragma once
#include <string>	// for std::string type

/**
 * struct INI_Setting
 * Parent class for INI_Setting types.
 * NOT FOR DIRECT USAGE
 */
struct INI_Setting {
	/** ENUM **
	 * type
	 *
	 * @param f	- Floating-Point value, stored as a double
	 * @param i	- Integer value, stored as a long int
	 * @param b	- Boolean value, stored as a bool
	 */
	enum class type {
		f = 'f',	// float
		i = 'i',	// int
		b = 'b',	// bool
	};

	/** CONSTRUCTOR **
	 * INI_Setting(enum type, string, string)
	 * 
	 * @param myType	- This INI Setting's value type
	 * @param myName	- This INI Setting's name
	 * @param myDesc	- (Default: disabled) A brief description of this INI setting's purpose, displayed as a comment in the INI file. 
	 */
	INI_Setting(type myType, std::string myName, std::string myDesc) : _type(myType), _name(myName), _desc(myDesc) 
	{
		// add type prefix to name if it doesn't exist
		if ( _name.at(0) != char(_type) )
			_name = char(_type) + _name;
	}

	// _name	- The unique name of this INI Setting. This should be descriptive of its function
	std::string _name;
	// _desc	- A short description of this INI setting's function. This will appear as a comment in the INI file.
	std::string _desc;
	// _type	- The type of this INI Setting. Valid: (f | i | b)
	const type _type;
};

/**
 * class fINI_Setting (INI_Setting child)
 * INI_Setting with a float value type.
 */
class fINI_Setting : public INI_Setting {
	// _value	- This INI Setting's value
	double _value;

public:
	/** CONSTRUCTOR **
	 * fINI_Setting(string, double)
	 * INI_Setting with a floating-point value
	 * 
	 * @param myName	- This INI Setting's name
	 * @param myVal		- This INI Setting's value
	 */
	fINI_Setting(std::string myName, double myVal, std::string myDesc = "") : INI_Setting(type::f, myName, myDesc), _value(myVal) {}

	/**
	 * value()
	 * Returns the value of this INI Setting.
	 *
	 * @returns double
	 */
	double value()
	{
		return _value;
	}
};

/**
 * class iINI_Setting (INI_Setting child)
 * INI_Setting with a integer value type.
 */
class iINI_Setting : public INI_Setting {
	// _value	- This INI Setting's value
	long int _value;

public:
	/** CONSTRUCTOR **
	 * iINI_Setting(string, long int)
	 * INI_Setting with an integer value
	 *
	 * @param myName	- This INI Setting's name
	 * @param myVal		- This INI Setting's value
	 */
	iINI_Setting(std::string myName, long int myVal, std::string myDesc = "") : INI_Setting(type::i, myName, myDesc), _value(myVal) {}

	/**
	 * value()
	 * Returns the value of this INI Setting.
	 *
	 * @returns long int
	 */
	long int value()
	{
		return _value;
	}
};

/**
 * class bINI_Setting (INI_Setting child)
 * INI_Setting with a boolean value type.
 */
class bINI_Setting : public INI_Setting {
	// _value	- This INI Setting's value
	bool _value;

public:
	/** CONSTRUCTOR **
	 * bINI_Setting(string, bool)
	 * INI_Setting with a boolean value
	 *
	 * @param myName	- This INI Setting's name
	 * @param myVal		- This INI Setting's value
	 */
	bINI_Setting(std::string myName, bool myVal, std::string myDesc = "") : INI_Setting(type::b, myName, myDesc), _value(myVal) {}

	/**
	 * value()
	 * Returns the value of this INI Setting.
	 *
	 * @returns bool
	 */
	bool value()
	{
		return _value;
	}
};