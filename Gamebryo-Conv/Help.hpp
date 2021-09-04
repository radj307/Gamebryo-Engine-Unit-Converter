#pragma once
#include <ostream>
#include <iomanip>

namespace ck_unit_conv {
	struct Help {
		std::string name;
		std::streamsize maxIndent;
		explicit Help(std::string program_name, std::streamsize maxIndent) : name{ std::move(program_name) }, maxIndent{ std::move(maxIndent) } {}
		friend std::ostream& operator<<(std::ostream& os, const Help& obj)
		{
			os << "USAGE:\n\t" << obj.name << " [--reset-ini|--ini-reset] [-h|--help] [<unit> <value> <unit|all>]\nOPTIONS\n\t";
			std::string length{ "-h | --help" };
			os << length << std::setw(obj.maxIndent - length.size()) << ' ' << "Prints this help display.\n\t";
			length = "--reset-ini | --ini-reset";
			os << length << std::setw(obj.maxIndent - length.size()) << ' ' << "Reset the INI config file, or generate a new one if it is missing.\n";
			os.flush();
			return os;
		}
	};

}