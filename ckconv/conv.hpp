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
	 * @enum	SystemID
	 * @brief	Accepted Measurement Systems
	 */
	enum class SystemID : char {
		METRIC,
		IMPERIAL,
		CREATIONKIT,
	};

	/**
	 * @struct	Metric
	 * @brief	Intra-Metric-System Conversion Factors
	 */
	static struct { // SystemID::METRIC
		enum class Powers : char {
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
		static inline constexpr T FP(const Powers& p) { return static_cast<T>(p); }

		const T PICOMETER{ std::pow(10.0l, FP(Powers::PICOMETER)) };
		const T NANOMETER{ std::pow(10.0l, FP(Powers::NANOMETER)) };
		const T MICROMETER{ std::pow(10.0l, FP(Powers::MICROMETER)) };
		const T MILLIMETER{ std::pow(10.0l, FP(Powers::MILLIMETER)) };
		const T CENTIMETER{ std::pow(10.0l, FP(Powers::CENTIMETER)) };
		const T DECIMETER{ std::pow(10.0l, FP(Powers::DECIMETER)) };
		const T METER{ std::pow(10.0l, FP(Powers::METER)) };
		const T DECAMETER{ std::pow(10.0l, FP(Powers::DECAMETER)) };
		const T HECTOMETER{ std::pow(10.0l, FP(Powers::HECTOMETER)) };
		const T KILOMETER{ std::pow(10.0l, FP(Powers::KILOMETER)) };
		const T MEGAMETER{ std::pow(10.0l, FP(Powers::MEGAMETER)) };
		const T GIGAMETER{ std::pow(10.0l, FP(Powers::GIGAMETER)) };
		const T TERAMETER{ std::pow(10.0l, FP(Powers::TERAMETER)) };

		const T* base{ &METER };
	} Metric;

	/**
	 * @struct	Imperial
	 * @brief	Intra-Imperial-System Conversion Factors
	 */
	static struct { // SystemID::IMPERIAL
		using T = long double;

		const T INCH{ 1.0l / 12.0l };
		const T FOOT{ 1.0l };
		const T YARD{ 3.0l };

		const T* base{ &FOOT };
	} Imperial;

	static struct { // SystemID::CREATIONKIT
		enum class Powers : char {
			PICOUNIT = -12,
			NANOUNIT = -9,
			MICROUNIT = -6,
			MILLIUNIT = -3,
			CENTIUNIT = -2,
			DECIUNIT = -1,
			UNIT = 0,
			DECAUNIT = 1,
			HECTOUNIT = 2,
			KILOUNIT = 3,
			MEGAUNIT = 6,
			GIGAUNIT = 9,
			TERAUNIT = 12,
		};

		using T = long double;
		static inline constexpr T FP(const Powers& p) { return static_cast<T>(p); }

		const T PICOUNIT{ std::pow(10.0l, FP(Powers::PICOUNIT)) };
		const T NANOUNIT{ std::pow(10.0l, FP(Powers::NANOUNIT)) };
		const T MICROUNIT{ std::pow(10.0l, FP(Powers::MICROUNIT)) };
		const T MILLIUNIT{ std::pow(10.0l, FP(Powers::MILLIUNIT)) };
		const T CENTIUNIT{ std::pow(10.0l, FP(Powers::CENTIUNIT)) };
		const T DECIUNIT{ std::pow(10.0l, FP(Powers::DECIUNIT)) };
		const T UNIT{ std::pow(10.0l, FP(Powers::UNIT)) };
		const T DECAUNIT{ std::pow(10.0l, FP(Powers::DECAUNIT)) };
		const T HECTOUNIT{ std::pow(10.0l, FP(Powers::HECTOUNIT)) };
		const T KILOUNIT{ std::pow(10.0l, FP(Powers::KILOUNIT)) };
		const T MEGAUNIT{ std::pow(10.0l, FP(Powers::MEGAUNIT)) };
		const T GIGAUNIT{ std::pow(10.0l, FP(Powers::GIGAUNIT)) };
		const T TERAUNIT{ std::pow(10.0l, FP(Powers::TERAUNIT)) };

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
		SystemID system;
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
		CONSTEXPR operator SystemID() const { return system; }
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
	 * @param in_system		Input Measurement SystemID
	 * @param v_base		Input Value, in the input system's base unit. (Metric = Meters, Imperial = Feet)
	 * @param out_system	Output Measurement SystemID
	 * @returns				long double
	 */
	inline constexpr long double convert_system(const SystemID& in_system, const long double& v_base, const SystemID& out_system)
	{
		if (in_system == out_system) // same system
			return v_base;
		// convert system
		switch (in_system) {
		case SystemID::METRIC:
			switch (out_system) { // METRIC ->
			case SystemID::IMPERIAL:
				return v_base / ONE_FOOT_IN_METERS;
			case SystemID::CREATIONKIT:
				return v_base / ONE_UNIT_IN_METERS;
			}
			break;
		case SystemID::IMPERIAL:
			switch (out_system) { // IMPERIAL ->
			case SystemID::METRIC:
				return v_base * ONE_FOOT_IN_METERS;
			case SystemID::CREATIONKIT:
				return v_base / ONE_UNIT_IN_FEET;
			}
			break;
		case SystemID::CREATIONKIT:
			switch (out_system) { // CREATIONKIT ->
			case SystemID::METRIC:
				return v_base * ONE_UNIT_IN_METERS;
			case SystemID::IMPERIAL:
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

		// IMPERIAL
		// negative
		if (str == "in" || s == "i" || s.find("inch") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.INCH };
		// positive
		if (str == "yd" || s == "y" || s.find("yard") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.YARD };
		// base
		if (str == "ft" || s == "f" || s.find("foot") < s.size() || s.find("feet") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.FOOT };
		
		// METRIC (comparisons omit -er|-re to allow both the American and British spelling of "meter|metre".)
		// negative
		if (str == "pm" || s.find("picomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.PICOMETER };
		if (str == "nm" || s.find("nanomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.NANOMETER };
		if (str == "um" || s.find("micromet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MICROMETER };
		if (str == "mm" || s.find("millimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MILLIMETER };
		if (str == "cm" || s.find("centimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.CENTIMETER };
		if (str == "dm" || s.find("decimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.DECIMETER };
		// positive
		if (str == "dam" || s.find("decamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.DECAMETER };
		if (str == "hm" || s.find("hectomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.HECTOMETER };
		if (str == "km" || s.find("kilomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.KILOMETER };
		if (str == "Mm" || s.find("megamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MEGAMETER };
		if (str == "Gm" || s.find("gigamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.GIGAMETER };
		if (str == "Tm" || s.find("teramet") < s.size())
			return Unit{ SystemID::METRIC, Metric.TERAMETER };
		// base
		if (str == "m" || s.find("met") < s.size() || str::strip(s, 'm', 'e', 't', 'r', 's').empty())
			return Unit{ SystemID::METRIC, Metric.METER };

		// CREATIONKIT
		// base
		if (str == "u" || s.find("unit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.UNIT };

		if (def.has_value())
			return def.value();

		throw make_exception("Unrecognized Unit: \"", str, '\"');
	}
}
