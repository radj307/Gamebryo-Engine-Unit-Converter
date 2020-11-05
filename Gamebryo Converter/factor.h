/**
 * const.h
 * Contains the conversion constants used for unit conversion
 * by radj307
 */
#pragma once

// decimal precision
static int __PRECISION = 6;

 /* meters <-> feet conversion factor
  * METERS	= feet * ratio_mi
  * FEET		= meters / ratio_mi
  */
static long double __FACTOR_MI = 0.3048006096012191857;	// value from the U.S. Survey Foot (1893) - Not using U.S. International Foot as the difference is incredibly minor (2' per 1 million feet, or 2ppm), and few states actually use it.

/* units <-> meters conversion factor
 * METERS	= units * ratio_um
 * UNITS	= meters / ratio_um
 */
static long double __FACTOR_UM = 0.01428753125; // value from converting imperial -> metric, then dividing metric / units. This is necessary as Bethesda's metric conversions are incorrect.

/* units <-> feet conversion factor
 * FEET		= units * ratio_ui
 * UNITS	= feet / ratio_ui
 */
static long double __FACTOR_UI = 0.046875;	// value from Bethesda directly