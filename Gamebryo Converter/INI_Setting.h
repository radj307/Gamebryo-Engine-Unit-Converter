/**
 * INI_Setting.h
 * Contains struct/classes for individual INI settings of various types.
 * by radj307
 */
#pragma once
#include <string>	// for std::string type

/**
 * struct INI_Setting
 * Parent class for INI Setting types.
 * Does not contain a value, use one of the below classes for objects.
 */
struct INI_Setting {
	/** ENUM **
	 * type
	 * 
	 * @param f	- Floating-Point value, stored as a double
	 * @param i	- Integer value, stored as a long int
	 * @param b	- Boolean value, stored as a bool
	 * @param n	- No value, exclusively for error-handling
	 */
	enum class type {
		f,	// float
		i,	// int
		b,	// bool
		n,	// none
	};

	// _name	- The unique name of this INI Setting. This should be descriptive of its function
	std::string _name;
	// _type	- The type of this INI Setting. Valid: (f | i | b)
	type _type;

	/** CONSTRUCTOR **
	 * INI_Setting(string, type)
	 * 
	 * @param myName	- This INI Setting's name
	 * @param myType	- This INI Setting's value type
	 */
	INI_Setting(std::string myName, type myType) : _name(myName), _type(myType) {};
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
	fINI_Setting(std::string myName, double myVal) : INI_Setting(myName, type::f), _value(myVal) {}

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
	iINI_Setting(std::string myName, long int myVal) : INI_Setting(myName, type::i), _value(myVal) {}

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
	bINI_Setting(std::string myName, bool myVal) : INI_Setting(myName, type::b), _value(myVal) {}

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