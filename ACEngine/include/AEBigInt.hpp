#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP

#include "include/AEMath.hpp"
#include "include/AETypedefs.hpp"
#include "include/AEUtils.hpp"
#include "include/AEUtilsMacros.hpp"
#include "jeaiii_to_text.h"
#include <charconv>
#include <array>

namespace ace::utils {
	/// <summary>
	/// Reverses the c-string of the given length (excluding null)
	/// </summary>
	/// <param name="str">The c-string to reverse</param>
	/// <param name="len">The </param>
	inline void revCString(char* str, const std::size_t len) noexcept {
		char* ptr = str + len - 1;
		char tmp = *str;

		while (str < ptr) {
			*(str++) = *ptr;
			*(ptr--) = tmp;
			tmp = *str;
		}
	}

	/// <summary>
	/// Converts the given ::ullint to a c-string.
	/// @note This writes directly to the given strings.
	/// @attention This doesn't clear the passed string; The data it had before will still be there.
	/// @warning The **str** c-string shall be at least 20 characters in length (excluding null)
	/// </summary>
	/// <param name="num">The number to convert</param>
	/// <param name="str">The c-string to write to</param>
	/// <returns>The number of characters successfully written (excluding null terminator)</returns>
	inline int ullToCString(ullint num, char* const str) noexcept {

		int a = 0;
		while (num > 9) {
			str[a++] = '0' + num % 10;
			num /= 10;
		}
		str[a++] = '0' + num % 10;
		ace::utils::revCString(str, a);

		return a;
	}
}



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


// greater than
/////////////////




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

	[[nodiscard]] inline std::size_t sectorAmount(void) const noexcept {
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

	inline void setLastSector(const ullint val) { this->setSector(this->sectorAmount() - 1, val); }

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

	static void sectorToCString(char* const str, ullint val) noexcept;


	template<class T>
#if defined(_MSC_VER)
	__forceinline
#else
	inline
#endif
	static constexpr char* sectorToString2(char* b, const T i) noexcept; // defined below class

//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////
	template<typename T>
	void copyFromInt(const T num) requires(std::is_integral<T>::value); // defined below class

	void copyFromString(const std::string_view str, const bool check = true);

	template<typename T>
	inline void copyFromFloat(const T flt) requires(std::is_floating_point<T>::value); // defined below class

	

	/// The vector that contains all the number sectors
	std::vector<ullint> m_vecSectors;
	/// The size of the number in jeaiii::digits
	ullint m_ullSize;
	/// The flag whether the number is negative
	bool m_bNegative;

};






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline function definitions (mostly primitive arithmetic-related stuff)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline AEBigint& AEBigint::operator=(const T num) requires(std::is_integral<T>::value) {
	dprintf("Assigning from integral value (digits: %u)", ace::math::lengthOfInt<T>(num));
	this->copyFromInt(num);
	return *this;
}


template<typename T>
void AEBigint::copyFromInt(const T num) requires(std::is_integral<T>::value) {
	if (num == 0) {
		this->clear(true);
		return;
	}

	this->clear(false);
	
	this->m_ullSize = ace::math::lengthOfInt<T>(num);

	if constexpr (std::is_signed<T>::value) { // its signed and may be negative

		if (num < 0) {
			//also its less than allowed sector size, so we can just mash it there
			this->m_vecSectors.emplace_back(ace::math::absval<llint>(num));
			this->m_bNegative = true;
		}
		else {
			this->m_vecSectors.emplace_back(num);
			this->m_bNegative = false;
		}
	}
	else
	{
		if (num > _AEBI_MAX_SECTOR_OPERATION_VALUE) {

			this->m_vecSectors.emplace_back(num % _AEBI_MAX_SECTOR_STORE_P10);
			this->m_vecSectors.emplace_back(num / _AEBI_MAX_SECTOR_STORE_VALUE);
		}
		else {
			this->m_vecSectors.emplace_back(num);
		}
		this->m_bNegative = false;
	}

}

template<typename T>
inline void AEBigint::copyFromFloat(const T flt) requires(std::is_floating_point<T>::value) {
	if (ace::math::absval(flt) < 1.0L) {
		this->clear(true); // welp, I guess you're still 0
		return;
	}

// 	this->clear(false);
	//allocate enough space for the maximum value of the float + negative sign + decimal ".X" + null terminator
	char fltnum[std::numeric_limits<T>::max_exponent10 + 3]{};

	//using precision and then "-1" to try to simulate rounding down
// 	this->copyFromString(
// 		std::string_view(
// 			fltnum,
// 			std::to_chars(fltnum, fltnum + sizeof(fltnum) - 1, flt, std::chars_format::fixed, 1).ptr - 2));
	this->copyFromString(
		std::string_view(
			fltnum,
			std::to_chars(fltnum, fltnum + sizeof(fltnum) - 1, std::trunc(flt), std::chars_format::fixed, 0).ptr), false);
}

