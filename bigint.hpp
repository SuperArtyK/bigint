#ifndef BIGINT_HPP_INCLUDED
#define BIGINT_HPP_INCLUDED


#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include <climits>
#include <cfloat>
#include "utils.hpp"
#include "bigint_flags.hpp"

/// \brief Implementation of the big integers, expansion of the built-in integer types of c++.
/// For indexing separate digits, array index style is used.
/// Also, it is little-endian in a sense. 
/// Least significant digits, ones, are stored first, at index 0
class Bigint{

public:

/////////////////
// constructors
/////////////////
	/// Constructs Bigint with value 0;
	Bigint();
	/// Constructs Bigint with value from another Bigint.
	Bigint(const Bigint& bint);
	/// Constructs Bigint with value from std::string.
	Bigint(const std::string& strnum);
	/// Constructs Bigint with value from c-string.
	Bigint(const char* strnum);
	/// Constructs Bigint with value from long long int(llint).
	Bigint(const llint num);
	/// Constructs Bigint with value from int.
	/// Needed, because compiler would make 'ambiguous' errors confusing that with strings and chars.
	/// We need a Number, not a string with char 0.
	Bigint(const int num);
#ifdef BIGINT_ENABLE_FLOATS
	/// Constructs Bigint with value from normal float.
	Bigint(const float num);
	/// Constructs Bigint with value from double.
	Bigint(const double num);
	/// Constructs Bigint with value from long double.
	Bigint(const long double num);
#endif // !BIGINT_ENABLE_FLOATS
	
/////////////////
// operators
/////////////////
// assigments
	/// Copies value of the given Bigint to itself.
	Bigint& operator=(const Bigint& bint);
	/// Copies int value in the (std::)string to itself.
	Bigint& operator=(const std::string& strnum);
	/// Copies int value in the (c-)string to itself.
	Bigint& operator=(const char* strnum);
	/// Copies int value to itself.
	Bigint& operator=(const llint num);
	/// Copies int value to itself.
	inline Bigint& operator=(const int num) { return operator=(llint(num)); }
	/// Copies float value to itself.
	Bigint& operator=(const float num);
	/// Copies double value to itself.
	Bigint& operator=(const double num);
	/// Copies long double value to itself.
	Bigint& operator=(const long double num);
	
// comparisons
// equality
	/// Checks for equality of both Bigint and Bigint.
	bool operator==(const Bigint& bint) const;
	/// Checks for equality of Bigint and c-string representation of the number.
	bool operator==(const char* strnum) const;
	/// Checks for equality of Bigint and std::string representation of the number.
	bool operator==(const std::string& strnum) const;
	/// Checks for equality of Bigint and long long.
	bool operator==(const llint num) const;
	/// Checks for equality of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator==(const int num) const { return this->operator==((llint)num); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Checks for equality of both Bigint and float.
	bool operator==(const float num) const;
	/// Checks for equality of both Bigint and double.
	bool operator==(const double num) const;
	/// Checks for equality of both Bigint and long double.
	bool operator==(const long double num) const;
#endif // !BIGINT_ENABLE_FLOATS
	
// inequality
	/// Checks for inequality of both Bigint and Bigint.
	inline bool operator!=(const Bigint& bint) const { return !this->operator==(bint); }
	/// Checks for inequality of Bigint and c-string representation of the number.
	inline bool operator!=(const char* strnum) const { return !this->operator==(Bigint(strnum)); }
	/// Checks for inequality of Bigint and std::string representation of the number.
	inline bool operator!=(const std::string& strnum) const { return !this->operator==(Bigint(strnum)); }
	/// Checks for inequality of Bigint and long long.
	inline bool operator!=(const llint num) const { return !this->operator==(Bigint(num)); }
	/// Checks for inequality of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator!=(const int num) const { return !this->operator==(Bigint(num)); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Checks for inequality of both Bigint and float.
	inline bool operator!=(const float num) const { return !this->operator==(num); }
	/// Checks for inequality of both Bigint and double.
	inline bool operator!=(const double num) const { return !this->operator==(num); }
	/// Checks for inequality of both Bigint and long double.
	inline bool operator!=(const long double num) const { return !this->operator==(num); }
#endif // !BIGINT_ENABLE_FLOATS

// less than
	/// Less-than comparison of Bigint and Bigint.
	bool operator<(const Bigint& bint) const;
	/// Less-than comparison of Bigint and c-string representation of the number.
	bool operator<(const char* strnum) const;
	/// Less-than comparison of Bigint and std::string representation of the number.
	bool operator<(const std::string& strnum) const;
	/// Less-than comparison of Bigint and long long.
	bool operator<(const llint num) const;
	/// Less-than comparison of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator<(const int num) const { return this->operator<((llint)num); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Less-than comparison of Bigint and float.
	inline bool operator<(const float num) const { return this->operator<(Bigint(ceil(num))); }
	/// Less-than comparison of Bigint and double.
	inline bool operator<(const double num) const { return this->operator<(Bigint(ceil(num))); }
	/// Less-than comparison of Bigint and long double.
	inline bool operator<(const long double num) const { return this->operator<(Bigint(ceil(num))); }
#endif // !BIGINT_ENABLE_FLOATS
	
// greater-than
	/// Greater-than comparison of Bigint and Bigint.
	bool operator>(const Bigint& bint) const;
	/// Greater-than comparison of Bigint and c-string representation of the number.
	bool operator>(const char* strnum) const;
	/// Greater-than comparison of Bigint and std::string representation of the number.
	bool operator>(const std::string& strnum) const;
	/// Greater-than comparison of Bigint and long long.
	bool operator>(const llint num) const;
	/// Greater-than comparison of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator>(const int num) const { return this->operator>(Bigint(num)); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Less-than comparison of Bigint and float.
	inline bool operator>(const float num) const { return this->operator>(Bigint(floor(num))); }
	/// Less-than comparison of Bigint and double.
	inline bool operator>(const double num) const { return this->operator>(Bigint(floor(num))); }
	/// Less-than comparison of Bigint and long double.
	inline bool operator>(const long double num) const { return this->operator>(Bigint(floor(num))); }
#endif // !BIGINT_ENABLE_FLOATS
	
// less than or equal to
	/// Less-than-or-equal-to comparison of Bigint and Bigint.
	inline bool operator<=(const Bigint& bint) const { return this->operator<(bint) || this->operator==(bint); }
	/// Less-than-or-equal-to comparison of Bigint and c-string representation of the number.
	inline bool operator<=(const char* strnum) const { return this->operator<=(Bigint(strnum)); }
	/// Less-than-or-equal-to comparison of Bigint and std::string representation of the number.
	inline bool operator<=(const std::string& strnum) const { return this->operator<=(Bigint(strnum)); }
	/// Less-than-or-equal-to comparison of Bigint and long long.
	inline bool operator<=(const llint num) const { return this->operator<=(Bigint(num)); }
	/// Less-than-or-equal-to comparison of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator<=(const int num) const { return this->operator<=(Bigint(num)); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Less-than comparison of Bigint and float.
	inline bool operator<=(const float num) const { return this->operator<(num) || this->operator==(num); }
	/// Less-than comparison of Bigint and double.
	inline bool operator<=(const double num) const { return this->operator<(num) || this->operator==(num); }
	/// Less-than comparison of Bigint and long double.
	inline bool operator<=(const long double num) const { return this->operator<(num) || this->operator==(num); }
#endif // !BIGINT_ENABLE_FLOATS

// greater than or equal to
	/// Greater-than-or-equal-to comparison of Bigint and Bigint.
	inline bool operator>=(const Bigint& bint) const { return this->operator>(bint) || this->operator==(bint); }
	/// Greater-than-or-equal-to comparison of Bigint and c-string representation of the number.
	inline bool operator>=(const char* strnum) const { return this->operator>=(Bigint(strnum)); }
	/// Greater-than-or-equal-to comparison of Bigint and std::string representation of the number.
	inline bool operator>=(const std::string& strnum) const { return this->operator>=(Bigint(strnum)); }
	/// Greater-than-or-equal-to comparison of Bigint and long long.
	inline bool operator>=(const llint num) const { return this->operator>=(Bigint(num)); }
	/// Greater-than-or-equal-to comparison of Bigint and (standard) int.
	/// Needed for the same reason as the Bigint(int) overload('ambiguity').
	inline bool operator>=(const int num) const { return this->operator>=(Bigint(num)); }
#ifdef BIGINT_ENABLE_FLOATS
	/// Less-than comparison of Bigint and float.
	inline bool operator>=(const float num) const { return this->operator>(num) || this->operator==(num); }
	/// Less-than comparison of Bigint and double.
	inline bool operator>=(const double num) const { return this->operator>(num) || this->operator==(num); }
	/// Less-than comparison of Bigint and long double.
	inline bool operator>=(const long double num) const { return this->operator>(num) || this->operator==(num); }
#endif // !BIGINT_ENABLE_FLOATS
	
	
/////////////////
// conversions
/////////////////
	/// Converts current bigint number to std::string.
	std::string toString() const;
	inline operator bool() const { if(this->m_bIsZero){ return false; } return true; }
	
/////////////////
// get-setters
/////////////////
	/// Returns the number length, in digits.
	inline ullint getNumberLength() const { return this->m_iNumLength; }
// TODO:Add compression here too
	/// Returns the number corresponding to the digit 'pos' in the number, in array index style, as unsigned char type.
	/// @note Returns is unsigned char type, convert to int where needed.
	inline ucint getDigitChar(const ullint pos) const { return (pos < this->m_vNumList.size()) ? this->m_vNumList[pos] : -1; }
	/// Returns the number corresponding to the digit 'pos' in the number, in array index style, as int type.
	inline int getDigit(const ullint pos) const { return int(this->getDigitChar(pos)); }
	/// Err, returns bigint's negative status(true if yes, false otherwise).
	inline bool isNegative() const { return this->m_bNegative; }
	inline bool isZero() const { return this-m_bIsZero; }
	/// Sets digit at position 'pos'(array index style) to value of 'val'.
	/// @note Unless 'force' is set, value 0 is ignored whenever setting it to digit higher than current m_iNumLength(to avoid '00123').
	void setDigit(const ucint val, const ullint pos, const bool force = false);
	/// Sets bigint's negative status.
	inline void setNegative(const bool neg) { this->m_bNegative = neg; }
	/// Clears the bigint and resets it's value back to 0.
	void clear();
	inline ullint getObjSize() const { return sizeof(Bigint) + m_vNumList.capacity() * sizeof(decltype(m_vNumList)::value_type); }
	
	
private:

/////////////////
// misc
/////////////////
	/// Copies value to 'this' from given Bigint.
	void copyFromBigint(const Bigint& bint);
	/// Copies value to 'this' from given std::string.
	void copyFromString(const std::string& strint, bool bSkipDecimal = true);
	/// Copies value to 'this' from given c-string.
	void copyFromCString(const char* arr, bool bSkipDecimal = true);
	/// Copies value to 'this' from given int(llint).
	void copyFromInt(const llint num);
#ifdef BIGINT_ENABLE_FLOATS
	/// Copies value to 'this' from given floating point value
	void copyFromFloat(const long double num);
#endif // !BIGINT_ENABLE_FLOATS

