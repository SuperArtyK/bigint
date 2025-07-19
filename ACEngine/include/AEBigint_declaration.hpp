#pragma once

#ifndef ENGINE_BIGINT_DECLARATION_HPP
#define ENGINE_BIGINT_DECLARATION_HPP

#include "include/AEMath.hpp"
#include "include/AETypedefs.hpp"
#include "include/AEUtils.hpp"
#include "include/AEUtilsMacros.hpp"
#include "jeaiii_to_text.h"
#include <charconv>
#include <array>


/// The lookup table of the powers of 10
/// @note Maximum value is 10^20, since it's the largest that ::ullint can hold.
constexpr ullint powerOf10Table[21] {
	1,                       powerOf10Table[0] * 10,  powerOf10Table[1] * 10,  powerOf10Table[2] * 10,  powerOf10Table[3] * 10,   //power 0-4
	powerOf10Table[4] * 10,  powerOf10Table[5] * 10,  powerOf10Table[6] * 10,  powerOf10Table[7] * 10,  powerOf10Table[8] * 10,   //power 5-9
	powerOf10Table[9] * 10,  powerOf10Table[10] * 10, powerOf10Table[11] * 10, powerOf10Table[12] * 10, powerOf10Table[13] * 10,  //power 10-14
	powerOf10Table[14] * 10, powerOf10Table[15] * 10, powerOf10Table[16] * 10, powerOf10Table[17] * 10, powerOf10Table[18] * 10,  //power 11-19
	powerOf10Table[19] * 10, // power 20
};


/// The largest representable power of 10 per number sector
#define _AEBI_MAX_SECTOR_STORE_P10 (powerOf10Table[19])

/// The largest representable value per number sector
/// @note Corresponds to the 10^19 - 1, the largest value of 9999.... that can fit into uint64
#define _AEBI_MAX_SECTOR_STORE_VALUE (_AEBI_MAX_SECTOR_STORE_P10 - 1)

/// The length in jeaiii::digits of the largest representable value per number sector
constexpr uint _AEBI_MAX_SECTOR_STORE_DIGITS = ace::math::lengthOfInt(_AEBI_MAX_SECTOR_STORE_VALUE);

/// The power of 10 of the largest value per sector to operate on.
#define _AEBI_MAX_SECTOR_OPERATION_P10 (powerOf10Table[18])

/// The largest value to perform operations on per sector.
#define _AEBI_MAX_SECTOR_OPERATION_VALUE (_AEBI_MAX_SECTOR_OPERATION_P10 - 1)






/// <summary>
/// This is the engine's big int type -- it allows to perform arbitrary-wide integer arithmetic.
/// It works by using number sectors, instead of strings/characters for singular numbers.
/// Each number sector is a uint64 that can hold slightly more than int64.
/// 
/// Doing it this way, increases the number density (digit amount per byte) greatly.
/// 
/// And all of the operations are done on the sectors, which drastically improves performance on normal
/// operations like addition, but reduces performance on per-digit operations.
/// @todo Add a flag to enable exceptions in string_view functions
/// @todo Make functions abstracted from the number cell type
/// @todo Make the cell type a typedef and fix constants/macros that play on the type limits (e.g. _AEBI_MAX_SECTOR_STORE_P10)
/// @todo Make a separate function to modify a cell, make it force inline
/// @todo Add compatibility with functions that work with non-bigints and that work on the cell directly (e.g. AEBigint::copyFromInt() )
/// @todo Add the const char* function overloads (inline), that would call the std::string_view overloads under the hood.
/// @todo Create a flag to preallocate the sectors on default constructor; This will allow to mark default constructor as noexcept if set to false.
/// @todo Add functionality to reserve more cells in vector after modifying operations.
/// </summary>
class AEBigint {

public:
	
/////////////////
// constructors
/////////////////
	/// <summary>
	/// Default constructor -- constructs the bigint and sets it to 0.
	/// 
	/// @note Sets the sector reserve amount to AEBI_RESERVE_SIZE
	/// </summary>
#if AEBI_DEFAULT_CSTOR_PREALLOCATE == 1
	inline AEBigint(void) {
		dprintf("Constructing an empty AEBigint");
		this->clear(true);
	}
#else
	inline AEBigint(void) noexcept : m_ullSize(1) {
		dprintf("Constructing an empty AEBigint");
	}
#endif

