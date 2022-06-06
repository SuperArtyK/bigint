#ifndef BIGINT_FLAGS_HPP_INCLUDED
#define BIGINT_FLAGS_HPP_INCLUDED

/*
	This file contains list of compile flags
	in the preprocessor 'define' form.
	
	Uncomment the following 'undef' line to disable
	the flag.
*/


/// Default vector preallocation size
#define BIGINT_PREALLOC_SIZE 48
/// Flat to enable floating point support at all
#define BIGINT_ENABLE_FLOATS
/// Flag to enable floating-point constants in BigintConstants Struct
#define BIGINT_ENABLE_FLOAT_CONSTANTS
/// Flag to enable BigintConstants struct
#define BIGINT_ENABLE_CONSTANTS



//Uncomment the undef to disable the flag.
//And vice-versa

//#undef BIGINT_ENABLE_FLOATS
//#undef BIGINT_ENABLE_FLOAT_CONSTANTS
//#undef BIGINT_ENABLE_CONSTANTS



#endif // !BIGINT_FLAGS_HPP_INCLUDED