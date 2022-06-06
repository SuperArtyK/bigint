#ifndef TESTS_COMPARISON_HPP_INCLUDED
#define TESTS_COMPARISON_HPP_INCLUDED

#include "bigint.hpp"
#include "utils.hpp"
#include <sstream>
#include <cstdlib>

/*
	comparison features tested:
		less than (operator<)
		greater then (operator>)
		equality (operator==)
		
*/


bool testLessThan(){
	printf("=> Test less than comparison #1: positive\n");
	Bigint testint(420);
	bool result = false;
	
	result = !(testint < 840);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, 420 and 840. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = (testint < 0);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, 420 and 0. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	result = (testint < -420);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, 420 and -420. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	
	///////
	 printf("=> Test less than comparison #1: negative\n");
	testint = (-420);
	
	result = !(testint < 840);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, -420 and 840. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = !(testint < 0);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, -420 and 0. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = (testint < -420);
	if(result){
		printf("=>--Fail less than comparison;\nCheck Bigint::operator<(llint) and Bigint::operator<(bigint).\n");
		printf("Compared, -420 and -420. Expected '0', got '%d'\n", (int)result);
		return false;
	}
	
	printf("=>++Test passed.\n");
	return true;
}

bool testGreaterThan(){
	printf("=> Test greater than comparison #1: positive\n");
	Bigint testint(420);
	bool result = false;
	
	result = (testint > 840);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, 420 and 840. Expected '0', got '%d'\n", (int)result);
		return false;
	}
	result = !(testint > 0);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, 420 and 0. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = !(testint > -420);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, 420 and -420. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	
	///////
	 printf("=> Test greater than comparison #1: negative\n");
	testint = (-420);
	
	result = (testint > 0);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, -420 and 0. Expected '0', got '%d'\n", (int)result);
		return false;
	}
	result = (testint > -420);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, -420 and -420. Expected '0', got '%d'\n", (int)result);
		return false;
	}
	result = !(testint > -840);
	if(result){
		printf("=>--Fail greater than comparison;\nCheck Bigint::operator>(llint) and Bigint::operator>(bigint).\n");
		printf("Compared, -420 and -840. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	
	printf("=>++Test passed.\n");
	return true;
}

bool testEquality(){
	printf("=> Test less than comparison #1: positive\n");
	Bigint testint(420);
	bool result = false;
	
	result = !(testint == 420);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==(bigint).\n");
		printf("Compared, 420 and 420. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = (testint == 0);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==bigint).\n");
		printf("Compared, 420 and 0. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	result = (testint == -420);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==(bigint).\n");
		printf("Compared, 420 and -420. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	
	///////
	 printf("=> Test less than comparison #1: negative\n");
	testint = (-420);
	
	result = (testint == 420);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==(bigint).\n");
		printf("Compared, -420 and 420. Expected '1', got '%d'\n", (int)!result);
		return false;
	}
	result = (testint == 0);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==bigint).\n");
		printf("Compared, -420 and 0. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	result = !(testint == -420);
	if(result){
		printf("=>--Fail equality comparison;\nCheck Bigint::operator==(llint) and Bigint::operator==(bigint).\n");
		printf("Compared, -420 and -420. Expected '1', got '%d'\n", (int)result);
		return false;
	}
	
	printf("=>++Test passed.\n");
	return true;
}

#endif // !TESTS_COMPARISON_HPP_INCLUDED