	/// <summary>
	/// Copy-constructor -- constructs and copies the data from passed bigint.
	/// 
	/// @note Sets the sector reserve amount to AEBI_RESERVE_SIZE (which is applied only if the size of copied big int is less than AEBI_RESERVE_SIZE)
	/// </summary>
	/// <param name="bint">The second bigint to construct from</param>
	AEBigint(const AEBigint& bint);

	/// <summary>
	/// Integer constructor -- constructs and sets the value from the passed integral number
	/// 
	/// @note Exists only to remove the "ambiguous" error. **A very big kludge**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The number to set the bigint value to</param>
	template<typename T>
	inline AEBigint(const T num) requires(std::is_integral<T>::value) {
		dprintf("Constructing with a integral value (digits: %u)", ace::math::lengthOfInt<T>(num));
		this->copyFromInt<T>(num);
	}

	/// <summary>
	/// String-view constructor -- constructs and sets the value from the passed string (as string-view)
	/// 
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// @note If the passed string is invalid, sets the value to 0 instead.
	/// @attention The string view needs to be valid (that reading it won't cause a read access violation), otherwise, as always, it may explode.
	/// </summary>
	/// <param name="str">The string view to set the bigint value to</param>	
	AEBigint(const std::string_view str);

	/// <summary>
	/// Float constructor -- constructs and sets the value from the passed floating point
	/// </summary>
	/// <typeparam name="T">The type of the passed float</typeparam>
	/// <param name="flt">The number to set the bigint value to</param>
	template<typename T>
	inline AEBigint(const T flt) requires(std::is_floating_point<T>::value) {
		dprintf("Constructing with a floating-point value (digits: %u, negative: %s)", ace::math::lengthOfFloat<T>(flt), ace::utils::boolToString(flt < 0).data());
		this->copyFromFloat(flt);
	}

	/// <summary>
	/// The destructor
	/// </summary>
	~AEBigint();


//////////////////////////////////
// assignment operators
// AEBigint_construction.cpp
//////////////////////////////////

	/// <summary>
	/// Copy-assignment -- assigns the value from the passed bigint
	/// </summary>
	/// <param name="bint">The bigint to copy</param>
	/// <returns>
	///		The reference to **this** (for chain assignment)
	///	</returns>
	AEBigint& operator=(const AEBigint& bint);

	/// <summary>
	/// String-assignment -- assigns the value from the passed string view
	/// 
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// @note If the passed string is invalid, sets the value to 0 instead
	/// </summary>
	/// <param name="bint">The string to set the value from</param>
	/// <returns>
	///		The reference to **this** (for chain assignment)
	///	</returns>
	AEBigint& operator=(const std::string_view str);

	/// <summary>
	/// Integer-assignment -- assigns the value from the passed integral number
	///	
	/// @note Exists only to remove the "ambiguous" error. **A very big kludge**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The number to set the bigint value to</param>
	/// <returns>
	///		The reference to **this** (for chain assignment)
	///	</returns>
	template<typename T>
	inline AEBigint& operator=(const T num) requires(std::is_integral<T>::value); // defined below class

	/// <summary>
	/// Float-assignment -- assigns the value from the passed floating point
	/// </summary>
	/// <typeparam name="T">The type of the passed float</typeparam>
	/// <param name="flt">The number to set the bigint value to</param>
	/// <returns>
	///		The reference to **this** (for chain assignment)
	///	</returns>
	template<typename T>
	inline AEBigint& operator=(const T flt) requires(std::is_floating_point<T>::value); // defined below class


//////////////////////////////////
// comparison operators
// AEBigint_construction.cpp
//////////////////////////////////
// equality
/////////////////
	
	/// <summary>
	/// Comparison (equality) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If both numbers are equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	bool operator==(const AEBigint& bint) const noexcept;

	/// <summary>
	/// Comparison (equality) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** and the number converted from the **str** are equal:
	///		* **true**
	///		
	///		Otherwise, or if the **str** doesn't contain a valid number:
	///		* **false**
	/// </returns>
	bool operator==(const std::string_view str) const;

	/// <summary>
	/// Comparison (equality) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** and **num** are equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator==(const T num) const noexcept requires(std::is_integral<T>::value);

