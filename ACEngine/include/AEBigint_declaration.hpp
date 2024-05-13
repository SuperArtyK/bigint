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
constexpr int _AEBI_MAX_SECTOR_STORE_DIGITS = ace::math::lengthOfInt(_AEBI_MAX_SECTOR_STORE_VALUE);

/// The power of 10 of the largest value per sector to operate on.
#define _AEBI_MAX_SECTOR_OPERATION_P10 (powerOf10Table[18])

/// The largest value to perform operations on per sector.
#define _AEBI_MAX_SECTOR_OPERATION_VALUE (_AEBI_MAX_SECTOR_OPERATION_P10 - 1)

/// The default vector of sectors reserve size.
/// Default value is 32 sectors, total of 608 jeaiii::digits
#define AEBI_RESERVE_SIZE 32




/// <summary>
/// This is the engine's big int type -- it allows to perform arbitrary-wide integer arithmetic.
/// It works by using number sectors, instead of strings/characters for singular numbers.
/// Each number sector is a uint64 that can hold slightly more than int64.
/// 
/// Doing it this way, increases the number density (digit amount per byte) greatly.
/// 
/// And all of the operations are done on the sectors, which drastically improves performance on normal
/// operations like addition, but reduces performance on per-digit operations.
/// </summary>
class AEBigint {

public:

/////////////////
// constructors
/////////////////
	/// <summary>
	/// Default constructor -- constructs the bigint and sets it to 0.
	/// @note Sets the sector reserve amount to AEBI_RESERVE_SIZE
	/// </summary>
	AEBigint(void);

	/// <summary>
	/// Copy-constructor -- constructs and copies the data from passed bigint.
	/// @note Sets the sector reserve amount to AEBI_RESERVE_SIZE
	/// </summary>
	/// <param name="bint">The second bigint to construct from</param>
	AEBigint(const AEBigint& bint);

	/// <summary>
	/// Integer constructor -- constructs and sets the value to the passed  integral number
	/// @note Exists only to remove the "ambiguous" error. **A very big kludge**.
	/// </summary>
	/// <typeparam name="T">The type of integer</typeparam>
	/// <param name="num">The number to set the value to</param>
	template<typename T>
	AEBigint(const T num) requires(std::is_integral<T>::value) {
		dprintf("Constructing with a integral value (digits: %u)", ace::math::lengthOfInt<T>(num));
		this->copyFromInt<T>(num);
	}

	AEBigint(const std::string_view str);

	template<typename T>
	AEBigint(const T flt) requires(std::is_floating_point<T>::value) {
		dprintf("Constructing with a floating-point value (digits: %u, negative: %s)", ace::math::lengthOfFloat<T>(flt), ace::utils::boolToString(flt < 0).data());
		this->copyFromFloat(flt);
	}

	~AEBigint();


//////////////////////////////////
// assignment operators
// AEBigint_construction.cpp
//////////////////////////////////
	AEBigint& operator=(const AEBigint& bint);

	AEBigint& operator=(const std::string_view str);

	template<typename T>
	inline AEBigint& operator=(const T num) requires(std::is_integral<T>::value); // defined below class

	template<typename T>
	inline AEBigint& operator=(const T flt) requires(std::is_floating_point<T>::value); // defined below class


//////////////////////////////////
// comparison operators
// AEBigint_construction.cpp
//////////////////////////////////
// equality
/////////////////
	bool operator==(const AEBigint& bint) const noexcept;

	bool operator==(const std::string_view str) const;

	template<typename T>
	inline bool operator==(const T num) const noexcept requires(std::is_integral<T>::value);

	template<typename T>
	inline bool operator==(const T flt) const requires(std::is_floating_point<T>::value);

// inequality
/////////////////
	inline bool operator!=(const AEBigint& bint) const noexcept {
		return !this->operator==(bint);
	}

	inline bool operator!=(const std::string_view str) const {
		return !this->operator==(str);
	}

	template<typename T>
	inline bool operator!=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator==<T>(num);
	}

	template<typename T>
	inline bool operator!=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator==<T>(flt);
	}

