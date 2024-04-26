#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP

#include "include/AEMath.hpp"
#include "include/AEUtilsMacros.hpp"
#include "include/AEUtils.hpp"
#include "include/AETypedefs.hpp"
#include <array>

constexpr ullint powerOf10Table[20] {
	1,                       powerOf10Table[0] * 10,  powerOf10Table[1] * 10,  powerOf10Table[2] * 10,  powerOf10Table[3] * 10,   //power 0-4
	powerOf10Table[4] * 10,  powerOf10Table[5] * 10,  powerOf10Table[6] * 10,  powerOf10Table[7] * 10,  powerOf10Table[8] * 10,   //power 5-9
	powerOf10Table[9] * 10,  powerOf10Table[10] * 10, powerOf10Table[11] * 10, powerOf10Table[12] * 10, powerOf10Table[13] * 10,  //power 10-14
	powerOf10Table[14] * 10, powerOf10Table[15] * 10, powerOf10Table[16] * 10, powerOf10Table[17] * 10, powerOf10Table[18] * 10,  //power 11-19
};






/// The largest representable power of 10 per number sector
#define _AEBI_MAX_SECTOR_STORE_P10 (powerOf10Table[19])

/// The largest representable value per number sector
/// @note Corresponds to the 10^19 - 1, the largest value of 9999.... that can fit into uint64
#define _AEBI_MAX_SECTOR_STORE_VALUE (_AEBI_MAX_SECTOR_STORE_P10 - 1)

/// The power of 10 of the largest value per sector to operate on.
#define _AEBI_MAX_SECTOR_OPERATION_P10 (powerOf10Table[18])

/// The largest value to perform operations on per sector.
#define _AEBI_MAX_SECTOR_OPERATION_VALUE (_AEBI_MAX_SECTOR_OPERATION_P10 - 1)

/// The default vector of sectors reserve size.
/// Default value is 32 sectors, total of 608 digits
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
	AEBigint();

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
		dprintf("Constructing with a integral value");
		this->copyFromInt<T>(num);
	}

	~AEBigint() {
		dprintf("Destroying this bigint");
	}

//////////////////////////////////
// assignment operators
//////////////////////////////////
	AEBigint& operator=(const AEBigint& bint);

	template<typename T>
	inline AEBigint& operator=(const T num) requires(std::is_integral<T>::value) {
		if (!this->isZero() && num != 0) {
			this->copyFromInt(num);
		}

		return *this;
	}



/////////////////
// getters
/////////////////
	[[nodiscard]] inline bool isZero() const noexcept {
		return this->m_ullSize == 1 && this->m_vecSectors[0] == 0;
	}

	[[nodiscard]] inline ullint getSize() const noexcept {
		return this->m_ullSize;
	}

	[[nodiscard]] inline bool isNegative() const noexcept {
		return this->m_bNegative;
	}

	[[nodiscard]] inline bool isPositive() const noexcept {
		return !this->m_bNegative;
	}

//////////////////////////////////
// miscellanea
// AEBigint_miscellanea.cpp
//////////////////////////////////
	[[nodiscard]] inline AEBigint operator-() const {
		AEBigint tmp = *this;
		tmp.m_bNegative = !tmp.m_bNegative;
		return tmp;
	}

	inline AEBigint& setNegativity(const bool negative) noexcept {
		this->m_bNegative = negative;
		return *this;
	}

	[[nodiscard]] inline static AEBigint abs(const AEBigint& bint) {
		if (bint.isNegative()) { 
			return -bint; 
		}
		return bint;
	}

	void clear(const bool setToZero = true);


//////////////////////////////////
// conversions
// AEBigint_miscellanea.cpp
//////////////////////////////////
	[[nodiscard]] std::string toString() const;

	[[nodiscard]] inline operator bool() const noexcept { return !this->isZero(); }

	friend inline std::ostream& operator<<(std::ostream& out, const AEBigint& bint) {
		return (out << bint.toString());
	}


private:

//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////
	template<typename T>
	void copyFromInt(const T num) requires(std::is_integral<T>::value); // defined below class



	/// The vector that contains all the number sectors
	std::vector<ullint> m_vecSectors;
	/// The size of the number in digits
	ullint m_ullSize;
	/// The flag whether the number is negative
	bool m_bNegative;

};

template<typename T>
void AEBigint::copyFromInt(const T num) requires(std::is_integral<T>::value) {
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


#endif // !ENGINE_BIGINT_HPP