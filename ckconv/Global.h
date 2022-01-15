#pragma once
#include "version.h"

#include <sysarch.h>
#include <TermAPI.hpp>
#include <palette.hpp>
#include <ParamsAPI2.hpp>
#include <xlog.hpp>
#include <INI.hpp>

#include <variant>
#include <filesystem>

INLINE CONSTEXPR const auto DEFAULT_NAME{ "ckconv.exe" };
INLINE CONSTEXPR const auto HEADER_OUTPUT{ "output" };

/**
 * @enum	OUT
 */
enum class OUT : char {
	INPUT_UNIT,
	INPUT_VALUE,
	OUTPUT_UNIT,
	OUTPUT_VALUE,
	EQUALS,
};

static struct {
	term::palette<OUT> palette{
		std::make_pair(OUT::INPUT_UNIT,		term::setcolor(color::white, color::format::BOLD)),
		std::make_pair(OUT::INPUT_VALUE,	term::setcolor(color::green)),
		std::make_pair(OUT::OUTPUT_UNIT,	term::setcolor(color::white, color::format::BOLD)),
		std::make_pair(OUT::OUTPUT_VALUE,	term::setcolor(color::green)),
		std::make_pair(OUT::EQUALS,			term::setcolor(color::white)),
	};
	xlog::xLogs<std::ostream> log;

	#ifdef OS_WIN
	using FmtFlag = std::_Iosb<int>::_Fmtflags;
	#else
	using FmtFlag = std::ios_base::fmtflags;
	#endif

	inline static constexpr const FmtFlag* const FIXED{ &std::ios_base::fixed }, * SCIENTIFIC{ &std::ios_base::scientific };

	std::filesystem::path ini_path;

	// CONFIG VALUES:

	std::streamsize precision{ 6ull };
	const FmtFlag* notation{ nullptr };
	bool quiet{ false };
	bool no_color{ false };
} Global;

inline void write_help(std::ostream& os, const std::string_view& program_name)
{
	os << program_name << ' '; // print current program name
	if (program_name != DEFAULT_NAME) // print original program name if modified
		os << "(" << DEFAULT_NAME << ") ";
	os
		<< 'v' << ckconv_VERSION << '\n' // print current program version number
		<< "  Converts between Metric, Imperial, and CreationKit length units.\n"
		<< '\n'
		<< "USAGE:\n"
		<< "  " << program_name << " [OPTIONS] [<INPUT_UNIT> <INPUT_VALUE> <OUTPUT_UNIT>]...\n"
		<< '\n'
		<< "OPTIONS:\n"
		<< "  " << "-h  --help                     Show the help display and exit." << '\n'
		<< "  " << "-v  --version                  Show the current version number and exit." << '\n'
		<< "  " << "--standard                     Force standard notation." << '\n'
		<< "  " << "--scientific                   Force scientific notation." << '\n'
		<< "  " << "-p <INT>  --precision <INT>    Set the number of digits after the decimal point to print. (Default: 6)" << '\n'
		<< "  " << "-q  --quiet                    Don't print input values/units." << '\n'
		<< "  " << "-n  --no-color                 Don't use color escape sequences." << '\n'
		<< "  " << "--reset-ini                    Create or overwrite the config with the current configuration. (Sensitive to other arguments)" << '\n'
		;
}
inline void handle_args(const opt::ParamsAPI2& args)
{
	// precision
	if (const auto precision{ args.typegetv_any<opt::Flag, opt::Option>('p', "precision") }; precision.has_value())
		Global.precision = str::stoll(precision.value());

	// notation
	if (args.check<opt::Option>("standard"))
		Global.notation = Global.FIXED;
	else if (args.check<opt::Option>("scientific"))
		Global.notation = Global.SCIENTIFIC;

	// quiet
	Global.quiet = args.check_any<opt::Flag, opt::Option>('q', "quiet");

	if (args.check_any<opt::Flag, opt::Option>('n', "no-color"))
		Global.palette.setActive(false);

	// reset-ini
	if (args.check<opt::Option>("reset-ini")) {
		if (file::INI(file::ini::INIContainer::Map{
			{
				std::string{HEADER_OUTPUT}, file::ini::INI::SectionContent{
					{ "precision", file::ini::VariableT{ static_cast<file::ini::Integer>(Global.precision) }},
					{ "notation", file::ini::VariableT{ "" } },
					{ "quiet", file::ini::VariableT{ Global.quiet } },
					{ "no-color", file::ini::VariableT{ false } },
				}
			},
			}).write(Global.ini_path)) {
			std::cout << "";
		}
	}
}
inline void handle_config(const file::INI& ini)
{
	// precision
	if (const auto precision{ ini.getv(HEADER_OUTPUT, "precision") }; precision.has_value())
		Global.precision = std::visit([](auto&& val) -> std::streamsize {
		using T = std::decay_t<decltype(val)>;
		if constexpr (std::same_as<T, file::ini::Integer>)
			return val;
		else if constexpr (std::same_as<T, file::ini::Float>)
			return static_cast<std::streamsize>(val);
		else if constexpr (std::same_as<T, file::ini::String>)
			return str::stoll(val);
		else if constexpr (std::same_as<T, file::ini::Boolean> || std::same_as<T, std::monostate>)
			return Global.precision;
		else static_assert(var::false_v, "Visitor cannot handle all potential type cases!");
			}, precision.value());

	// notation
	if (const auto notation{ ini.getvs(HEADER_OUTPUT, "notation") }; notation.has_value()) {
		const auto lc{ str::tolower(notation.value()) };
		if (lc == "fixed")
			Global.notation = Global.FIXED;
		else if (lc == "scientific")
			Global.notation = Global.SCIENTIFIC;
	}

	// quiet
	Global.quiet = ini.checkv(HEADER_OUTPUT, "quiet", true);

	// no-color
	if (ini.checkv(HEADER_OUTPUT, "no-color", true)) {
		Global.no_color = true;
		Global.palette.setActive(false);
		Global.log.setColorEnabled(false);
	}
}