	/// <summary>
	/// Comparison (equality) between **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** and value converted from **flt** are equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator==(const T flt) const requires(std::is_floating_point<T>::value);

// inequality
/////////////////
	
	/// <summary>
	/// Comparison (inequality) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If both numbers are mot equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	inline bool operator!=(const AEBigint& bint) const noexcept {
		return !this->operator==(bint);
	}

	/// <summary>
	/// Comparison (equality) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** and the number converted from the **str** are not equal, or if the **str** doesn't contain a valid number:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	inline bool operator!=(const std::string_view str) const {
		return !this->operator==(str);
	}

	/// <summary>
	/// Comparison (inequality) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** and **num** are mot equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator!=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator==<T>(num);
	}

	/// <summary>
	/// Comparison (inequality) with **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** and value converted from **flt** are not equal:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator!=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator==<T>(flt);
	}

// greater than
/////////////////
	
	/// <summary>
	/// Comparison (greater than) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If **this** is greater than **bint**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	bool operator>(const AEBigint& bint) const noexcept;

	/// <summary>
	/// Comparison (greater than) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** is greater than the number converted from the **str** are not equal:
	///		* **true**
	///		
	///		Otherwise, or if the **str** doesn't contain a valid numbe:
	///		* **false**
	/// </returns>
	bool operator>(const std::string_view str) const {
		return this->compareString<true>(str);
	}

	/// <summary>
	/// Comparison (greater than) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** is greater than the **num**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator>(const T num) const noexcept requires(std::is_integral<T>::value) {
		return this->compareInt<T, true>(num);
	}

	/// <summary>
	/// Comparison (greater than) between **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** is greater than the value converted from **flt**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator>(const T flt) const requires(std::is_floating_point<T>::value) {
		return this->compareFloat<T, true>(flt);
	}

// greater than or equal to
//////////////////////////////////

	/// <summary>
	/// Comparison (greater than or equal to) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If **this** is greater than or equal to **bint**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	bool operator>=(const AEBigint& bint) const noexcept {
		return !this->operator<(bint);
	}

	/// <summary>
	/// Comparison (greater than or equal to) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** is greater than or equal to the number converted from the **str** are not equal:
	///		* **true**
	///		
	///		Otherwise, or if the **str** doesn't contain a valid numbe:
	///		* **false**
	/// </returns>
	bool operator>=(const std::string_view str) const {
		return !this->operator<(str);
	}

	/// <summary>
	/// Comparison (greater than or equal to) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** is greater than or equal to the **num**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator>=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator< <T>(num);
	}
	
	/// <summary>
	/// Comparison (greater than or equal to) between **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** is greater than or equal to the value converted from **flt**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator>=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator< <T>(flt);
	}

// less than
/////////////////
	
	/// <summary>
	/// Comparison (less than) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If **this** is less than **bint**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	inline bool operator<(const AEBigint& bint) const noexcept {
		return bint.operator>(*this);
	}

	/// <summary>
	/// Comparison (less than) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** is less than the number converted from the **str** are not equal:
	///		* **true**
	///		
	///		Otherwise, or if the **str** doesn't contain a valid numbe:
	///		* **false**
	/// </returns>
	inline bool operator<(const std::string_view str) const {
		return this->compareString<false>(str);
	}

	/// <summary>
	/// Comparison (less than) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** is less than the **num**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator<(const T num) const noexcept requires(std::is_integral<T>::value) {
		return this->compareInt<T, false>(num);
	}

	/// <summary>
	/// Comparison (less than) between **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** is less than the value converted from **flt**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator<(const T flt) const requires(std::is_floating_point<T>::value) {
		return this->compareFloat<T, false>(flt);
	}

