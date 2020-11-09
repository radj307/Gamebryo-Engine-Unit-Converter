/**
 * main.cpp
 * Contains main()
 * by radj307
 */
#include "INI/INI.h"
INI cfg("conv.ini"); // init INI
#include "main.hpp"

int main(const int argc, const char* argv[])
{
	// switch argument count
	switch ( argc ) {
	case 1:return 1;
	default:
		// return the inverted result of bool interpret()
		return !interpret(argc, argv);
	}
}