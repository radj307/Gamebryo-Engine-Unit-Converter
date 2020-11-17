#include "sys.h"
#include "INI_Include.h"
#include "interpret.hpp"

int main(int argc, char* argv[])
{
	opt::list args(argc, argv);
	// interpret args & return result code
	return interpret(args);
}