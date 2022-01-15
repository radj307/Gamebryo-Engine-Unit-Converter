#include "conv.hpp"
#include "Global.h"

#include <envpath.hpp>

/**
 * @struct	Convert
 * @brief	Performs a single conversion operation, and exposes std::ostream operator<<() to format and insert it into an output stream.
 */
struct Convert {
	/// @brief	String Tuple
	using Tuple = std::tuple<std::string, std::string, std::string>;
private:
	Tuple _vars;

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

	///	@brief	Returns the result of the conversion
	long double getResult() const noexcept(false) { return ckconv::convert(ckconv::getUnit(std::get<0>(_vars)), str::stold(std::get<1>(_vars)), ckconv::getUnit(std::get<2>(_vars))); }

public:
	/// @brief	Default constructor
	Convert(std::tuple<std::string, std::string, std::string>&& vars) : _vars{ sort_args(std::move(vars)) } {}
	/**
	 * @brief			Constructor
	 * @param unit_in	Input Unit (OR Input Value, if val_in is the input unit)
	 * @param val_in	Input Value (OR Input Unit, if unit_in is the input value)
	 * @param unit_out	Output Unit
	*/
	Convert(const std::string& unit_in, const std::string& val_in, const std::string& unit_out) : _vars{ sort_args(std::make_tuple(unit_in, val_in, unit_out)) } {}

	long double operator()() const { return getResult(); }

	/**
	 * @brief	Format and print the result of the conversion to the given ostream instance.
	 *\n		This may change the floatfield and/or precision of the output stream.
	 * @returns	std::ostream&
	 */
	friend std::ostream& operator<<(std::ostream& os, const Convert& conv)
	{
		const auto& [unit_in, input, unit_out] {conv._vars};
		const auto output{ conv.getResult() };

		// override floatfield notation
		if (Global.notation != nullptr)
			os.setf(*Global.notation);

		// modify precision
		if (Global.precision != 6ll)
			os.precision(Global.precision);

		// quiet
		if (!Global.quiet) {
			os  // show input value & unit
				<< Global.palette.set(OUT::INPUT_VALUE) << input << Global.palette.reset()
				<< ' '
				<< Global.palette.set(OUT::INPUT_UNIT) << unit_in << Global.palette.reset()
				<< Global.palette.set(OUT::EQUALS) << " = " << Global.palette.reset();
		}


		return os // output:
			<< Global.palette.set(OUT::OUTPUT_VALUE) << output << Global.palette.reset()
			<< ' '
			<< Global.palette.set(OUT::OUTPUT_UNIT) << unit_out << Global.palette.reset()
			;
	}
};

int main(const int argc, char** argv)
{
	try {
		const auto clog_rdbuf{ std::clog.rdbuf(Global.log.rdbuf()) };

		// parse arguments
		opt::ParamsAPI2 args{ argc, argv, 'p', "precision" };
		// find the program's location
		const auto [program_path, program_name] { env::PATH().resolve_split(argv[0]) };

		// handle help argument
		if (args.check_any<opt::Flag, opt::Option>('h', "help")) {
			write_help(std::cout, program_name.generic_string());
			return 0;
		}
		// handle version argument
		else if (args.check_any<opt::Flag, opt::Option>('v', "version")) {
			std::cout << program_name.generic_string() << " v" << ckconv_VERSION << std::endl;
			return 0;
		}

		// handle other arguments
		handle_args(args);

		// Set the ini path
		Global.ini_path = [&program_path, &program_name]() {
			std::filesystem::path tmp{ program_name };
			tmp.replace_extension("ini");
			return program_path / tmp;
		}();

		// read the ini if it exists
		if (file::exists(Global.ini_path))
			handle_config(file::INI(Global.ini_path));

		const auto params{ args.typegetv_all<opt::Parameter>() };

		const auto& getArgTuple{ [&params](decltype(params)::const_iterator& it) -> Convert::Tuple {
			const auto first{ *it };
			const auto second{ *++it };
			const auto third{ *++it };
			return{ first, second, third };
		} };

		for (auto it{ params.begin() }; it < params.end(); ++it) {
			if (std::distance(it, params.end()) > 2ull) {
				std::cout << "  " << Convert(getArgTuple(it)) << '\n';
			}
		}

		return 0;
	} catch (const std::exception& ex) {
		std::cerr << term::error << ex.what() << std::endl;
		return -1;
	} catch (...) {
		std::cerr << term::crit << "An undefined exception occurred!" << std::endl;
		return -1;
	}


	std::cout << Convert("u", "1", "m") << '\n';
	std::cout << Convert("u", "99", "m") << '\n';
	std::cout << Convert("m", "1", "u") << '\n';
	std::cout << Convert("m", "1", "ft") << '\n';
	std::cout << Convert("mm", "500", "u") << '\n';
	std::cout << Convert("mm", "500", "in") << '\n';

	return 0;
}