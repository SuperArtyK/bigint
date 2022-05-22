#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <type_traits>
#include <cstdio>
#include <iostream>
#include <cmath>


//Conversions between char and int, as single number digits.

///\brief Convert ascii char to 1 int digit.
#define CHAR_TO_INT(x) (int(x) - 48)
///Convert 1-digited int to char value
#define INT_TO_CHAR(x) (char(x+48))

///Flag to enable deblog(x) logging. Set to 0 to disable
#define DEBUG_LOGGING 1
///Debug printf(), use the same way as you have used printf before.
///Prints additional newline with each print
#define dprintf(...) if(DEBUG_LOGGING){printf("DEBUG::%s()-> ", __FUNCTION__); printf(__VA_ARGS__); printf("\n");}
///Debug std::cout.
///For arguments, use same as normal cout.
///But put them into parentheses.
///Prints additional newline with each print.
///Example: dcout("123"<<456);  --  prints '123456', and a newline
#define dcout(...) if(DEBUG_LOGGING){std::cout<<"DEBUG::"<<__FUNCTION__ <<"()-> "<<__VA_ARGS__<<'\n';}

///Get size of the array (on stack)
#define arrsz(x) (sizeof(x)/sizeof(x[0]))



typedef unsigned long long int ullint;
typedef long long int llint;
typedef unsigned char ucint;
typedef char cint;



///Get number length of integer types
template<typename T>
inline int numlength_i(T num){
	static_assert(std::is_integral<T>::value, "Cannot use non-integral types in the 'numlength' designed for integral types( numlength_i() ). For float, use numlength_f() instead.");
    int dig = 1;
    while(num/=10)
        dig++;
    return dig;
}

///Get number length of the floating point types
template<typename T>
inline int numlength_f(T num){
	static_assert(std::is_floating_point<T>::value, "Cannot use non-float types in the 'numlength' designed for float types( numlength_f() ). For integral, use numlength_i() instead.");
    return ((num == 0)?0:(int)log10(abs(num)))+1;
}

#endif // !UTILS_HPP_INCLUDED
