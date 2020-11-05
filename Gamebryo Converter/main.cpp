/**
 * main.cpp
 * Contains main()
 * by radj307
 */
#include "ini.h"

const std::string __INI_NAME = "conv.ini";

#include "main.hpp"

int main(int argc, char* argv[], char* envp[])
{
	// switch argument count
	switch ( argc ) {
	case 1: // user did not include any arguments (argc=1 & argv[0]) is implicit
		// check if INI exists, if not, create a new one
		(void)init_factor(__INI_NAME);
		return 1;
	default:
		// initialize INI, throw away return value
		(void)init_factor(__INI_NAME);
		// return the inverted result of bool interpret()
		return !interpret(argc, argv);
	}
}