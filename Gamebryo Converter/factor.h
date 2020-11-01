/**
 * const.h
 * Contains the conversion constants used for unit conversion
 * by radj307
 */
#pragma once

 /* meters <-> feet conversion factor
  * METERS	= feet * ratio_mi
  * FEET		= meters / ratio_mi
  */
static double __ratio_mi = 0.3048006096012191857;	// value from the U.S. Survey Foot (1893) - Not using U.S. International Foot as the difference is incredibly minor (2' per 1 million feet, or 2ppm), and few states actually use it.

/* units <-> meters conversion factor
 * METERS	= units * ratio_um
 * UNITS	= meters / ratio_um
 */
static double __ratio_um = 0.01428753125;// 0.0142822188; // value from converting imperial -> metric, then dividing metric / units. This is necessary as Bethesda's metric conversions are incorrect.

/* units <-> feet conversion factor
 * FEET		= units * ratio_ui
 * UNITS	= feet / ratio_ui
 */
static double __ratio_ui = 0.046875;	// value from Bethesda directly