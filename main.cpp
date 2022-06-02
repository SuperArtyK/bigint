#include <iostream>
#include "bigint.hpp"
#include "tests.hpp"
using namespace std;


//TODO:Add tests for the comparisons

/*
    List of things to add:
            
        * assigning from:
            float
            double
            long double
        
        * constants:
            double min/max
            long double min/max
            float min/max
            
        * operator+ for:
            Bigint
            int
            c-string
            std::string
            
        * unary operator++
        
        * operator- for:
            Bigint
            int
            c-string
            std::string
        
        * unary operator--
        
        
*/
int main() {
    dprintf("---------------");
	dprintf("Entered main...");
	dprintf("---------------\n");
	
	test();
	
	Bigint a(-420), b(420);
	//cout<<BigintConstants::llintmax<<'\n';
	//cout<<BigintConstants::llintmin<<'\n';
	//cout<<BigintConstants::intmax<<'\n';
	//cout<<BigintConstants::intmin<<'\n';
	//cout<<BigintConstants::uintmax<<'\n';
	//cout<<BigintConstants::uintmin<<'\n';
	//
	//cout<<BigintConstants::shortmax<<'\n';
	//cout<<BigintConstants::shortmin<<'\n';
	//cout<<BigintConstants::ushortmax<<'\n';
	//cout<<BigintConstants::ushortmin<<'\n';
	//cout<<BigintConstants::charmax<<'\n';
	//cout<<BigintConstants::charmin<<'\n';
	//cout<<BigintConstants::ucharmax<<'\n';
	//cout<<BigintConstants::ucharmin<<'\n';
	cout<<(a>420);
	//dprintf("\n12345");
}
