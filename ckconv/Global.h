#pragma once
#include "version.h"

#include <sysarch.h>
#include <TermAPI.hpp>
#include <palette.hpp>
#include <ParamsAPI2.hpp>
#include <INI.hpp>
#include <fileutil.hpp>

#include <variant>
#include <filesystem>
#include <cmath>

#ifdef OS_WIN
/// @brief	This is used to keep track of the number output notation.
using FmtFlag = std::_Iosb<int>::_Fmtflags;
#else
/// @brief	This is used to keep track of the number output notation.
using FmtFlag = std::ios_base::fmtflags;
#endif

INLINE CONSTEXPR const FmtFlag* const FIXED{ &std::ios_base::fixed }, * SCIENTIFIC{ &std::ios_base::scientific };

namespace ckconv {

	/// @brief	Default executable name.
	INLINE CONSTEXPR const auto DEFAULT_NAME{ "ckconv.exe" };

	/// @brief	INI Header that contains output-related settings.
	INLINE CONSTEXPR const auto HEADER_OUTPUT{ "output" }, HEADER_GLOBAL{ "" };

	/**
	 * @enum	OUT
	 * @brief	Corresponds to sections of output that should have the same colors.
	 */
	enum class OUT : char {
		INPUT_UNIT,
		INPUT_VALUE,
		OUTPUT_UNIT,
		OUTPUT_VALUE,
		EQUALS,
	};

	static struct {
		///	@brief	Palette instance containing each of the keys from OUT. This is used to allow disabling color sequences program-wide.
		term::palette<OUT> palette{
			std::make_pair(OUT::INPUT_UNIT,		term::setcolor(color::white, color::format::BOLD)),
			std::make_pair(OUT::INPUT_VALUE,	term::setcolor(color::green)),
			std::make_pair(OUT::OUTPUT_UNIT,	term::setcolor(color::white, color::format::BOLD)),
			std::make_pair(OUT::OUTPUT_VALUE,	term::setcolor(color::green)),
			std::make_pair(OUT::EQUALS,			term::setcolor(color::white)),
		};


		std::filesystem::path ini_path;

		// CONFIG VALUES:

		std::streamsize precision{ 6ull };
		const FmtFlag* notation{ nullptr };
		bool quiet{ false };
		bool no_color{ false };
	} Global;

	/**
	 * @brief				Write the help documentation to the given output stream.
	 * @param os			Output stream ref to write to.
	 * @param program_name	The current name of the executable.
	 */
	inline void write_help(std::ostream& os, const std::string_view& program_name) noexcept
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
			<< "  " << "-p <INT>  --precision <INT>    Force show at least <INT> number of digits after the decimal point." << '\n'
			<< "  " << "-q  --quiet                    Don't print input values/units." << '\n'
			<< "  " << "-n  --no-color                 Don't use color escape sequences." << '\n'
			<< "  " << "--reset-ini                    Create or overwrite the config with the current configuration. (Sensitive to other arguments)" << '\n'
			;
	}
	/**
	 * @brief		Handle all arguments except for help/version.
	 * @param args	The argument container instance.
	 */
	inline void handle_args(const opt::ParamsAPI2& args) noexcept(false)
	{
		// precision
		if (const auto precision{ args.typegetv_any<opt::Flag, opt::Option>('p', "precision") }; precision.has_value())
			Global.precision = str::stoll(precision.value());

		// notation
		if (args.check<opt::Option>("standard"))
			Global.notation = FIXED;
		else if (args.check<opt::Option>("scientific"))
			Global.notation = SCIENTIFIC;

		// quiet
		Global.quiet = args.check_any<opt::Flag, opt::Option>('q', "quiet");

		// no-color
		if (args.check_any<opt::Flag, opt::Option>('n', "no-color")) {
			Global.no_color = true;
			Global.palette.setActive(false);
		}

		// reset-ini | ini-reset
		if (args.check_any<opt::Option>("reset-ini", "ini-reset")) {
			const bool is_new{ !file::exists(Global.ini_path) };
			using section = file::ini::INIContainer::SectionContent;
			using var = file::ini::VariableT;
			if (file::INI(std::move(file::ini::INIContainer::Map{ {
					{
						std::string{HEADER_GLOBAL}, section {
							{ "version", file::ini::VariableT{ static_cast<file::ini::String>(ckconv_VERSION) } },
						}
					},
					{
						std::string{HEADER_OUTPUT}, section {
							{ "precision", var{ static_cast<file::ini::Integer>(Global.precision) } },
							{ "notation", var{ []() -> std::string {
									if (Global.notation != nullptr) {
										if (Global.notation == FIXED)
											return "fixed";
										else if (Global.notation == SCIENTIFIC)
											return "scientific";
									}
									return{};
								}()
							} },
							{ "quiet", var{ Global.quiet } },
							{ "no-color", var{ Global.no_color } },
						}
					},
				},
				})).write(Global.ini_path)) {
				std::cout << (Global.no_color ? "" : term::msg) << "Successfully " << (is_new ? "created" : "reset") << " config: " << Global.ini_path << std::endl;
			}
			else throw make_exception("Failed to ", (is_new ? "create" : "reset"), " config: ", Global.ini_path);
		}
	}

	/**
	 * @brief 
	 * @param strver 
	 */
	inline void handle_config_version(const std::string& strver) noexcept(false)
	{
		if (!strver.empty())
			if (const auto pos{ strver.find('.') }; pos < strver.size() && strver.substr(0ull, pos) == STRINGIZE(ckconv_VERSION_MAJOR))
				return;
		throw make_exception("Configuration file created with an incompatible build of ckconv:  ", Global.ini_path, "!  (Current version: ", ckconv_VERSION, ')');
	}

	/**
	 * @brief		Handle setting values from the configuration file.
	 * @param ini	The INI config instance.
	 */
	inline void handle_config(const file::INI& ini) noexcept(false)
	{
		// version
		handle_config_version(ini.getvs(HEADER_GLOBAL, "version").value_or(ckconv_VERSION));

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
			else static_assert(var::false_v<T>, "Visitor cannot handle all potential type cases!");
				}, precision.value());

		// notation
		if (const auto notation{ ini.getvs(HEADER_OUTPUT, "notation") }; notation.has_value()) {
			if (const auto lc{ str::tolower(notation.value()) }; lc == "fixed")
				Global.notation = FIXED;
			else if (lc == "scientific")
				Global.notation = SCIENTIFIC;
		}

		// quiet
		Global.quiet = ini.checkv(HEADER_OUTPUT, "quiet", true);

		// no-color
		if (ini.checkv(HEADER_OUTPUT, "no-color", true)) {
			Global.no_color = true;
			Global.palette.setActive(false);
		}
	}

	inline std::ostream& configure_ostream(std::ostream& os)
	{
		// override floatfield notation
		if (Global.notation != nullptr)
			os.setf(*Global.notation);

		// modify precision
		if (Global.precision != 6ll)
			os.precision(Global.precision);

		return os;
	}
}
