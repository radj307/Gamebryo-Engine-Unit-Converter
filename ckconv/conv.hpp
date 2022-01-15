/**
 * @file	unit.hpp
 * @author	radj307
 * @brief	Contains real-world measurement unit converters, currently supporting metric & imperial.
 */
#include <sysarch.h>
#include <make_exception.hpp>
#include <str.hpp>
#include <var.hpp>
#include <math.hpp>
#include <TermAPI.hpp>

#include <optional>
#include <algorithm>

namespace ckconv {
	/**
	 * @enum	System
	 * @brief	Accepted Measurement Systems
	 */
	enum class System : char {
		METRIC,
		IMPERIAL,
		CREATIONKIT,
	};

	/**
	 * @struct	Metric
	 * @brief	Intra-Metric-System Conversion Factors
	 */
	static struct { // System::METRIC
		enum class MetricPowers : char {
			PICOMETER = -12,
			NANOMETER = -9,
			MICROMETER = -6,
			MILLIMETER = -3,
			CENTIMETER = -2,
			DECIMETER = -1,
			METER = 0,
			DECAMETER = 1,
			HECTOMETER = 2,
			KILOMETER = 3,
			MEGAMETER = 6,
			GIGAMETER = 9,
			TERAMETER = 12,
		};

		using T = long double;

