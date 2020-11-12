/****************************************************************************************
 *									INI_Defaults.h										*
 *		Contains the default INI settings used for validating INI settings.				*
 *									 by radj307											*
 *																						*
 *					  Include all expected INI settings below.							*
 *		   Any setting in the file that does not exist here will be ignored.			*
 *	 Any settings not found in the file that exist here will be added automatically.	*
 ****************************************************************************************/
#pragma once
#include <vector>			// for vectors
#include "INI_Setting.h"	// for INI_Settings

/** STATIC CONST **
 * vector<INI_Setting*>
 * Used for verifying loaded INI settings, and creating a new INI file with INI::write()
 * 
 * EXAMPLE:
 * new fINI_Setting("MySetting", 1.25, "Example Setting"),
 * new iINI_Setting("MySetting", 125, "Example Setting"),
 * 
 * WRITES:
 *	; Example Setting
 *	fMySetting			=	1.25
 *	; Example Setting
 *	iMySetting			=	125
 * 
 * Names should be less than 19 characters to be formatted correctly. (Longer names are fine, but don't appear inline)
 */
static const std::vector<INI_Setting*> _DEF = {
	new iINI_Setting("Precision", 6, "Defines how many digits to include after the decimal point."),
	new fINI_Setting("Factor_MI", 0.3048006096012191857, "Metric <-> Imperial Conversion Factor"),
	new fINI_Setting("Factor_UM", 0.01428753125, "Unit <-> Metric Conversion Factor"),
	new fINI_Setting("Factor_UI", 0.046875, "Unit <-> Imperial Conversion Factor"),
	new bINI_Setting("bAlwaysShowInput", false, "When true, always shows the full input value/type when converting to type \"all\""),
	new bINI_Setting("bDisableColors", false, "When true, disables colorized terminal output."),
};