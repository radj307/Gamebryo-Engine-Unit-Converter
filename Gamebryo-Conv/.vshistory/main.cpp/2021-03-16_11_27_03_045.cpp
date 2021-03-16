//#include "sys.h"
#include <INI.hpp>
#include <opt.hpp>

#include "build-cfg.hpp"

Config cfg;

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	opt::list args(argc, argv, "");
	// interpret args & return result code
	return interpret(args);
}