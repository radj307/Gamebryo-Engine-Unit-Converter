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

		const T PICOMETER{ std::pow(10.0L, FP(Powers::PICOMETER)) };
		const T NANOMETER{ std::pow(10.0L, FP(Powers::NANOMETER)) };
		const T MICROMETER{ std::pow(10.0L, FP(Powers::MICROMETER)) };
		const T MILLIMETER{ std::pow(10.0L, FP(Powers::MILLIMETER)) };
		const T CENTIMETER{ std::pow(10.0L, FP(Powers::CENTIMETER)) };
		const T DECIMETER{ std::pow(10.0L, FP(Powers::DECIMETER)) };
		const T METER{ std::pow(10.0L, FP(Powers::METER)) };
		const T DECAMETER{ std::pow(10.0L, FP(Powers::DECAMETER)) };
		const T HECTOMETER{ std::pow(10.0L, FP(Powers::HECTOMETER)) };
		const T KILOMETER{ std::pow(10.0L, FP(Powers::KILOMETER)) };
		const T MEGAMETER{ std::pow(10.0L, FP(Powers::MEGAMETER)) };
		const T GIGAMETER{ std::pow(10.0L, FP(Powers::GIGAMETER)) };
		const T TERAMETER{ std::pow(10.0L, FP(Powers::TERAMETER)) };

		// the base unit of the Metric system (meters)
		const T* const base{ &METER };
	} Metric;

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

		const T PICOUNIT{ std::pow(10.0L, FP(Powers::PICOUNIT)) };
		const T NANOUNIT{ std::pow(10.0L, FP(Powers::NANOUNIT)) };
		const T MICROUNIT{ std::pow(10.0L, FP(Powers::MICROUNIT)) };
		const T MILLIUNIT{ std::pow(10.0L, FP(Powers::MILLIUNIT)) };
		const T CENTIUNIT{ std::pow(10.0L, FP(Powers::CENTIUNIT)) };
		const T DECIUNIT{ std::pow(10.0L, FP(Powers::DECIUNIT)) };
		const T UNIT{ std::pow(10.0L, FP(Powers::UNIT)) };
		const T DECAUNIT{ std::pow(10.0L, FP(Powers::DECAUNIT)) };
		const T HECTOUNIT{ std::pow(10.0L, FP(Powers::HECTOUNIT)) };
		const T KILOUNIT{ std::pow(10.0L, FP(Powers::KILOUNIT)) };
		const T MEGAUNIT{ std::pow(10.0L, FP(Powers::MEGAUNIT)) };
		const T GIGAUNIT{ std::pow(10.0L, FP(Powers::GIGAUNIT)) };
		const T TERAUNIT{ std::pow(10.0L, FP(Powers::TERAUNIT)) };

		// the base unit of this system
		const T* const base{ &UNIT };
	} CreationKit;

	/**
	 * @struct	Imperial
	 * @brief	Intra-Imperial-System Conversion Factors
	 */
	static struct { // SystemID::IMPERIAL
		using T = long double;
		// regular length units
		const T TWIP{ 1.0L / 17280.0L };
		const T THOU{ 1.0L / 12000.0L };
		const T BARLEYCORN{ 1.0L / 36.0L };
		const T INCH{ 1.0L / 12.0L };
		const T HAND{ 1.0L / 3.0L };
		const T FOOT{ 1.0L };
		const T YARD{ 3.0L };
		const T CHAIN{ 66.0L };
		const T FURLONG{ 660.0L };
		const T MILE{ 5280.0L };
		const T LEAGUE{ 15840.0L };
		// maritime units
		const T FATHOM{ 6.0761L };
		const T CABLE{ 607.61L };
		const T NAUTICAL_MILE{ 6076.1L };
		// 17th century onwards
		const T LINK{ 66.0L / 100.0L };
		const T ROD{ 66.0L / 4.0L };
		// the base unit of this system
		const T* const base{ &FOOT };
	} Imperial;

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
		std::string sym;

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

		friend std::ostream& operator<<(std::ostream& os, const Unit& unit)
		{
			return os << unit.sym;
		}
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
		if (math::equal(out_unit, 0.0L))
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
		if (in.unitcf == 0.0L)
			throw make_exception("Illegal input conversion factor");
		if (out.unitcf == 0.0L)
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

		//#define DISABLE_NUTJOB_UNITS

		// BEGIN IMPERIAL //
		#ifndef DISABLE_NUTJOB_UNITS
		if (s.find("twip") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.TWIP, "twip(s)" };
		if (str == "th" || s.find("thou") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.THOU, "th"};
		if (str == "Bc" || s.find("barleycorn") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.BARLEYCORN, "Bc" };
		if (str == "h" || s.find("hand") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.HAND, "h" };
		if (str == "ch" || s.find("chain") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.CHAIN, "ch" };
		if (str == "fur" || s.find("furlong") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.FURLONG, "fur" };
		if (str == "lea" || s.find("league") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.LEAGUE, "lea"};
		if (str == "ftm" || s.find("fathom") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.FATHOM, "ftm"};
		if (s.find("cable") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.CABLE, "cable(s)"};
		if (s.find("link") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.LINK, "link(s)"};
		if (str == "rd" || s.find("rod") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.ROD, "rd" };
		#endif // DISABLE_NUTJOB_UNITS
		if (str == "in" || s == "i" || s.find("inch") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.INCH, "\""};
		if (str == "ft" || s == "f" || s.find("foot") < s.size() || s.find("feet") < s.size())
			return Unit{ SystemID::IMPERIAL, *Imperial.base, "\'"};
		if (str == "yd" || s.find("yard") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.YARD, "yd"};
		if (str == "nmi" || s.find("nauticalmile") < s.size() || s.find("nmile") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.CABLE, "nmi" };
		if (str == "mi" || s.find("mile") < s.size())
			return Unit{ SystemID::IMPERIAL, Imperial.MILE, "mi"};
		// END IMPERIAL //
		
		// BEGIN METRIC //
		// comparisons omit -er|-re to allow both the American and British spelling of "meter|metre".
		if (str == "pm" || s.find("picomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.PICOMETER, "pm"};
		if (str == "nm" || s.find("nanomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.NANOMETER, "nm"};
		if (str == "um" || s.find("micromet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MICROMETER, "µm"};
		if (str == "mm" || s.find("millimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MILLIMETER, "mm"};
		if (str == "cm" || s.find("centimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.CENTIMETER, "cm"};
		if (str == "dm" || s.find("decimet") < s.size())
			return Unit{ SystemID::METRIC, Metric.DECIMETER, "dm"};
		if (str == "dam" || s.find("decamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.DECAMETER, "dam"};
		if (str == "hm" || s.find("hectomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.HECTOMETER, "hm"};
		if (str == "km" || s.find("kilomet") < s.size())
			return Unit{ SystemID::METRIC, Metric.KILOMETER, "km"};
		if (str == "Mm" || s.find("megamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.MEGAMETER, "Mm"};
		if (str == "Gm" || s.find("gigamet") < s.size())
			return Unit{ SystemID::METRIC, Metric.GIGAMETER, "Gm"};
		if (str == "Tm" || s.find("teramet") < s.size())
			return Unit{ SystemID::METRIC, Metric.TERAMETER, "Tm"};
		// this has to be checked after all prefix types
		if (str == "m" || s.find("met") < s.size())
			return Unit{ SystemID::METRIC, *Metric.base, "m"};
		// END METRIC //

		// BEGIN CREATIONKIT //
		if (str == "pu" || s.find("picounit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.PICOUNIT, "pu"};
		if (str == "nu" || s.find("nanounit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.NANOUNIT, "nu"};
		if (str == "uu" || s.find("microunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.MICROUNIT, "µu"};
		if (str == "mu" || s.find("milliunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.MILLIUNIT, "mu"};
		if (str == "cu" || s.find("centiunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.CENTIUNIT, "cu"};
		if (str == "du" || s.find("deciunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.DECIUNIT, "du"};
		if (str == "dau" || s.find("decaunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.DECAUNIT, "dau"};
		if (str == "hu" || s.find("hectounit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.HECTOUNIT, "hu"};
		if (str == "ku" || s.find("kilounit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.KILOUNIT, "ku"};
		if (str == "Mu" || s.find("megaunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.MEGAUNIT, "Mu"};
		if (str == "Gu" || s.find("gigaunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.GIGAUNIT, "Gu"};
		if (str == "Tu" || s.find("teraunit") < s.size())
			return Unit{ SystemID::CREATIONKIT, CreationKit.TERAUNIT, "Tu"};
		// this has to be checked after all prefix types
		if (str == "u" || s.find("unit") < s.size())
			return Unit{ SystemID::CREATIONKIT, *CreationKit.base, "u"};
		// END CREATIONKIT //

		if (def.has_value())
			return def.value();

		throw make_exception("Unrecognized Unit: \"", str, '\"');
	}
}
