/**
 * main.cpp
 * Contains main()
 * by radj307
 */
#include "ini.h"
#include "main.hpp"

int main(int argc, char* argv[], char* envp[])
{
	init_factor("conv.ini");

	// switch argument count
	switch ( argc ) {
	case 1: // user did not include any arguments, return 1 for error. (argc=1 & argv[0]) is implicit
		return 1;
	default: // return the inverted result of bool interpret()
		return !interpret(argc, argv);
	}
}