#include <TermAPI.hpp>

int main(const int argc, char** argv)
{
	std::cout << term::EnableANSI;
	try {


		return 0;
	} catch (const std::exception& ex) {
		std::cerr << term::error << ex.what() << std::endl;
		return -1;
	}catch (...){
		std::cerr << term::crit << "An unknown exception occurred!" << std::endl;
		return -1;
	}
}