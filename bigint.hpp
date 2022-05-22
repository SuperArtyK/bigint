#ifndef BIGINT_HPP_INCLUDED
#define BIGINT_HPP_INCLUDED


#include <vector>
#include <string>
#include <cstring>
#include "utils.hpp"
#include <limits>

#define BIGINT_PREALLOC_SIZE 32



/// \brief Implementation of the big integers, expansion of the built-in integer types of c++.
/// For indexing separate digits, array index style is used.
/// Also, it is little-endian in a sense. 
/// Least significant digits, ones, are stored first, at index 0
class Bigint{

public:
//constructors
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
    /// Checks for equality of both Bigint's.
    bool operator==(const Bigint& bint) const;
    
//conversions
    /// Converts current bigint number to std::string.
	std::string toString() const;


//quick getters, all inline, in the header
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
	
//setters
	/// Sets digit at position 'pos'(array index style) to value of 'val'.
	/// @note Unless 'force' is set, value 0 is ignored whenever setting it to digit higher than current m_iNumLength(to avoid stuff like 00123).
	void setDigit(const ucint val, const ullint pos, const bool force = false);
	/// Sets bigint's negative status.
	inline void setNegative(const bool neg) { this->m_bNegative = neg; }
	/// Clears the bigint and resets it's value back to 0.
    void clear();
	
private:
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
    
    /// Array of all number digits of the number.
    /// 0th element is digit of ones of the number.
	std::vector<ucint> m_vNumList;
	/// Length of the number, in digits, always >= 1.
	ullint m_iNumLength;
	/// Negative status of the variable.
	bool m_bNegative;
	
};

/// \brief Struct with constants of max's and min's of the number types(static vars).
/// (currently featuring: all integral types, up to signed long long)
struct BigintConstants{
    //long long's
    ///Representation of the long long's max.
    const inline static Bigint llintmax = (std::numeric_limits<llint>::max());
    ///Representation of the long long's min.
	const inline static Bigint llintmin = (std::numeric_limits<llint>::min());
	
	//just int's
    ///Representation of the int's max.
	const inline static Bigint intmax = (std::numeric_limits<int>::max());
    ///Representation of the int's min.
	const inline static Bigint intmin = (std::numeric_limits<int>::min());
	
	//u-int's
    ///Representation of the unsigned int's max.
	const inline static Bigint uintmax = llint(std::numeric_limits<unsigned int>::max());
    ///Representation of the unsigned int's min.
	const inline static Bigint uintmin = llint(std::numeric_limits<unsigned int>::min());
	
	//short'ies
    ///Representation of the short's max.
	const inline static Bigint shortmax = (std::numeric_limits<short>::max());
    ///Representation of the short's min.
	const inline static Bigint shortmin = (std::numeric_limits<short>::min());
	
	//unsigned short'ies
    ///Representation of the unsigned short's max
	const inline static Bigint ushortmax = (std::numeric_limits<unsigned short>::max());
    ///Representation of the unsigned short's min
	const inline static Bigint ushortmin = (std::numeric_limits<unsigned short>::min());
	
	//char's
    ///Representation of the char's max.
	const inline static Bigint charmax = (std::numeric_limits<char>::max());
    ///Representation of the char's min.
	const inline static Bigint charmin = (std::numeric_limits<char>::min());
	
	//u-char's
    ///Representation of the unsigned char's max.
	const inline static Bigint ucharmax = (std::numeric_limits<unsigned char>::max());
    ///Representation of the unsigned char's max.
	const inline static Bigint ucharmin = (std::numeric_limits<unsigned char>::min());
};


#endif // !BIGINT_HPP_INCLUDED
