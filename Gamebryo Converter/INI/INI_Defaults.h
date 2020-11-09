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
 */
static const std::vector<INI_Setting*> _DEF = {
	new iINI_Setting("iPrecision", 6),
	new fINI_Setting("fFactor_MI", 0.3048006096012191857),
	new fINI_Setting("fFactor_UM", 0.01428753125),
	new fINI_Setting("fFactor_UI", 0.046875),
};