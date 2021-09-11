#include <INI.hpp>
#include <resolve-path.hpp>

#include "build-cfg.hpp"
#include "interpret.hpp"
#include "Help.hpp"

int main(const int argc, char** argv, char** envp)
{
	try {
		using namespace ck_unit_conv;

		const std::string DEFAULT_INI_NAME{ "Gamebryo-Converter.ini" };
		const opt::Params args(argc, argv, { "file" });
		// { { 'h' }, { { "help", false }, {"file", true}, {"reset-ini", false}, {"ini-reset", false}, {"ANSI", false} }});

		const auto [location, name] { opt::resolve_split_path(envp, argv[0]) };

		if (args.empty() || args.check_flag('h') || args.check_opt("help"))
			std::cout << Help(name, 32) << '\n';

		INI ini;
		const auto resetINI{ args.check_opt("reset-ini") || args.check_opt("ini-reset") };

		if (resetINI) { // reset INI
			if (DEFAULT_INI.write(DEFAULT_INI_NAME))
				std::cout << sys::log << "Reset \"" << DEFAULT_INI_NAME << '\"' << std::endl;
			else
				std::cout << sys::error << "Failed to reset \"" << DEFAULT_INI_NAME << '\"' << std::endl;
		}

		if (file::exists(DEFAULT_INI_NAME))
			ini.read(DEFAULT_INI_NAME); // read INI file if it exists
		else if (file::exists(location + DEFAULT_INI_NAME))
			ini.read(location + DEFAULT_INI_NAME); // read ini on PATH if it exists
		else // use default ini if none found.
			ini = std::move(DEFAULT_INI.buildINI());

		if (ini.get("config", "use-ansi-colors") || args.check_opt("ANSI")) {
			setANSISupport(true);
			useANSI = true;
		}

		Config conf(ini);
		cfg = &conf;

		// COMMANDS
		if (args.contains("file", "f"))
			File proc(args.getv("file").value_or(""));

		// interpret args & return result code
		#ifdef _DEBUG
		int retval{ interpret(args) };
		return retval;
		#else
		return interpret(args);
		#endif
	} catch (std::exception& ex) {
		std::cout << sys::error << ex.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << sys::error << "An unknown exception occurred!" << std::endl;
		return -2;
	}
}