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
/// And all of the operations are done on the sectors, which drastically improves performance on normal
/// operations like addition, but reduces performance on per-digit operations.
/// </summary>
class AEBigint {

public:

/////////////////
// constructors
/////////////////
	AEBigint() : m_vecSectors(1), m_ullSize(0), m_bNegative(false) {
		dprintf("Constructing an empty bigint");
		m_vecSectors.reserve(AEBI_RESERVE_SIZE);
	}

	AEBigint(const AEBigint& bint) : m_vecSectors(bint.m_vecSectors), m_ullSize(bint.m_ullSize), m_bNegative(bint.m_bNegative) {
		dprintf("Constructing AEBigint with another AEBigint");
		m_vecSectors.reserve(AEBI_RESERVE_SIZE);
	}

	template <typename T>
	AEBigint(const T num) requires(std::is_integral<T>::value) : 
		m_vecSectors(1), m_ullSize(0), m_bNegative(false) {
		dprintf("Constructing with an integral type");
		this->copyFromInt<T>(num);

	}



//////////////////////////////////
// assignment operators
//////////////////////////////////
	AEBigint& operator=(const AEBigint& bint) {
		dprintf("Assigning from another Bigint");

		if (this == &bint || (this->m_ullSize == bint.m_ullSize && this->isZero())) {
			dprintf("Tried self-assigning or assign to 0-bigint while 0 originally");
			return *this;
		}
		
		this->m_vecSectors = bint.m_vecSectors;
		this->m_vecSectors.reserve(AEBI_RESERVE_SIZE);
		this->m_ullSize = bint.m_ullSize;
		this->m_bNegative = bint.m_bNegative;
		return *this;
	}




/////////////////
// getters
/////////////////
	[[nodiscard]] inline bool isZero() const noexcept {
		return this->m_ullSize == 0;
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

/////////////////
// miscellania
/////////////////

	inline AEBigint operator-() const {
		AEBigint tmp = *this;
		tmp.m_bNegative = !tmp.m_bNegative;
		return tmp;
	}

/////////////////
// conversions
/////////////////

	std::string toString() const;

	friend std::ostream& operator<<(std::ostream& out, const AEBigint& bint);

	template<typename T>
	void copyFromInt(const T num) noexcept requires(std::is_integral<T>::value);


private:

	static constexpr std::array<ullint, 2> intToSectors(const ullint num) noexcept {
		return std::array<ullint, 2>{num % (_AEBI_MAX_SECTOR_STORE_P10), num / _AEBI_MAX_SECTOR_STORE_P10 };
	}

	/// The vector that contains all the number sectors
	std::vector<ullint> m_vecSectors;
	/// The size of the number in digits
	ullint m_ullSize;
	/// The flag whether the number is negative
	bool m_bNegative;

};

template<typename T>
void AEBigint::copyFromInt(const T num) noexcept requires(std::is_integral<T>::value) {

	this->m_vecSectors.resize(1);
	m_vecSectors.reserve(AEBI_RESERVE_SIZE);

	if constexpr (std::is_unsigned<T>::value) {
		if (num > _AEBI_MAX_SECTOR_OPERATION_VALUE) {
			this->m_vecSectors[0] = num % _AEBI_MAX_SECTOR_STORE_P10;
			this->m_vecSectors.emplace_back(num / _AEBI_MAX_SECTOR_STORE_VALUE);
		}
		else {
			this->m_vecSectors[0] = num;
		}
	}
	else {
		if (num > 0) {
			this->m_vecSectors[0] = (num);
		}
		else {
			this->m_vecSectors[0] = (ace::math::absval<T>(num));
			this->m_bNegative = true;
		}
	}
	this->m_ullSize = ace::math::lengthOfInt<T>(num);
}






#endif // !ENGINE_BIGINT_HPP