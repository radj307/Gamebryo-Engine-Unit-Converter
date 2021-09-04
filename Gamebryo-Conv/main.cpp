#include <INI.hpp>
#include <optlib.hpp>
#include <resolve-path.hpp>

#include "build-cfg.hpp"

#include "interpret.hpp"

int main(const int argc, char** argv, char** envp)
{
	const std::string DEFAULT_INI_NAME{ "Gamebryo-Converter.ini"};
	const opt::Param args(argc, argv, { { }, { { "file", true }, { "reset-ini", false } } });

	INI ini;
	const auto resetINI{ args.check_opt("resetini") };


	if ( resetINI ) { // reset INI
		if ( DEFAULT_INI.write(DEFAULT_INI_NAME) )
			std::cout << sys::log << "Reset \"" << DEFAULT_INI_NAME << '\"' << std::endl;
		else 
			std::cout << sys::error << "Failed to reset \"" << DEFAULT_INI_NAME << '\"' << std::endl;
	}

	if ( file::exists(DEFAULT_INI_NAME) )
		ini.read(DEFAULT_INI_NAME); // read INI file if it exists
	else if ( const auto [location, name] { opt::resolve_split_path(envp, argv[0]) }; file::exists(location + DEFAULT_INI_NAME) )
		ini.read(location + DEFAULT_INI_NAME); // read ini on PATH if it exists
	else // use default ini if none found.
		ini = std::move(DEFAULT_INI.buildINI());

	Config conf(ini);
	cfg = &conf;

	// COMMANDS
	if ( args.exists_any("file", "f") ) {
		File proc(args.getv("file"));
	}

	// interpret args & return result code
	return interpret(args);
}