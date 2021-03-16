#include <INI.hpp>
#include <opt.hpp>

#include "build-cfg.hpp"

Config* cfg{ nullptr };

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	if (!file::exists("def.ini")) {
		if (write_default_ini())
			std::cout << sys::log << "Wrote default configuration to 'def.ini'" << std::endl;
		else std::cout << sys::error << "Failed to write default INI, conversions may not work correctly!" << std::endl;
	}
	INI ini("def.ini");
	Config conf(ini);
	cfg = &conf;
	opt::list args(argc, argv, "file,f,resetini");
	// interpret args & return result code
	return interpret(args);
}