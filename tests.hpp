#ifndef TESTS_HPP_INCLUDED
#define TESTS_HPP_INCLUDED

#include "bigint.hpp"
#include "utils.hpp"
#include <sstream>
#include <cstdlib>
#include "test_comparison.hpp"
#include "test_core.hpp"

//prototypes
bool testGreatThan();
bool testLessThan();
bool testDigit();
bool testToString();
bool testOstream();
bool testAssignment();

/// Err, tests whole bunch of stuff.
/// For individual tests, search for functions named testX().
void test(){
    
    printf("=>  Entered Tests...");
    
    bool __failed = !testDigit();
	__failed = __failed || !testToString();
	__failed = __failed || !testOstream();
	__failed = __failed || !testAssignment();
	__failed = __failed || !testLessThan();
	__failed = __failed || !testGreatThan();
	
	
	
	if(__failed){
	    printf("Tests failed, aborting program...\n");
	    exit(255);
	}
	
	
	printf("\nAll Tests passeed.\n---------------\n\n");
}


#endif // !TESTS_HPP_INCLUDED