		const T PICOMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::PICOMETER)) };
		const T NANOMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::NANOMETER)) };
		const T MICROMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::MICROMETER)) };
		const T MILLIMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::MILLIMETER)) };
		const T CENTIMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::CENTIMETER)) };
		const T DECIMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::DECIMETER)) };
		const T METER{ std::pow(10.0l, static_cast<long double>(MetricPowers::METER)) };
		const T DECAMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::DECAMETER)) };
		const T HECTOMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::HECTOMETER)) };
		const T KILOMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::KILOMETER)) };
		const T MEGAMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::MEGAMETER)) };
		const T GIGAMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::GIGAMETER)) };
		const T TERAMETER{ std::pow(10.0l, static_cast<long double>(MetricPowers::TERAMETER)) };

		const T* base{ &METER };
	} Metric;

	/**
	 * @struct	Imperial
	 * @brief	Intra-Imperial-System Conversion Factors
	 */
	static struct { // System::IMPERIAL
		using T = long double;

		const T INCH{ 1.0l / 12.0l };
		const T FOOT{ 1.0l };
		const T YARD{ 3.0l };

		const T* base{ &FOOT };
	} Imperial;

	static struct { // System::CREATIONKIT
		using T = long double;
		const T UNIT{ 1.0l };

		const T* base{ &UNIT };
	} CreationKit;

	/// @brief	Inter-System (Metric:Imperial) Conversion Factor
	const constexpr auto ONE_FOOT_IN_METERS{ 0.3048 };
	/// @brief	Inter-System (CKUnit:Metric) Conversion Factor
	const constexpr auto ONE_UNIT_IN_METERS{ 0.0142875313 }, ONE_UNIT_IN_FEET{ 0.046875 };

	/**
	 * @struct	Unit
	 * @brief	POD Structure with System & conversion factor members used to represent a measurement unit.
	 */
	struct Unit {
		System system;
		long double unitcf; // unit conversion factor

		/// @brief	Retrieve the given value in it's base form.
		long double base(const long double& val) const
		{
			return val * unitcf;
		}
		CONSTEXPR bool operator==(const Unit& o) const
		{
			return system == o.system && unitcf == o.unitcf;
		}
		CONSTEXPR operator System() const { return system; }
		CONSTEXPR operator long double() const { return unitcf; }
	};

	/**
	 * @brief			Converts between units in one measurement system.
	 * @param in_unit	Input Conversion Factor
	 * @param v			Input Value
	 * @param out_unit	Output Conversion Factor
	 * @returns			long double
	 */
	inline constexpr long double convert_unit(const long double& in_unit, const long double& v, const long double& out_unit)
	{
		if (math::equal(out_unit, 0.0l))
			throw make_exception("convert_unit() failed:  Cannot divide by zero!");
		return ((v * in_unit) / out_unit);
	}
	/**
	 * @brief				Convert between measurement systems.
	 * @param in_system		Input Measurement System
	 * @param v_base		Input Value, in the input system's base unit. (Metric = Meters, Imperial = Feet)
	 * @param out_system	Output Measurement System
	 * @returns				long double
	 */
	inline constexpr long double convert_system(const System& in_system, const long double& v_base, const System& out_system)
	{
		if (in_system == out_system) // same system
			return v_base;
		// convert system
		switch (in_system) {
		case System::METRIC:
			switch (out_system) { // METRIC ->
			case System::IMPERIAL:
				return v_base / ONE_FOOT_IN_METERS;
			case System::CREATIONKIT:
				return v_base / ONE_UNIT_IN_METERS;
			}
			break;
		case System::IMPERIAL:
			switch (out_system) { // IMPERIAL ->
			case System::METRIC:
				return v_base * ONE_FOOT_IN_METERS;
			case System::CREATIONKIT:
				return v_base / ONE_UNIT_IN_FEET;
			}
			break;
		case System::CREATIONKIT:
			switch (out_system) { // CREATIONKIT ->
			case System::METRIC:
				return v_base * ONE_UNIT_IN_METERS;
			case System::IMPERIAL:
				return v_base * ONE_UNIT_IN_FEET;
			}
			break;
		default:break;
		}
		throw make_exception("convert_system() failed:  No handler exists for the given input type!");
	}

	/**
	 * @brief		Convert a number in a given unit to another unit and/or system.
	 * @param in	Input Unit.
	 * @param val	Input Value.
	 * @param out	Output Unit.
	 * @returns		long double
	 */
	template<var::arithmetic T>
	inline static constexpr long double convert(const Unit& in, const T& val, const Unit& out)
	{
		if (in.unitcf == 0.0l)
			throw make_exception("Illegal input conversion factor");
		if (out.unitcf == 0.0l)
			throw make_exception("Illegal output conversion factor");

		if (in.system == out.system) // convert between units only
			return convert_unit(in.unitcf, val, out.unitcf);
		// Convert between systems & units
		return convert_system(in.system, in.base(static_cast<long double>(val)), out.system) / out.unitcf;
	}

	/**
	 * @brief		Retrieve the unit specified by a string containing the unit's official symbol, or name.
	 * @param str	Input String. (This is not processed beyond case-conversion)
	 * @param def	Optional default return value if the string is invalid.
	 * @returns		Unit
	 */
	inline Unit getUnit(const std::string& str, const std::optional<Unit>& def = std::nullopt)
	{
		if (str.empty()) {
			if (def.has_value())
				return def.value();
			throw make_exception("No unit specified ; string was empty!");
		}
		const auto s{ str::tolower(str) };

		// check imperial
		// negative
		if (str == "in" || s == "i" || str::matches_min_any<false>(s, 4, "inch", "inches"))
			return Unit{ System::IMPERIAL, Imperial.INCH };
		// positive
		if (str == "yd" || s == "y" || str::matches_min_any<false>(s, 4, "yard", "yards"))
			return Unit{ System::IMPERIAL, Imperial.YARD };
		// base
		if (str == "ft" || s == "f" || str::matches_min_any<false>(s, 4, "foot", "feet"))
			return Unit{ System::IMPERIAL, Imperial.FOOT };

		// check metric (str == (symbol) || str == (name))
		// negative
		if (str == "pm" || str::matches_min_any<false>(s, 7, "picometer"))
			return Unit{ System::METRIC, Metric.PICOMETER };
		if (str == "nm" || str::matches_min_any<false>(s, 7, "nanometer"))
			return Unit{ System::METRIC, Metric.NANOMETER };
		if (str == "µm" || str == "um" || str::matches_min_any<false>(s, 8, "micrometer"))
			return Unit{ System::METRIC, Metric.MICROMETER };
		if (str == "mm" || str::matches_min_any<false>(s, 8, "millimeter"))
			return Unit{ System::METRIC, Metric.MILLIMETER };
		if (str == "cm" || str::matches_min_any<false>(s, 8, "centimeter"))
			return Unit{ System::METRIC, Metric.CENTIMETER };
		if (str == "dm" || str::matches_min_any<false>(s, 7, "decimeter"))
			return Unit{ System::METRIC, Metric.DECIMETER };
		// positive
		if (str == "dam" || str::matches_min_any<false>(s, 7, "decameter"))
			return Unit{ System::METRIC, Metric.DECAMETER };
		if (str == "hm" || str::matches_min_any<false>(s, 8, "hectometer"))
			return Unit{ System::METRIC, Metric.HECTOMETER };
		if (str == "km" || str::matches_min_any<false>(s, 7, "kilometer"))
			return Unit{ System::METRIC, Metric.KILOMETER };
		if (str == "Mm" || str::matches_min_any<false>(s, 7, "megameter"))
			return Unit{ System::METRIC, Metric.MEGAMETER };
		if (str == "Gm" || str::matches_min_any<false>(s, 7, "gigameter"))
			return Unit{ System::METRIC, Metric.GIGAMETER };
		if (str == "Tm" || str::matches_min_any<false>(s, 7, "terameter"))
			return Unit{ System::METRIC, Metric.TERAMETER };
		// base
		if (str == "m" || str::matches_min_any<false>(s, 3, "meter"))
			return Unit{ System::METRIC, Metric.METER };

		if (str == "u" || str == "un" || str::matches_min_any<false>(s, 4, "unit"))
			return Unit{ System::CREATIONKIT, CreationKit.UNIT };

		if (def.has_value())
			return def.value();

		throw make_exception("Unrecognized Unit: \"", str, '\"');
	}
}