template<typename T>
inline AEBigint& AEBigint::operator=(const T flt) requires(std::is_floating_point<T>::value) {
	dprintf("Assigning from floating-point value (digits: %u, negative: %s)", ace::math::lengthOfFloat<T>(flt), ace::utils::boolToString(flt < 0).data());
	this->copyFromFloat(flt);
	return *this;
}



template<typename T>
inline bool AEBigint::operator==(const T num) const noexcept requires(std::is_integral<T>::value) {

	if constexpr (IS_SAME_NOCV(T, ullint)) {
		if (num >= powerOf10Table[19]) { //doesn't fit into 1 sector
			if (this->sectorAmount() != 2 ||
				this->getSector(0) != (num % powerOf10Table[19]) ||
				this->getSector(1) != (num / powerOf10Table[19])) {
				return false;
			}
		}
	}
	if (this->sectorAmount() != 1 || this->getFirstSector() != num) {
		return false;
	}

	return true;
}

template<typename T>
inline bool AEBigint::operator==(const T flt) const requires(std::is_floating_point<T>::value) {

	return (this->isNegative() != (flt < 0) || !ace::math::fequals(flt, std::trunc(flt)) || this->size() != ace::math::lengthOfFloat<T>(flt))
		? false
		: this->operator==(AEBigint(flt));
}


template<class T>
#if defined(_MSC_VER)
__forceinline
#else
inline
#endif
constexpr char* AEBigint::sectorToString2(char* b, const T i) noexcept
{

	using u32 = jeaiii::u32;
	using u64 = jeaiii::u64;

	constexpr auto q = sizeof(T);
	using U = jeaiii::cond<q == 1, unsigned char, jeaiii::cond<q <= sizeof(short), unsigned short, jeaiii::cond<q <= sizeof(u32), u32, u64>>>;

	// convert bool to int before test with unary + to silence warning if T happens to be bool
	const U n = +i < 0 ? *b++ = '-', U(0) - U(i) : U(i);
	const u32 z = n % u32(100000000);
	u64 u = n / u32(100000000);

	const u32 y = u % u32(100000000);
	u /= u32(100000000);
	{
		const auto f0 = u32(10 * (1 << 24) / 1e3 + 1) * u;
		*reinterpret_cast<jeaiii::pair*>(b) = jeaiii::digits.fd[f0 >> 24];
		b -= u < u32(1e3);
		const auto f2 = (f0 & jeaiii::mask24) * 100;
		*reinterpret_cast<jeaiii::pair*>(b + 2) = jeaiii::digits.dd[f2 >> 24];
		b += 4;
	}
	{
		// do 8 jeaiii::digits
		const auto f0 = (u64((1ull << 48ull) / 1e6 + 1) * y >> 16) + 1;
		*reinterpret_cast<jeaiii::pair*>(b) = jeaiii::digits.dd[f0 >> 32];
		const auto f2 = (f0 & jeaiii::mask32) * 100;
		*reinterpret_cast<jeaiii::pair*>(b + 2) = jeaiii::digits.dd[f2 >> 32];
		const auto f4 = (f2 & jeaiii::mask32) * 100;
		*reinterpret_cast<jeaiii::pair*>(b + 4) = jeaiii::digits.dd[f4 >> 32];
		const auto f6 = (f4 & jeaiii::mask32) * 100;
		*reinterpret_cast<jeaiii::pair*>(b + 6) = jeaiii::digits.dd[f6 >> 32];
		b += 8;
	}
	// do 8 jeaiii::digits
	const auto f0 = (u64((1ull << 48ull) / 1e6 + 1) * z >> 16) + 1;
	*reinterpret_cast<jeaiii::pair*>(b) = jeaiii::digits.dd[f0 >> 32];
	const auto f2 = (f0 & jeaiii::mask32) * 100;
	*reinterpret_cast<jeaiii::pair*>(b + 2) = jeaiii::digits.dd[f2 >> 32];
	const auto f4 = (f2 & jeaiii::mask32) * 100;
	*reinterpret_cast<jeaiii::pair*>(b + 4) = jeaiii::digits.dd[f4 >> 32];
	const auto f6 = (f4 & jeaiii::mask32) * 100;
	*reinterpret_cast<jeaiii::pair*>(b + 6) = jeaiii::digits.dd[f6 >> 32];
	return b + 8;
}


#endif // !ENGINE_BIGINT_HPP