#include <INI.hpp>
#include <opt.hpp>

#include "build-cfg.hpp"

Config* cfg{ nullptr };

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	INI ini("def.ini");
	Config cfg(ini);
	opt::list args(argc, argv, "");
	// interpret args & return result code
	return interpret(args);
}