	/// Outputs the bigint to the standard output stream, std::ostream.
	friend std::ostream& operator<<(std::ostream& out, const Bigint& bint);
	
/////////////////
// variables
/////////////////

	/// Array of all number digits of the number.
	/// 0th element is digit of ones of the number.
	std::vector<ucint> m_vNumList;
	/// Length of the number, in digits, always >= 1.
	ullint m_iNumLength;
	/// Negative status of the variable.
	bool m_bNegative;
	/// Status: If the number equals to 0
	/// to get the status faster than comparing number itself. 
	bool m_bIsZero;
	
};








////////////////////////////////////////
// Bigint Constants and useful values
////////////////////////////////////////


/// \brief Struct with constants of max's and min's of the number types(static vars).
/// (currently featuring: all integral types, up to signed long long)
struct BigintConstants{
#ifdef BIGINT_ENABLE_CONSTANTS
	
//type max/min's

//integral types
	//long long's
	/// Representation of the long long's max.
	const inline static Bigint llintmax = (LLONG_MAX);
	/// Representation of the long long's min.
	const inline static Bigint llintmin = (LLONG_MIN);
	
	//just int's
	/// Representation of the int's max.
	const inline static Bigint intmax = (INT_MAX);
	/// Representation of the int's min.
	const inline static Bigint intmin = (INT_MIN);
	
