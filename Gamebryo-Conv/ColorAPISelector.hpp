#pragma once
#include <ColorLib.hpp>
#include <TermAPI.hpp>
#include <sysapi.h>
namespace ck_unit_conv {
	inline void setANSISupport(const bool enabled = true)
	{
		if ( enabled )
			std::cout << sys::term::EnableANSI;
		else
			std::cout << sys::term::DisableANSI;
	}

	static bool useANSI{ false };

	struct Colorize {
		short _color;
		Colorize(const short color) : _color{ color } {}
		friend std::ostream& operator<<(std::ostream& os, const Colorize& obj)
		{
			if (useANSI)
				os << color::setcolor(obj._color, true);
			else
				os << Color::Colorizer(obj._color);
			return os;
		}
	};
}