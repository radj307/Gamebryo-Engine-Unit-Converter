#include <INI.hpp>
#include <opt.hpp>

#include "build-cfg.hpp"

Config* cfg{ nullptr };

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	if (!file::exists("def.ini")) {
		if (write_default_ini())
			std::cout << sys::log << std::endl;
	}
	INI ini("def.ini");
	Config cfg(ini);
	opt::list args(argc, argv, "");
	// interpret args & return result code
	return interpret(args);
}