	//u-int's
	/// Representation of the unsigned int's max.
	const inline static Bigint uintmax = llint(UINT_MAX);
	/// Representation of the unsigned int's min.
	const inline static Bigint uintmin = llint(0);
	
	//short'ies
	/// Representation of the short's max.
	const inline static Bigint shortmax = (SHRT_MAX);
	/// Representation of the short's min.
	const inline static Bigint shortmin = (SHRT_MIN);
	
	//unsigned short'ies
	/// Representation of the unsigned short's max
	const inline static Bigint ushortmax = (USHRT_MAX);
	/// Representation of the unsigned short's min
	const inline static Bigint ushortmin = (0);
	
	//char's
	/// Representation of the char's max.
	const inline static Bigint charmax = (SCHAR_MAX);
	/// Representation of the char's min.
	const inline static Bigint charmin = (SCHAR_MIN);
	
	//u-char's
	/// Representation of the unsigned char's max.
	const inline static Bigint ucharmax = (UCHAR_MAX);
	/// Representation of the unsigned char's max.
	const inline static Bigint ucharmin = (0);


#ifdef BIGINT_ENABLE_FLOATS
#ifdef BIGINT_ENABLE_FLOAT_CONSTANTS
//floats
	/// Representation of the float's max.
	const inline static Bigint floatmax = (FLT_MAX);
	/// Representation of the float's min.
	/// @note This value is not a FLT_MIN, but rather a negative representation of FLT_MAX; maximum-large negative number that a float can show
	const inline static Bigint floatmin = (-FLT_MAX);
	