// greater than
/////////////////
	bool operator>(const AEBigint& bint) const noexcept;

	bool operator>(const std::string_view str) const {
		return this->compareString<true>(str);
	}

	template<typename T>
	inline bool operator>(const T num) const noexcept requires(std::is_integral<T>::value) {
		return this->compareInt<T, true>(num);
	}

	template<typename T>
	inline bool operator>(const T flt) const requires(std::is_floating_point<T>::value) {
		return this->compareFloat<T, true>(flt);
	}

// greater than or equal to
//////////////////////////////////
	bool operator>=(const AEBigint& bint) const noexcept {
		return !this->operator<(bint);
	}

	bool operator>=(const std::string_view str) const {
		return !this->operator<(str);
	}

	template<typename T>
	inline bool operator>=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator< <T>(num);
	}

	template<typename T>
	inline bool operator>=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator< <T>(flt);
	}

// less than
/////////////////
	inline bool operator<(const AEBigint& bint) const noexcept {
		return bint.operator>(*this);
	}

	inline bool operator<(const std::string_view str) const {
		return this->compareString<false>(str);
	}

	template<typename T>
	inline bool operator<(const T num) const noexcept requires(std::is_integral<T>::value) {
		return this->compareInt<T, false>(num);
	}

	template<typename T>
	inline bool operator<(const T flt) const requires(std::is_floating_point<T>::value) {
		return this->compareFloat<T, false>(flt);
	}

// less than or equal to
//////////////////////////////////
	inline bool operator<=(const AEBigint& bint) const noexcept {
		return !this->operator>(bint);
	}

	inline bool operator<=(const std::string_view str) const {
		return !this->operator>(str);
	}

	template<typename T>
	inline bool operator<=(const T num) const noexcept requires(std::is_integral<T>::value) {
		return !this->operator> <T>(num);
	}

	template<typename T>
	inline bool operator<=(const T flt) const requires(std::is_floating_point<T>::value) {
		return !this->operator> <T>(flt);
	}

/////////////////
// getters
/////////////////
	[[nodiscard]] inline bool isZero(void) const noexcept {
		return this->size() == 1 && this->getFirstSector() == 0;
	}

	[[nodiscard]] inline bool isNegative(void) const noexcept {
		return this->m_bNegative;
	}

	[[nodiscard]] inline bool isPositive(void) const noexcept {
		return !this->m_bNegative;
	}

	[[nodiscard]] inline ullint size(void) const noexcept {
		return this->m_ullSize;
	}

	[[nodiscard]] inline ullint length(void) const noexcept {
		return this->size();
	}

	[[nodiscard]] inline std::size_t getSectorAmount(void) const noexcept {
		return this->m_vecSectors.size();
	}

	[[nodiscard]] inline ucint getDigitChar(const ullint dig) const noexcept {
		if (dig == 0) { return this->getFirstSector() % 10; }
		return (this->getSector(dig / _AEBI_MAX_SECTOR_STORE_DIGITS) / 
			powerOf10Table[(dig % _AEBI_MAX_SECTOR_STORE_DIGITS)]) 
			% 10;
	}

	[[nodiscard]] inline int getDigit(const ullint dig) const noexcept {
		return int(this->getDigitChar(dig));
	}

	[[nodiscard]] inline ullint getSector(const std::size_t sector) const noexcept {
		return this->m_vecSectors[sector];
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

	friend std::ostream& operator<<(std::ostream& out, const AEBigint& bint);

	void toCString(char* dataptr) const noexcept;

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
	template<typename T>
	inline void copyFromInt(const T num) requires(std::is_integral<T>::value); // defined below class

	void copyFromString(const std::string_view str, const bool check = true);

	template<typename T>
	inline void copyFromFloat(const T flt) requires(std::is_floating_point<T>::value); // defined below class


	template<const bool greaterThan>
	inline bool compareString(const std::string_view str) const;

	template<typename T, const bool greaterThan>
	inline bool compareInt(const T num) const noexcept requires(std::is_integral<T>::value);

	template<typename T, const bool greaterThan>
	inline bool compareFloat(const T flt) const requires(std::is_integral<T>::value);

	/// The vector that contains all the number sectors
	std::vector<ullint> m_vecSectors;
	/// The size of the number in jeaiii::digits
	ullint m_ullSize;
	/// The flag whether the number is negative
	bool m_bNegative;

};


#endif // !ENGINE_BIGINT_DECLARATION_HPP