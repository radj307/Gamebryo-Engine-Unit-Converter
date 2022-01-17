#include "conv.hpp"
#include "Global.h"
using namespace ckconv;

#include <math.hpp>
#include <envpath.hpp>

/**
 * @struct	Convert
 * @brief	Performs a single conversion operation, and exposes std::ostream operator<<() to format and insert it into an output stream.
 */
struct Convert {
	/// @brief	String Tuple
	using Tuple = std::tuple<ckconv::Unit, long double, ckconv::Unit>;
	using StrTuple = std::tuple<std::string, std::string, std::string>;
	using NumberT = long double;
private:
	Tuple _vars;
	std::streamsize _min_indent{ 0ull };

	///	@brief	Sorts the first & second arguments so that they are in the correct order when passed to the converter. Also removes any commas.
	static inline Tuple convert_tuple(std::tuple<std::string, std::string, std::string>&& tpl)
	{
		auto& [first, second, third] { tpl };

		// swap the first & second args if the first argument is the value
		if (std::all_of(first.begin(), first.end(), [](auto&& ch) { return isdigit(ch) || ch == '.' || ch == '-' || ch == ','; })) {
			const auto copy{ second };
			second = str::strip(first, ',');
			first = copy;
		}
		else second = str::strip(second, ',');

		return{ ckconv::getUnit(first), str::stold(second), ckconv::getUnit(third) };
	}

	///	@brief	Returns the result of the conversion.
	static inline NumberT getResult(const ckconv::Unit& input_unit, const NumberT& input, const ckconv::Unit& output_unit) noexcept(false)
	{
		if (math::equal(input, 0.0l)) // if input is 0, short-circuit and return 0
			return 0.0l;
		if (input_unit == output_unit)
			return input;
		return ckconv::convert(input_unit, input, output_unit);
	}

public:
	/// @brief	Default constructor
	Convert(std::tuple<std::string, std::string, std::string>&& vars, const std::streamsize& min_indent = 0ull) : _vars{ std::move(convert_tuple(std::move(vars))) }, _min_indent{ min_indent } {}
	/**
	 * @brief			Constructor
	 * @param unit_in	Input Unit (OR Input Value, if val_in is the input unit)
	 * @param val_in	Input Value (OR Input Unit, if unit_in is the input value)
	 * @param unit_out	Output Unit
	 */
	Convert(const std::string& unit_in, const std::string& val_in, const std::string& unit_out, const std::streamsize& min_indent = 0ull) : Convert(std::move(std::make_tuple(unit_in, val_in, unit_out)), min_indent) {}

	NumberT operator()() const { return getResult(std::get<0>(_vars), std::get<1>(_vars), std::get<2>(_vars)); }

	/**
	 * @brief	Format and print the result of the conversion to the given ostream instance.
	 *\n		This may change the floatfield and/or precision of the output stream.
	 * @returns	std::ostream&
	 */
	friend std::ostream& operator<<(std::ostream& os, const Convert& conv)
	{
		using ckconv::operator<<;
		// get inputs
		const auto& [input_unit, input, output_unit] {conv._vars};

		if (!Global.quiet) {
			const auto
				input_unit_str{ str::stringify(configure_ostream, input_unit) },
				input_str{ str::stringify(configure_ostream, input) };

			os // insert input
				<< Global.palette.set(OUT::INPUT_VALUE) << input_str << Global.palette.reset()
				<< ' '
				<< Global.palette.set(OUT::INPUT_UNIT) << input_unit_str << Global.palette.reset()
				<< str::VIndent(conv._min_indent, (input_str.size() + input_unit_str.size() + 1ull))
				<< Global.palette.set(OUT::EQUALS) << '=' << Global.palette.reset() << ' ';
		}

		const NumberT output{ conv.getResult(input_unit, input, output_unit) };
		const auto output_str{ str::stringify(configure_ostream, output) };

		os << Global.palette.set(OUT::OUTPUT_VALUE) << output_str << Global.palette.reset();

		if (!Global.quiet)
			os << ' ' << Global.palette.set(OUT::OUTPUT_UNIT) << output_unit << Global.palette.reset();

		return os;
	}
};

#ifndef OS_WIN
/**
 * @brief		Check if the given read file descriptor has pending data.
 * @param fd	The target file descriptor. (STDIN = 0)
 * @param s		Timeout in seconds.
 * @param us	Additional timeout in nanoseconds.
 * @returns		bool
 *\n			true	There is pending input.
 *\n			false	There is no pending input.
 */
INLINE bool hasPending(const int& fd)
{
	struct timespec timeout { 0L, 0L };
	fd_set fds; // create a file descriptor set
	FD_ZERO(&fds);
	FD_SET(fd, &fds); // stdin file descriptor is 0
	fflush(NULL); // flush input buffer
	return pselect(fd + 1, &fds, nullptr, nullptr, &timeout, nullptr) == 1;
}
#else
// Dummy function for windows that always returns false
INLINE CONSTEXPR bool hasPending(const int& fd) { return false; }
#endif

/**
 * @brief	Read all available input from STDIN.
 * @returns	std::vector<std::string>
 */