// less than or equal to
//////////////////////////////////

	/// <summary>
	/// Comparison (less than or equal to) between **this** and passed AEBigint, **bint**.
	/// </summary>
	/// <param name="bint">The AEBigint number to compare</param>
	/// <returns>
	///		If **this** is less than or equal to **bint**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	inline bool operator<=(const AEBigint& bint) const noexcept {
		return !this->operator>(bint);
	}

	/// <summary>
	/// Comparison (less than or equal to) between **this** and passed std::string_view, **str**.
	///
	/// @note See ace::utils::isNum() for rules for a std::string_view to be interpreted as a valid number.
	/// </summary>
	/// <param name="str">The std::string_view supposedly containing a number</param>
	/// <returns>
	///		If **this** is less than or equal to the number converted from the **str** are not equal:
	///		* **true**
	///		
	///		Otherwise, or if the **str** doesn't contain a valid numbe:
	///		* **false**
	/// </returns>
	inline bool operator<=(const std::string_view str) const {
		return !this->operator>(str);
	}

	/// <summary>
	/// Comparison (less than or equal to) between **this** and passed integral type, **num**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The integral type value to compare</param>
	/// <returns>
	///		If **this** is less than or equal to the **num**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator<=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator> <T>(num);
	}

	/// <summary>
	/// Comparison (less than or equal to) between **this** and passed floating point type, **flt**.
	/// </summary>
	/// <typeparam name="T">The type of floating point</typeparam>
	/// <param name="num">The floating type value to compare</param>
	/// <returns>
	///		If **this** is less than or equal to the value converted from **flt**:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	template<typename T>
	inline bool operator<=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator> <T>(flt);
	}

/////////////////
// getters
/////////////////

	/// <summary>
	/// Checks if **this** is equivalent to zero (1 digit and first sector is 0)
	/// </summary>
	/// <returns>
	///		If the number is zero:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	[[nodiscard]] inline bool isZero(void) const noexcept {
		return this->size() == 1 && (this->getFirstSector() == 0); // fix for vector being empty
	}

	/// <summary>
	/// Checks if **this** is negative (negative flag is set)
	/// </summary>
	/// <returns>
	///		If the number is negative:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	[[nodiscard]] inline bool isNegative(void) const noexcept {
		return this->m_bNegative;
	}

	/// <summary>
	/// Checks if **this** is positive (negative flag is unset)
	/// </summary>
	/// <returns>
	///		If the number is positive:
	///		* **true**
	///		
	///		Otherwise:
	///		* **false**
	/// </returns>
	[[nodiscard]] inline bool isPositive(void) const noexcept {
		return !this->m_bNegative;
	}

	/// <summary>
	/// Returns the length of the number in digits **this**
	/// 
	/// @note Equivalent to AEBigint::length(void)
	/// @see AEBigint::length(void)
	/// </summary>
	/// <returns>
	///		Length of the number in digits as type **ullint**.
	/// </returns>
	[[nodiscard]] inline ullint size(void) const noexcept {
		return this->m_ullSize;
	}

	/// <summary>
	/// Returns the length of the number in digits in **this**
	/// 
	/// @note Equivalent to AEBigint::size(void)
	/// @see AEBigint::size(void)
	/// </summary>
	/// <returns>
	///		Length of the number in digits as type **ullint**.
	/// </returns>
	[[nodiscard]] inline ullint length(void) const noexcept {
		return this->size();
	}

	/// <summary>
	/// Returns the amount of number sectors in **this** (length of value-storing **std::vector**)
	/// </summary>
	/// <returns>
	///		Amount of number sectors as type **std::size_t**.
	/// </returns>
	[[nodiscard]] inline std::size_t getSectorAmount(void) const noexcept {
		return this->m_vecSectors.size();
	}
	
	/// <summary>
	/// Returns the single numeric digit of **this** as **char* (ascii character representation)
	/// 
	/// </summary>
	/// <param name="dig">The n'th digit to retrieve (zero-indexed)</param>
	/// <returns>
	///		The selected digit of **this** as type **char** (direct value not ascii text)
	/// </returns>
	[[nodiscard]] inline char getDigitChar(const ullint dig) const noexcept {
		if (dig == 0ull) { return ace::utils::intToNumChar<true>(this->getFirstSector() % 10ull); }
		return ace::utils::intToNumChar<true>((this->getSector(dig / _AEBI_MAX_SECTOR_STORE_DIGITS) /
			powerOf10Table[(dig % _AEBI_MAX_SECTOR_STORE_DIGITS)])
			% 10);
	}

	[[nodiscard]] inline ullint getDigit(const ullint dig) const noexcept {
		if (dig == 0) { return this->getFirstSector() % 10ull; }
		return (this->getSector(dig / _AEBI_MAX_SECTOR_STORE_DIGITS) /
			powerOf10Table[(dig % _AEBI_MAX_SECTOR_STORE_DIGITS)])
			% 10;
	}

	[[nodiscard]] inline ullint getSector(const std::size_t sector) const noexcept {
		return (this->getSectorAmount()) ? this->m_vecSectors[sector] : 0ull;
	}

	[[nodiscard]] inline std::size_t getMemoryUsage() const noexcept {
		return sizeof(AEBigint) + this->m_vecSectors.capacity() * sizeof(ullint);
	}

	[[nodiscard]] inline ullint getLastSector(void) const noexcept {
		return this->getSector(this->m_vecSectors.size() - 1);
	}

	[[nodiscard]] inline ullint getFirstSector(void) const noexcept {
		return this->getSector(0);
	}

