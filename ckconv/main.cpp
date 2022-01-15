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
	using Tuple = std::tuple<std::string, std::string, std::string>;
	using NumberT = long double;
private:
	Tuple _vars;
	std::streamsize _min_indent{ 0ull };

	///	@brief	Sorts the first & second arguments so that they are in the correct order when passed to the converter. Also removes any commas.
	static inline Tuple&& sort_args(Tuple&& tpl)
	{
		auto& [first, second, third] { tpl };

		// swap the first & second args if the first argument is the value
		if (std::all_of(first.begin(), first.end(), [](auto&& ch) { return isdigit(ch) || ch == '.' || ch == '-' || ch == ','; })) {
			const auto copy{ second };
			second = str::strip(first, ',');
			first = copy;
		}
		else second = str::strip(second, ',');

		return std::move(tpl);
	}

	///	@brief	Returns the result of the conversion.
	NumberT getResult() const noexcept(false) 
	{ 
		const NumberT input{ str::stold(std::get<1>(_vars)) };
		if (math::equal(input, 0.0l)) // if input is 0, short-circuit and return 0
			return 0.0l;
		return ckconv::convert(ckconv::getUnit(std::get<0>(_vars)), input, ckconv::getUnit(std::get<2>(_vars)));
	}

public:
	/// @brief	Default constructor
	Convert(std::tuple<std::string, std::string, std::string>&& vars, const std::streamsize& min_indent = 0ull) : _vars{ sort_args(std::move(vars)) }, _min_indent{ min_indent } {}
	/**
	 * @brief			Constructor
	 * @param unit_in	Input Unit (OR Input Value, if val_in is the input unit)
	 * @param val_in	Input Value (OR Input Unit, if unit_in is the input value)
	 * @param unit_out	Output Unit
	 */
	Convert(const std::string& unit_in, const std::string& val_in, const std::string& unit_out, const std::streamsize& min_indent = 0ull) : Convert(std::move(std::make_tuple(unit_in, val_in, unit_out)), min_indent) {}

	NumberT operator()() const { return getResult(); }

	/**
	 * @brief	Format and print the result of the conversion to the given ostream instance.
	 *\n		This may change the floatfield and/or precision of the output stream.
	 * @returns	std::ostream&
	 */
	friend std::ostream& operator<<(std::ostream& os, const Convert& conv)
	{
		// get inputs
		const auto& [unit_in, input, unit_out] {conv._vars};
		
		// temporary buffer to prevent modifying STDOUT stream state
		std::stringstream ss;
		ss << configure_ostream;

		if (!Global.quiet) {
			ss // insert input
				<< Global.palette.set(OUT::INPUT_VALUE) << input << Global.palette.reset()
				<< ' '
				<< Global.palette.set(OUT::INPUT_UNIT) << unit_in << Global.palette.reset()
				<< str::VIndent(conv._min_indent, (input.size() + unit_in.size() + 1ull))
				<< Global.palette.set(OUT::EQUALS) << '=' << Global.palette.reset() << ' ';
		}

		const NumberT output{ conv.getResult() };

		ss // insert output
			<< Global.palette.set(OUT::OUTPUT_VALUE) << output << Global.palette.reset()
			<< ' '
			<< Global.palette.set(OUT::OUTPUT_UNIT) << unit_out << Global.palette.reset()
			;

		return os << ss.rdbuf();
	}
};

int main(const int argc, char** argv)
{
	int rc{ -1 };
	try {
		// parse arguments
		opt::ParamsAPI2 args{ argc, argv, 'p', "precision" };
		// find the program's location
		const auto [program_path, program_name] { env::PATH().resolve_split(argv[0]) };

		// handle help argument
		if (args.empty() || args.check_any<opt::Flag, opt::Option>('h', "help")) {
			write_help(std::cout, program_name.generic_string());
			return 0;
		}
		// handle version argument
		else if (args.check_any<opt::Flag, opt::Option>('v', "version")) {
			std::cout << program_name.generic_string() << " v" << ckconv_VERSION << std::endl;
			return 0;
		}

		// Set the ini path
		Global.ini_path = program_path / std::filesystem::path{ program_name }.replace_extension("ini");

		// handle other arguments (this must be called before reading the INI so --reset-ini works properly)
		handle_args(args);

		// read the ini if it exists
		if (file::exists(Global.ini_path))
			handle_config(file::INI(Global.ini_path));

		// get all parameters
		const auto params{ args.typegetv_all<opt::Parameter>() };

		// lambda to retrieve the current argument & the next 2 arguments as a string tuple. Does NOT check for out-of-range iterators!
		const auto& getArgTuple{ [&params](decltype(params)::const_iterator& it) -> Convert::Tuple {
			const auto first{ *it }; // these have to be defined in long form or operator++ causes them to be in the wrong order.
			const auto second{ *++it };
			const auto third{ *++it };
			return{ first, second, third };
		} };

		std::streamsize minIndent{ 8ll };
		for (auto it{ params.begin() }; it < params.end(); ++it)
			if (std::distance(it, params.end()) > 2ull) // if there are at least 2 more arguments after this one
				std::cout << "  " << Convert(getArgTuple(it), minIndent) << '\n';

		rc = 0;
	} catch (const std::exception& ex) {
		std::cerr << term::error << ex.what() << std::endl;
	} catch (...) {
		std::cerr << term::crit << "An undefined exception occurred!" << std::endl;
	}
	return rc;
}