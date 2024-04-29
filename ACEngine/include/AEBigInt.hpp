#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP

#include "include/AEMath.hpp"
#include "include/AETypedefs.hpp"
#include "include/AEUtils.hpp"
#include "include/AEUtilsMacros.hpp"
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

/// The length in digits of the largest representable value per number sector
constexpr int _AEBI_MAX_SECTOR_STORE_DIGITS = ace::math::lengthOfInt(_AEBI_MAX_SECTOR_STORE_VALUE);

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

	[[nodiscard]] inline bool isNegative() const noexcept {
		return this->m_bNegative;
	}

	[[nodiscard]] inline bool isPositive() const noexcept {
		return !this->m_bNegative;
	}

	[[nodiscard]] inline ullint getSize() const noexcept {
		return this->m_ullSize;
	}

	[[nodiscard]] inline std::size_t getSectorAmount() const noexcept {
		return this->m_vecSectors.size();
	}

	[[nodiscard]] inline std::size_t getMemoryUsage() const noexcept {
		return sizeof(AEBigint) + this->m_vecSectors.capacity() * sizeof(ullint);
	}

	[[nodiscard]] inline ucint getDigitChar(const ullint dig) const noexcept {
		if (dig == 0) {
			return this->m_vecSectors[0] % 10;
		}

		return (this->m_vecSectors[dig / _AEBI_MAX_SECTOR_STORE_DIGITS] / powerOf10Table[(dig % _AEBI_MAX_SECTOR_STORE_DIGITS)]) % 10;
	}

	[[nodiscard]] inline int getDigit(const ullint dig) const noexcept {
		return int(this->getDigitChar(dig));
	}


/////////////////
// setters
/////////////////
	inline AEBigint& setNegativity(const bool negative) noexcept {
		this->m_bNegative = negative;
		return *this;
	}

	void setDigit(const ullint dig, const ucint val);


//////////////////////////////////
// miscellanea
// AEBigint_miscellanea.cpp
//////////////////////////////////
	[[nodiscard]] inline AEBigint operator-() const {
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
	[[nodiscard]] std::string toString() const;

	[[nodiscard]] inline operator bool() const noexcept { return !this->isZero(); }

	friend inline std::ostream& operator<<(std::ostream& out, const AEBigint& bint) {
		return (out << bint.toString());
	}

	std::string toString2() const {

		if (this->isZero()) {
			return "0"; // a quick shortcut and performance gain :)
		}

		std::string result;
		char buf[20]{};
		char* dataptr;

		if (this->m_bNegative) {
			result.resize(this->m_ullSize + 1); // reserve space for the '-'
			result[0] = ('-');
			dataptr = result.data()+1;
		}
		else {
			result.resize(this->m_ullSize);
			dataptr = result.data();
		}

		
		//snprintf(buf, sizeof(buf), "%llu", this->m_vecSectors[this->m_vecSectors.size() - 1]);

		ucint tmp = snprintf(buf, sizeof(buf), "%llu", this->m_vecSectors[this->m_vecSectors.size() - 1]);
		std::memcpy(dataptr, buf, tmp);

		dataptr += tmp;

		for (std::size_t i = this->m_vecSectors.size() - 1; i > 0; i--) {

			std::memcpy(dataptr, this->sectorToString2(buf, this->m_vecSectors[i - 1]), 19);
			dataptr += 19;


			//result.append(this->sectorToString2(buf, this->m_vecSectors[i - 1]));
		}

		return result;
	}


	[[nodiscard]] static constexpr const char* sectorToString2(char* str, ullint val) noexcept {

		switch (val) {

			case 0:
				return "0000000000000000000";
				break;

			case 1:
				return "0000000000000000001";
				break;

			case 2:
				return "0000000000000000002";
				break;

			case 3:
				return "0000000000000000003";
				break;

			case 4:
				return "0000000000000000004";
				break;

			case 5:
				return "0000000000000000005";
				break;

			case 6:
				return "0000000000000000006";
				break;

			case 7:
				return "0000000000000000007";
				break;

			case 8:
				return "0000000000000000008";
				break;

			case 9:
				return "0000000000000000009";
				break;

			default:
				std::memset(str, '0', 19);

				int i = 18;

				while (val > 9) {
					str[i--] = '0' + val % 10;
					val /= 10;
				}
				str[i] = '0' + val % 10;

				//snprintf(str, 20, "%0.19llu", val);

				return str;
				break;

		}


		
	}


	
private:

	[[nodiscard]] const char* sectorToString(char* str, ullint val) const noexcept {

		if (val == 0) {
			return "0000000000000000000";
		}

		std::memset(str, '0', 19);

		int i = 18;

		while (val > 9) {
			str[i--] = '0' + val % 10;
			val /= 10;
		}
		str[i] = '0' + val % 10;

		//snprintf(str, 20, "%0.19llu", val);

		return str;
	}

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