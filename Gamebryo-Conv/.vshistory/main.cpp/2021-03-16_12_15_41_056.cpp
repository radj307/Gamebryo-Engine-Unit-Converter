//#include "sys.h"
#include <INI.hpp>
#include <opt.hpp>

#include "build-cfg.hpp"

/**
 * write_default_ini(const string&)
 * @brief Write the default INI config values to disk.
 * @param filename	- (Default: "def.ini")
 * @return true		- Successfully wrote default INI values to disk.
 * @return false	- Failed to write default INI values to disk.
 */
inline bool write_default_ini(const std::string& filename = "def.ini", bool overwrite = true)
{
	const section_map
		// Default Setting Values
		defMap{
			{
				"conversions", {
					{ "imperial_to_metric",	"0.3048" },			// Factor_MI
					{ "metric_to_unit",		"0.0142875313" },	// Factor_UM
					{ "imperial_to_unit",	"21.3333333333" },	// Factor_UI
				}
			},
			{
				"config", {
					{ "precision", "6" },
					{ "disable-colors", "false" },
					{ "always-show-input", "false" },
				}
			},
	},
		// Default Setting Comments
		defCommentMap{
			{
				"conversions", {
					{ "imperial_to_metric",	"Used for conversions between metric & imperial. ([Metric] = [Imperial] * [Factor])" },
					{ "metric_to_unit",		"Used for conversions between metric & units. ([Units] = [Metric] * [Factor])" },
					{ "imperial_to_unit",	"Used for conversions between imperial & units. ([Units] = [Imperial] * [Factor])" },
				}
			},
			{
				"config", {
					{ "disable-colors", "false" },
					{ "always-show-input", "false" },
				}
			},
	};
	// forward the return value from the INI_Defaults::write() function.
	return INI_Defaults{ defMap, defCommentMap }.write(filename, !overwrite);
}

write_default_ini();

INI ini("def.ini");
Config cfg(ini);

#include "interpret.hpp"

int main(const int argc, char* argv[])
{
	opt::list args(argc, argv, "");
	// interpret args & return result code
	return interpret(args);
}