	/// Representation of the double's max.
	const inline static Bigint doublemax = (DBL_MAX);
	/// Representation of the double's min.
	/// @note This value is not a DBL_MIN, but rather a negative representation of DBL_MAX; maximum-large negative number that a double can show
	const inline static Bigint doublemin = (-DBL_MAX);
	
	/// Representation of the long double's max.
	const inline static Bigint ldoublemax = (LDBL_MAX);
	/// Representation of the long double's min.
	/// @note This value is not a LDBL_MIN, but rather a negative representation of LDBL_MAX; maximum-large negative number that a long double can show
	const inline static Bigint ldoublemin = (-LDBL_MAX);
#endif // !BIGINT_ENABLE_FLOAT_CONSTANTS
#endif // !BIGINT_ENABLE_FLOATS
//special
	/// Zero value
	const inline static Bigint zero = (0);
	/// Positive 1
	const inline static Bigint onePos = (1);
	/// Negative 1
	const inline static Bigint oneNeg = (-1);
	
	/// Returns amount of memory used(bytes) for the whole BigintConstants struct
	inline static ullint getSize() {
		return (
			//max-min's
			llintmax.getObjSize() + llintmin.getObjSize() + //long long
			intmax.getObjSize() + intmin.getObjSize() +  //int
			uintmax.getObjSize() + uintmin.getObjSize() + //u-int
			shortmax.getObjSize() + shortmin.getObjSize() + //short
			ushortmax.getObjSize() + ushortmin.getObjSize() + //u-short
			charmax.getObjSize() + charmin.getObjSize() + //char
			ucharmax.getObjSize() + ucharmin.getObjSize() + //u-char
#ifdef BIGINT_ENABLE_FLOATS
			floatmax.getObjSize() + floatmin.getObjSize() + //float
			doublemax.getObjSize() + doublemin.getObjSize() + //double
			ldoublemax.getObjSize() + ldoublemin.getObjSize() + //long double
#endif // !BIGINT_ENABLE_FLOATS
			//special
			zero.getObjSize() + onePos.getObjSize() + 
			oneNeg.getObjSize()
			);
	}

#endif // !BIGINT_ENABLE_CONSTANTS
};


#endif // !BIGINT_HPP_INCLUDED
