#ifndef BIGINT_HPP_INCLUDED
#define BIGINT_HPP_INCLUDED


#include <vector>
#include <string>
#include <cstring>
#include "utils.hpp"
#include <limits>
#include <climits>

#define BIGINT_PREALLOC_SIZE 32



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
	

/////////////////
// operators
/////////////////

//assigments
    /// Copies value of the given Bigint to itself.
    Bigint& operator=(const Bigint& bint);
    /// Copies int value in the (std::)string to itself.
	Bigint& operator=(const std::string& strnum);
	/// Copies int value in the (c-)string to itself.
	Bigint& operator=(const char* strnum);
	/// Copies int value to itself.
	Bigint& operator=(const llint num);

//comparisons
//equality
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
//inequality
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
//less than
    /// Less-than comparison of Bigint and Bigint
    bool operator<(const Bigint& bint) const;
    /// Less-than comparison of Bigint and c-string representation of the number.
    bool operator<(const char* strnum) const;
    /// Less-than comparison of Bigint and std::string representation of the number.
    bool operator<(const std::string& strnum) const;
    /// Less-than comparison of Bigint and long long
    bool operator<(const llint num) const;
    /// Less-than comparison of Bigint and (standard) int
    /// Needed for the same reason as the Bigint(int) overload('ambiguity').
    inline bool operator<(const int num) const { return this->operator<((llint)num); }
//greater-than
    /// Greater-than comparison of Bigint and Bigint
    bool operator>(const Bigint& bint) const;
    /// Greater-than comparison of Bigint and c-string representation of the number.
    bool operator>(const char* strnum) const;
    /// Greater-than comparison of Bigint and std::string representation of the number.
    bool operator>(const std::string& strnum) const;
    /// Greater-than comparison of Bigint and long long
    bool operator>(const llint num) const;
    /// Greater-than comparison of Bigint and (standard) int
    /// Needed for the same reason as the Bigint(int) overload('ambiguity').
    inline bool operator>(const int num) const { return this->operator>(Bigint(num)); }
//less than or equal to
    /// Less-than-or-equal-to comparison of Bigint and Bigint
    inline bool operator<=(const Bigint& bint) const { return this->operator<(bint) || this->operator==(bint); }
    /// Less-than-or-equal-to comparison of Bigint and c-string representation of the number.
//don't care about the quick checks.
//they will be done anyway in the called functions
    inline bool operator<=(const char* strnum) const { return this->operator<=(Bigint(strnum)); }
    /// Less-than-or-equal-to comparison of Bigint and std::string representation of the number.
    inline bool operator<=(const std::string& strnum) const { return this->operator<=(Bigint(strnum)); }
    /// Less-than-or-equal-to comparison of Bigint and long long
    inline bool operator<=(const llint num) const { return this->operator<=(Bigint(num)); }
    /// Less-than-or-equal-to comparison of Bigint and (standard) int
    /// Needed for the same reason as the Bigint(int) overload('ambiguity').
    inline bool operator<=(const int num) const { return this->operator<=(Bigint(num)); }
//greater than or equal to
    /// Greater-than-or-equal-to comparison of Bigint and Bigint
    inline bool operator>=(const Bigint& bint) const { return this->operator>(bint) || this->operator==(bint); }
    /// Greater-than-or-equal-to comparison of Bigint and c-string representation of the number.
//don't care about the quick checks.
//they will be done anyway in the called functions
    inline bool operator>=(const char* strnum) const { return this->operator>=(Bigint(strnum)); }
    /// Greater-than-or-equal-to comparison of Bigint and std::string representation of the number.
    inline bool operator>=(const std::string& strnum) const { return this->operator>=(Bigint(strnum)); }
    /// Greater-than-or-equal-to comparison of Bigint and long long
    inline bool operator>=(const llint num) const { return this->operator>=(Bigint(num)); }
    /// Greater-than-or-equal-to comparison of Bigint and (standard) int
    /// Needed for the same reason as the Bigint(int) overload('ambiguity').
    inline bool operator>=(const int num) const { return this->operator>=(Bigint(num)); }
    
    
/////////////////
// conversions
/////////////////

    /// Converts current bigint number to std::string.
    std::string toString() const;
    inline operator bool() const { if(this->m_isZero){ return false; } return true; }
/////////////////
// get-setters
/////////////////

    /// Returns the number length, in digits.
	inline ullint getNumberLength() const { return this->m_iNumLength; }
//TODO:Add compression here too
    /// Returns the number corresponding to the digit 'pos' in the number, in array index style, as unsigned char type.
    /// @note Returns is unsigned char type, convert to int where needed.
	inline ucint getDigitChar(const ullint pos) const { return (pos < this->m_vNumList.size()) ? this->m_vNumList[pos] : -1; }
	/// Returns the number corresponding to the digit 'pos' in the number, in array index style, as int type.
	inline int getDigit(const ullint pos) const { return int(this->getDigitChar(pos)); }
	/// Err, returns bigint's negative status(true if yes, false otherwise).
	inline bool isNegative() const { return this->m_bNegative; }
	/// Sets digit at position 'pos'(array index style) to value of 'val'.
	/// @note Unless 'force' is set, value 0 is ignored whenever setting it to digit higher than current m_iNumLength(to avoid stuff like 00123).
	void setDigit(const ucint val, const ullint pos, const bool force = false);
	/// Sets bigint's negative status.
	inline void setNegative(const bool neg) { this->m_bNegative = neg; }
	/// Clears the bigint and resets it's value back to 0.
    void clear();
	
private:

/////////////////
// misc
/////////////////

    /// Copies value to 'this' from given Bigint.
    void copyFromBigint(const Bigint& bint);
    /// Copies value to 'this' from given std::string.
    void copyFromString(const std::string& strint);
    /// Copies value to 'this' from given c-string.
    void copyFromCString(const char* arr);
    /// Copies value to 'this' from given int(llint).
    void copyFromInt(llint num);
    
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
	/// Returns status: If the number equals to 0
	/// to get the status faster than comparing number itself. 
	bool m_isZero;
};



/// \brief Struct with constants of max's and min's of the number types(static vars).
/// (currently featuring: all integral types, up to signed long long)
struct BigintConstants{
//type max/min's
    
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
	const inline static Bigint uintmax = llint(std::numeric_limits<unsigned int>::max());
    /// Representation of the unsigned int's min.
	const inline static Bigint uintmin = llint(std::numeric_limits<unsigned int>::min());
	
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
	
//special
	/// Zero value
	const inline static Bigint zero = (0);
	/// Positive 1
	const inline static Bigint onePos = (1);
	/// Negative 1
	const inline static Bigint oneNeg = (-1);
	
};


#endif // !BIGINT_HPP_INCLUDED
