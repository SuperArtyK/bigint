
/*! @file ACEngine/main_ACEngine.cpp
 *  This file contains the testing code for the engine or..larger project code, like a game.
 *  
 *  @todo Add the global variable list from my older engine
 *  @todo Add AELogParser to parse the engine's (AELogger) log files
 *  @todo Add engine's file format, AEFile
 *  @todo Transfer and adapt code from my previous engine attempt
 *  @todo Add project documentation (explanation, and examples for example) to the readme.md files
 *  
 *  Should not cause everything to break :)
 */

#include "AEBigint.hpp"


int main() {
	
	AEBigint a = ULLINT_MAX;

	std::cout << a << NLC << a.getSize() << NLC << ace::utils::boolToString(a.isNegative()) << NLC << NLC;

	a= -a;

	std::cout << a << NLC << a.getSize() << NLC << ace::utils::boolToString(a.isNegative()) << NLC << NLC;


	for (int i = 0; i < arrsize(powerOf10Table); i++) {
		std::cout << powerOf10Table[i] << NLC;
	}

	return 0;
}