INLINE std::vector<std::string> read_all_stdin()
{
	// lambda that gets the next word from STDIN
	const auto& getNextWord{ [&]() -> std::string {
		std::string s;
		if (std::cin.good())
			str::getline(std::cin, s, ' ', '\n', '\t');
		return s;
	} };
	std::vector<std::string> vec;
	vec.reserve(24);
	while (std::cin.good() && hasPending(0))
		if (auto word{ getNextWord() }; !word.empty())
			vec.emplace_back(std::move(word));
	vec.shrink_to_fit();
	return vec;
}

/**
 * @brief		Concatenate 2 string vectors.
 * @param fst	First Vector
 * @param snd	Second Vector
 * @returns		std::vector<std::string>
 */
template<typename T> [[nodiscard]]
INLINE std::vector<T> catvec(const std::vector<T>& fst, const std::vector<T>& snd)
{
	std::vector<T> vec;
	vec.reserve(fst.size() + snd.size());
	for (auto& it : fst)
		vec.emplace_back(it);
	for (auto& it : snd)
		vec.emplace_back(it);
	vec.shrink_to_fit();
	return vec;
}

/**
 * @brief		This function only exists because windows doesn't allow checking if STDIN has pending data.
 * @param args	Arguments
 * @returns		std::vector<std::string>
 */
[[nodiscard]] INLINE std::vector<std::string> get_conversion_parameters(const opt::ParamsAPI2& args)
{
	#ifndef OS_WIN // if not using windows, read from stdin
	return catvec(read_all_stdin(), args.typegetv_all<opt::Parameter>());
	#else // Windows:
	const auto params{ args.typegetv_all<opt::Parameter>() };
	if (args.check<opt::Flag>('P'))
		return catvec(read_all_stdin(), params);
	else return params;
	#endif
}

int main(const int argc, char** argv)
{
	int rc{ -1 };
	try {
		// parse arguments
		opt::ParamsAPI2 args{ argc, argv, 'p', "precision", 'a', "align-to" };
		// find the program's location
		const auto [program_path, program_name] { env::PATH().resolve_split(argv[0]) };

		// Set the ini path
		Global.ini_path = program_path / std::filesystem::path{ program_name }.replace_extension("ini");

		// handle help argument
		if ((args.empty() && !hasPending(0)) || args.check_any<opt::Flag, opt::Option>('h', "help")) {
			write_help(std::cout, program_name.generic_string());
			return 0;
		}
		// handle version argument
		else if (args.check_any<opt::Flag, opt::Option>('v', "version")) {
			std::cout << program_name.generic_string() << " v" << ckconv_VERSION << std::endl;
			return 0;
		}
		// set ini
		else if (args.check_any<opt::Option>("set-ini", "ini-set")) {
			handle_args(args);
			write_settings_to_config();
			return 0;
		}
		// units
		else if (args.check_any<opt::Flag, opt::Option>('u', "units")) {
			if (const auto& units{ args.typegetv_any<opt::Flag, opt::Option>('u', "units") }; units.has_value()) {
				const auto s{ str::tolower(units.value()) };
				if (s == "m" || s == "metric") {
					std::cout << ckconv::PrintableMeasurementUnits<SystemID::METRIC>() << std::endl;
					return 0;
				}
				else if (s == "i" || s == "imperial") {
					std::cout << ckconv::PrintableMeasurementUnits<SystemID::IMPERIAL>() << std::endl;
					return 0;
				}
				else if (s == "creationkit" || s == "creation kit" || s == "ck" || s == "gamebryo" || s == "engine" || s == "unit" || s == "units" || s == "un" || s == "u") {
					std::cout << ckconv::PrintableMeasurementUnits<SystemID::CREATIONKIT>() << std::endl;
					return 0;
				}
			}
			std::cout << ckconv::PrintableMeasurementUnits<SystemID::ALL>() << std::endl;
			return 0;
		}

		// read the ini if it exists
		if (file::exists(Global.ini_path))
			handle_config(file::INI(Global.ini_path));

		handle_args(args);

		const auto params{ get_conversion_parameters(args) };

		// Hidden debug option to dump all parameters to STDOUT
		if (args.check<opt::Option>("debug-dump-all")) {
			for (auto& it : params)
				std::cout << term::debug << '\"' << it << "\"\n";
			return 0;
		}

		// get all parameters
		if (params.empty())
			throw make_exception("Nothing to do.");

		// lambda to retrieve the current argument & the next 2 arguments as a string tuple. Does NOT check for out-of-range iterators!
		const auto& getArgTuple{ [&params](decltype(params)::const_iterator& it) -> Convert::StrTuple {
			const auto first{ *it }; // these have to be defined in long form or operator++ causes them to be in the wrong order.
			const auto second{ *++it };
			const auto third{ *++it };
			return{ first, second, third };
		} };

		for (auto it{ params.begin() }; it < params.end(); ++it)
			if (std::distance(it, params.end()) > 2ull) // if there are at least 2 more arguments after this one
				std::cout << Convert(getArgTuple(it), Global.align_to_column) << '\n';

		rc = 0;
	} catch (const std::exception& ex) {
		std::cerr << term::error << ex.what() << std::endl;
	} catch (...) {
		std::cerr << term::crit << "An undefined exception occurred!" << std::endl;
	}
	return rc;
}