#include <iostream>
#include "bigint.hpp"
#include "tests.hpp"
using namespace std;

int main() {
    dprintf("---------------");
	dprintf("Entered main...");
	dprintf("---------------\n");
	
	test();
	
	Bigint a(123), b(123);
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
	cout<<(a==b);
	//dprintf("\n12345");
}
