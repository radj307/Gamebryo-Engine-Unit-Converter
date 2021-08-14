#include <INI.hpp>
#include <optlib.hpp>

#include "build-cfg.hpp"

Config* cfg{ nullptr }; ///< @brief This is required until I go through the whole program and change the use of global vars.

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	opt::Param args(argc, argv, { { }, { { "file", true }, { "reset-ini", false } } });
	if (!file::exists("def.ini") || args.check_opt("resetini")) {
		if (write_default_ini())
			std::cout << sys::log << "Wrote default configuration to 'def.ini'" << std::endl;
		else std::cout << sys::error << "Failed to write default INI, conversions may not work correctly!" << std::endl;
	}
	INI ini("def.ini");
	Config conf(ini);
	cfg = &conf;
	// interpret args & return result code
	return interpret(args);
}