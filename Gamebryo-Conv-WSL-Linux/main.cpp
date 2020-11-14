#include "sys.h"
#include "INI_Include.h"
#include "interpret.hpp"

int main(int argc, const char* argv[])
{
	// vector to hold arguments
	std::vector<std::string> arguments;
	// copy arguments to vector, skip argv[1] as it is the program location
	for ( int i = 1; i < argc; i++ ) { arguments.push_back(argv[i]); }
	// interpret args & return result code
	return interpret(arguments);
}