/////////////////
// setters
/////////////////
	inline AEBigint& setNegativity(const bool negative) noexcept {
		this->m_bNegative = negative;
		return *this;
	}

	void setDigit(const ullint dig, const ucint val);

	void setSector(const std::size_t sector, const ullint val);

	inline void setLastSector(const ullint val) { this->setSector(this->getSectorAmount() - 1, val); }

	inline void setFirstSector(const ullint val) { this->setSector(0, val); }


//////////////////////////////////
// miscellanea
// AEBigint_miscellanea.cpp
//////////////////////////////////
	[[nodiscard]] inline AEBigint operator-(void) const {
		AEBigint tmp = *this;
		tmp.m_bNegative = !tmp.m_bNegative;
		return tmp;
	}

	[[nodiscard]] inline static AEBigint abs(const AEBigint& bint) {
		if (bint.isNegative()) { return -bint; }
		return bint;
	}

	void clear(const bool setToZero = true);


//////////////////////////////////
// conversions
// AEBigint_miscellanea.cpp
//////////////////////////////////
	[[nodiscard]] std::string toString(void) const;

	[[nodiscard]] inline operator bool(void) const noexcept { return !this->isZero(); }

	[[nodiscard]] inline operator std::string(void) const { return this->toString(); }

	void toCString(char* dataptr) const noexcept;

	friend std::ostream& operator<<(std::ostream& out, const AEBigint& bint);



//////////////////////////////////
// basic arithmetic
// AEBigint_arithmetic.cpp
//////////////////////////////////

	/// <summary>
	/// Performs raw self addition of **bint** to **this** (without looking into the 
	/// </summary>
	/// <param name="bint">The AEBigint value to add</param>
	void rawSelfAdd(const AEBigint& bint);

private:

	template<class T>
#if defined(_MSC_VER)
	__forceinline
#else
	inline
#endif
	static constexpr char* sectorToCString(char* b, const T i) noexcept; // defined below class

//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////
	
	/// <summary>
	/// Perform a copy-assignment operation on **this** using the value in **bint**
	/// </summary>
	/// <param name="bint">The AEBigint value to copy from</param>
	void copyFromBigint(const AEBigint& bint);

	template<typename T>
	inline void copyFromInt(const T num) requires(std::is_integral<T>::value); // defined below class

	bool copyFromString(const std::string_view str, const bool check = true);

	template<typename T>
	inline void copyFromFloat(const T flt) requires(std::is_floating_point<T>::value); // defined below class

//////////////////////////////////
// Comparison
// AEBigint_construction.cpp
//////////////////////////////////
	template<const bool greaterThan>
	inline bool compareString(const std::string_view str) const;

	template<typename T, const bool greaterThan>
	inline bool compareInt(const T num) const noexcept requires(std::is_integral<T>::value);

	template<typename T, const bool greaterThan>
	inline bool compareFloat(const T flt) const requires(std::is_integral<T>::value);




//////////////////////
// VARIABLES
//////////////////////
	/// The vector that contains all the number sectors
	std::vector<ullint> m_vecSectors;
	/// The size of the number in digits
	ullint m_ullSize;
	/// The flag whether the number is negative
	bool m_bNegative;

};

inline AEBigint operator ""_bi(const char* str, const std::size_t sz) {
	return AEBigint(std::string_view(str, sz));
}


#endif // !ENGINE_BIGINT_DECLARATION_HPP
