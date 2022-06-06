#include <iostream>
#include "bigint.hpp"
#include "tests.hpp"
using namespace std;


//TODO:Add tests for the comparisons

/*
	List of things to add:
			
		* operator+ for:
			Bigint
			int
			c-string
			std::string
			floats
			
		* unary operator++
		
		* operator- for:
			Bigint
			int
			c-string
			std::string
			floats
		
		* unary operator--
		
		* operator/ for:
			Bigint
			int
			c-string
			std::string
			floats
		
		* operator* for:
			Bigint
			int
			c-string
			std::string
			floats
		
		* operator- for:
			Bigint
			int
			c-string
			std::string
			floats
		
*/


int main() {
	dprintf("---------------");
	dprintf("Entered main...");
	dprintf("---------------\n");
	
	test();
	
	cout<<BigintConstants::llintmax<<endl;
	cout<<BigintConstants::llintmin<<endl;
	cout<<BigintConstants::intmax<<endl;
	cout<<BigintConstants::intmin<<endl;
	cout<<BigintConstants::uintmax<<endl;
	cout<<BigintConstants::uintmin<<endl;
	cout<<BigintConstants::shortmax<<endl;
	cout<<BigintConstants::shortmin<<endl;
	cout<<BigintConstants::ushortmax<<endl;
	cout<<BigintConstants::ushortmin<<endl;
	cout<<BigintConstants::charmax<<endl;
	cout<<BigintConstants::charmin<<endl;
	cout<<BigintConstants::ucharmax<<endl;
	cout<<BigintConstants::ucharmin<<endl;
	cout<<BigintConstants::floatmax<<endl<<endl;
	cout<<BigintConstants::floatmin<<endl<<endl;
	cout<<BigintConstants::doublemax<<endl<<endl;
	cout<<BigintConstants::doublemin<<endl<<endl;
	cout<<BigintConstants::ldoublemax<<endl<<endl;
	cout<<BigintConstants::ldoublemin<<endl;
	
	
	
	return 0;
}
