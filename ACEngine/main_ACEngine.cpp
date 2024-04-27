
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
using namespace std;


void outputBint(const AEBigint& bint) {
	cout << "--------------------------------------------" << NLC;
	cout << "The size of the bigint (digits): " << bint.getSize()<<NLC;
	cout << "The amount of number sectors: " << bint.getSectorAmount() << NLC;
	cout << "The memory usage of the bigint (bytes): " << bint.getMemoryUsage() << " (sizeof(AEBigint) = " <<sizeof(AEBigint)<<")" << NLC;
	cout << "The bigint is negative: " << ace::utils::boolToString(bint.isNegative()) << NLC;
	cout << "The value of the bigint: " << bint << NLC;
	cout << "--------------------------------------------" << NLC;

}





int main() {

	AEBigint a = UINT_MAX;
	outputBint(a);
	outputBint(0);
	outputBint(LLINT_MIN);
	outputBint(ULLINT_MAX);

	
	
	for (int i = a.getSize()-1; i >=0; i--) {
		cout << a.getDigit(i);
	}
	cout << NLC << a<<NLC;
	a.setDigit(256, 9);
	//cout<<a<<NLC;
	outputBint(a);

	return 0;
}


