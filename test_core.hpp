#ifndef TESTS_CORE_HPP_INCLUDED
#define TESTS_CORE_HPP_INCLUDED

#include "bigint.hpp"
#include "utils.hpp"
#include <sstream>
#include <cstdlib>

/*
	core features tested:
		setDigit
		toString conversion
		ostream output
		assignment(and copy-constructuon)
*/



/// Briefly tests Bigint::set/getDigit.
bool testDigit(){
	printf("=> Test setting digits #1: positive\n");
	Bigint testint;
	int number[3] = {0,2,4};
	for(int i = 0; i < arrsz(number); i++){
		testint.setDigit(number[i], i);
	}
	
	for(int i = 0; i < arrsz(number); i++){
		if(testint.getDigit(i) != number[i]){
			printf("=>--Fail setting digit(%d);\n"
				"Check Bigint::setDigit(ucint, ullint) and Bigint().\n"
				"If no bugs found, look at Bigint::getDigit(ullint), maybe that's the issue.\n",
				i);
			printf("Expected '%d', got '%d'!\n", number[i], testint.getDigit(i));
			return false;
		}
	}
	printf("=>++Test passed.\n");
	return true;
}

/// Tests Bigint::toString().
bool testToString(){
	printf("=> Test toString conversion #1: positive\n");
	Bigint testint;
	const char number[3] = {'4','2','0'};
	for(int i = 0; i < arrsz(number); i++){
		testint.setDigit(CHAR_TO_INT(number[i]), arrsz(number)-1-i);
	}
	std::string tempstr = testint.toString();
	
	for(int i = 0; i < arrsz(number); i++){
		if(tempstr[i] != number[i]){
			printf("Fail converting numbers to string(%d);\n"
				"Check Bigint::setDigit(ucint, ullint) and Bigint().\n"
				"If no bugs found, look at Bigint::getDigit(ullint), maybe that's the issue.\n",
				i);
			printf("Expected '%s', got '%s'!\n", number, tempstr.c_str());
			return false;
		}
	}
	printf("=>++Test passed.\n");
	return true;
}

/// Tests operator<<() implementation for Bigint.
bool testOstream(){
	printf("=> Test ostream printing #1: positive\n");
	Bigint testint;
	const char number[3] = {'4','2','0'};
	for(int i = 0; i < arrsz(number); i++){
		testint.setDigit(CHAR_TO_INT(number[i]), arrsz(number)-1-i);
	}
	
	std::stringstream ss;
	ss << testint;
	std::string tempstr = ss.str();
	for(int i = 0; i < arrsz(number); i++){
		if(tempstr[i] != number[i]){
			printf("Fail outputting numbers through ostream(%d);\n"
				"Check operator<<(ostream&, const Bigint&), Bigint::setDigit(ucint, ullint)().\n"
				"If no bugs found, look at Bigint::getDigit(ullint), maybe that's the issue.\n",
				i);
			printf("Expected '%s', got '%s'!\n", number, tempstr.c_str());
			return false;
		}
	}
	
	printf("=>++Test passed.\n");
	return true;
}


/// Tests copy constructors and assignment operators.
bool testAssignment(){
	
	printf("=> Test assignment/copy #1: positive\n");
	Bigint testint(420), testint2("420"), testint3(testint);
#ifdef BIGINT_ENABLE_FLOATS
	Bigint testint4(420.123f);
#endif // !BIGINT_ENABLE_FLOATS
	std::string tempstr = testint.toString();
	
	//something, something, tempstr = testint.toString();
	if(tempstr != "420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromInt(llint).\n");
		printf("Expected '420', got '%s'\n", tempstr.c_str());
		return false;
	}
	tempstr = testint2.toString();
	if(tempstr != "420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromCString(const char*).\n");
		printf("Expected '420', got '%s'\n", tempstr.c_str());
		return false;
	}
	tempstr = testint3.toString();
	if(tempstr != "420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromBigint(const Bigint&).\n");
		printf("Expected '420', got '%s'\n", tempstr.c_str());
		return false;
	}
#ifdef BIGINT_ENABLE_FLOATS
	tempstr = testint4.toString();
	if(tempstr != "420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromBigint(const long double).\n");
		printf("Expected '420', got '%s'\n", tempstr.c_str());
		return false;
	}
#endif // !BIGINT_ENABLE_FLOATS
	
	///////
	printf("=> Test assignment/copy #2: negative\n");
	testint = -420;
	testint2 = "-420";
	testint3 = testint;
#ifdef BIGINT_ENABLE_FLOATS
	testint4 = -420.123f;
#endif // !BIGINT_ENABLE_FLOATS

	tempstr = testint.toString();
	
	if(tempstr != "-420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromInt(llint) and Bigint::setDigit(ucint, ullint).\n");
		printf("Expected '-420', got '%s'\n", tempstr.c_str());
		return false;
	}
	tempstr = testint2.toString();
	if(tempstr != "-420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromCString(const char*).\n");
		printf("Expected '-420', got '%s'\n", tempstr.c_str());
		return false;
	}
	tempstr = testint3.toString();
	if(tempstr != "-420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromBigint(const Bigint&)/\n");
		printf("Expected '-420', got '%s'\n", tempstr.c_str());
		return false;
	}
#ifdef BIGINT_ENABLE_FLOATS
	tempstr = testint4.toString();
	if(tempstr != "-420"){
		printf("=>--Fail assigning value to bigint;\nCheck Bigint::copyFromBigint(const long double).\n");
		printf("Expected '-420', got '%s'\n", tempstr.c_str());
		return false;
	}
#endif
	
	printf("=>++Test passed.\n");
	return true;
}

#endif // !TESTS_CORE_HPP_INCLUDED
