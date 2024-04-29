
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
	cout << "The value of the bigint: " << bint.toString2() << NLC;
	cout << "--------------------------------------------" << NLC;

}

long double runningAvg(const int amt, const long double val, const long double newval) noexcept{
	return (val * amt + newval) / (amt + 1);
}



int main() {

	std::srand(1);

	AEBigint a = UINT_MAX;
	outputBint(a);
	outputBint(0);
	outputBint(LLINT_MIN);
	outputBint(ULLINT_MAX);

	
	
	for (int i = a.getSize()-1; i >=0; i--) {
		cout << a.getDigit(i);
	}

	a.setDigit(18, 9);
	cout << NLC << a.toString2()<<NLC;
	a.clear();
	a.setDigit(8000000000, 9);
// 	for (ullint i = 0; i < a.getSectorAmount(); i++) {
// 		a.setSectorValue(i, std::rand() * powerOf10Table[9] + std::rand());
// 	}


	cout << "Press enter to start benchmark" << NLC;

	cin.get();
	std::string b;
	timePoint<HighResClock> tp1;
	ullint repeat = 0;
	long double tm1, tm2, avg = 0;


	
	constexpr int iter = 1;

	for (;;) {


		
		cout << "Starting timing string processing (optimised)..." << NLC;
		tp1 = getHighResTime();
		for (int i = 0; i < iter; i++) {
			b = a.toString();
		}
		cout << "time used processing (optimised): " << (tm2 = timeBetween(tp1, getHighResTime(), long double) * 1000.0L) << NLC;

		b.clear();
		b.shrink_to_fit();


		cout << "Starting timing string processing (unoptimised)..." << NLC;

		tp1 = getHighResTime();
		//cout<<a<<NLC;
		//outputBint(a);
		for (int i = 0; i < iter; i++) {
			b = a.toString2();
		}
		cout << "time used processing (unoptimised): " << (tm1 = timeBetween(tp1, getHighResTime(), long double) * 1000.0L) << NLC;

		b.clear();
		b.shrink_to_fit();



		cout << "Improvement: " << ((tm1 - tm2) / tm2 * 100) << "%; average: " << (avg = runningAvg(repeat++, avg, ((tm1 - tm2) / tm2 * 100)))<< NLC << NLC;

	}

	cin.get();
	return 0;
}


