#pragma once

#ifndef ENGINE_BIGINT_DEFINITION_HPP
#define ENGINE_BIGINT_DEFINITION_HPP

#include "AEBigint_declaration.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline function definitions (mostly templated primitive arithmetic type-related stuff)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
// copying
//////////////////////////////////
template<typename T>
inline AEBigint& AEBigint::operator=(const T num) requires(std::is_integral<T>::value) {
	dprintf("Assigning from integral value (digits: %u)", ace::math::lengthOfInt<T>(num));
	this->copyFromInt(num);
	
	return *this;
}

template<typename T>
inline void AEBigint::copyFromInt(T num) requires(std::is_integral<T>::value) {
	// small performance optimization :P
	if (num == 0) {
		dprintf("The number is 0, clearing the bigint and setting to 0...");
		this->clear<true>();
		return;
	}
	
	/*
		 * logic, unsigned:
		 * 0) clear the bigint, not set it to 0, and allocate additional space if it's larger than 1 sector (digit10+1)/_AEBI_MAX_SECTOR_STORE_DIGITS
		 * 1) set negativity to false
		 * 2) assign the value depending on size, do a constexpr check (numeric_limits<T>::max() > _AEBI_MAX_SECTOR_STORE_VALUE)
		 * 2.1) if constexpr numeric limits max is less than or equal to _AEBI_MAX_SECTOR_STORE_VALUE, then we can store it in one sector
		 * 2.2) if constexpr numeric limits max is greater than _AEBI_MAX_SECTOR_STORE_VALUE, then if run-time value (num > _AEBI_MAX_SECTOR_STORE_VALUE)
		 * 2.2.1) if value is less than or equal to _AEBI_MAX_SECTOR_STORE_VALUE, then we can store it in one sector (2.1)
		 * 2.2.2) if value is greater than _AEBI_MAX_SECTOR_STORE_VALUE, while loop for num > 0, then emplace_back the num % _AEBI_MAX_SECTOR_STORE_P10, then num /= _AEBI_MAX_SECTOR_STORE_P10;
		 * 3) update the digit count
		 * 
		 * changes if signed, ASSUMING it's not intmax_t (because if it's negative -- it can't just be upcasted and abs'd):
		 * 1) do a constexpr check if type is signed, then set negativity to negativty of num
		 * 2) make a unsigned max variable and assign it to abs(num)
		 * 
		 * if intmax_t -- then check if it's equal to std::numeric_limits<std::intmax_t>::min()
		 * if so, set negativity to true, and absnum to std::numeric_limits<std::intmax_t>::max() + 1
		 **/
	
	// a crutch so that we can use the same code for signed and unsigned types
	std::uintmax_t absnum;

	// small difference for signed and unsigned types
	if constexpr (std::is_signed<T>::value) {
		// check for special case of intmax_t min

		this->m_bNegative = (num < 0);

		if constexpr (std::numeric_limits<T>::min() == std::numeric_limits<std::intmax_t>::min()) {
			// special case for intmax_t min, which is the largest negative number,
			// it's abs is larger than its largest positive number, being max+1
			if (num == std::numeric_limits<std::intmax_t>::min()) {
				absnum = std::numeric_limits<std::intmax_t>::max() + 1;
				dprintf("Special case for intmax_t min, setting absnum to intmax_t maximum+1");
			}
			else {
				absnum = ace::math::absval(num); // abs the number
			}
		}
		else {
			absnum = ace::math::absval(num); // abs the number
		}
		
	}
	else {
		absnum = num;
		this->m_bNegative = false;
	}

	if constexpr (std::numeric_limits<T>::max() > _AEBI_MAX_SECTOR_STORE_VALUE) {
		// possibly 2+ sectors long
		
		if(absnum > _AEBI_MAX_SECTOR_STORE_VALUE) {
			// we need to store it in multiple sectors
			dprintf("The number is larger than a sector, splitting...");
			

			this->clear<false>(absnum / _AEBI_MAX_SECTOR_STORE_P10 + 1);
			
			while (absnum > 0) {
				this->m_vecSectors.emplace_back(absnum % _AEBI_MAX_SECTOR_STORE_P10);
				absnum /= _AEBI_MAX_SECTOR_STORE_P10;
			}
		}
		else {
			// we can store it in one sector
			this->clear<false>();
			this->m_vecSectors.push_back(absnum);
		}
	}	
	else {
		// we can definitely store it in one sector
		this->clear<false>();
		this->m_vecSectors.push_back(absnum);
	}

	
	this->updateDigitCount();
	dprintf("Digit count updated, now: %llu", this->m_ullSize);
}



template<typename T>
inline void AEBigint::copyFromFloat(const T flt) requires(std::is_floating_point<T>::value) {
	if (ace::math::absval(flt) < T(1.0)) {
		this->clear<true>(); // welp, I guess you're still 0
		return;
	}

	// 	this->clear(false);
	//allocate enough space for the maximum value of the float + negative sign + decimal ".X" + null terminator
	char fltnum[std::numeric_limits<T>::max_exponent10 + 3]{};

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


//////////////////////////////////
// Comparison
// AEBigint_construction.cpp
//////////////////////////////////
template<typename T>
inline bool AEBigint::operator==(const T num) const noexcept requires(std::is_integral<T>::value) {


	if constexpr (std::is_signed<T>::value) {
		// check if the sign is different
		if (this->isNegative() != (num < 0)) {
			return false;
		}
		// 2+ sector long
		if constexpr (std::numeric_limits<T>::max() > _AEBI_MAX_SECTOR_STORE_VALUE) {

		}
		else {

		}

	}
	else {

	}



	return true;
}

template<typename T>
inline bool AEBigint::operator==(const T flt) const requires(std::is_floating_point<T>::value) {

	return (this->isNegative() != (flt < 0) || // different negativity
		!ace::math::fequals(flt, std::trunc(flt)) || // if the float has any decimals (imitating native in behaviour)
		this->size() != ace::math::lengthOfFloat<T>(flt)) // if float is larger than this number
		? false
		: this->operator==(AEBigint(flt));
}


template<bool setToZero>
// @fixme: look at all references to this function and fix them
inline void AEBigint::clear(const std::size_t offset){
	this->m_vecSectors.clear();
	this->m_vecSectors.reserve(AEBI_RESERVE_SIZE + offset);

	if constexpr (setToZero) {
		this->m_vecSectors.push_back(0);
		this->m_ullSize = 1;
		this->m_bNegative = false;
	}

}


template<typename T, const bool greaterThan>
inline bool AEBigint::compareInt(const T num) const noexcept requires(std::is_integral<T>::value) {

	// @fixme Rewrite the comparison to support the new type-(and length)-abstracted system

	if (this->isNegative() == (num > 0)) {
		if constexpr (greaterThan) {
			return this->isPositive();
		}
		else {
			return this->isNegative();
		}
	}

	const auto checkSinglePos = [&](void) noexcept {
		if constexpr (greaterThan) {
			if (this->getSectorAmount() == 1 && this->getFirstSector() <= num) {
				return true; // return that we truly failed
			}
		}
		else {
			if (this->getSectorAmount() > 1 || this->getFirstSector() >= num) {
				return true;
			}
		}
		return false;
		};

	if constexpr (std::is_signed<T>::value) {
		const auto checkSingleNeg = [&](void) noexcept {
			if constexpr (greaterThan) {
				if (this->getSectorAmount() > 1 || this->getFirstSector() >= ace::math::absval(num)) {
					return true;
				}
			}
			else {
				if (this->getSectorAmount() == 1 && this->getFirstSector() <= ace::math::absval(num)) {
					return true; // return that we truly failed
				}
			}
			return false;
			};

		if (this->isNegative()) {
			if (checkSingleNeg()) {
				return false;
			}
		}
		else {
			if (checkSinglePos()) {
				return false;
			}
		}
	}
	else {
		if constexpr (std::numeric_limits<T>::max() > _AEBI_MAX_SECTOR_STORE_VALUE) {
			if (num > _AEBI_MAX_SECTOR_STORE_P10) { // 2 sectors wide
				if constexpr (greaterThan) {
					if (this->getSectorAmount() > 2) {
						return true;
					}
					if ((this->getSectorAmount() < 2) ||
						(this->getFirstSector() <= (num % _AEBI_MAX_SECTOR_STORE_P10) &&
							this->getLastSector() <= (num / _AEBI_MAX_SECTOR_STORE_P10)
							)
						) {
						return false;
					}
				}
				else {
					if (this->getSectorAmount() < 2) {
						return true;
					}
					if ((this->getSectorAmount() > 2) ||
						(this->getFirstSector() >= (num % _AEBI_MAX_SECTOR_STORE_P10) &&
							this->getLastSector() >= (num / _AEBI_MAX_SECTOR_STORE_P10)
							)
						) {
						return false;
					}
				}
			}
			else {
				if (checkSinglePos()) {
					return false;
				}
			}
		}
		else {
			if (checkSinglePos()) {
				return false;
			}
		}
	}

	return true;
}

template<const bool greaterThan>
bool AEBigint::compareString(const std::string_view str) const {
	if (!ace::utils::isNum<false>(str)) { //if has decimals -- not equal instantly
		return false;
	}
	const bool neg = (str[0] == '-');

	if (this->isNegative() != neg) {

		if constexpr (greaterThan) {
			return this->isPositive();
		}
		else {
			return this->isNegative();
		}
	}

	const auto checkNum = (this->isNegative())
		? [](const AEBigintDigitIndex num1, const AEBigintDigitIndex num2) noexcept { return num1 > num2; }
	: [](const AEBigintDigitIndex num1, const AEBigintDigitIndex num2) noexcept {return num1 < num2; };

	if constexpr (greaterThan) {
		if (checkNum(this->size(), (str.size() - neg))) {
			return false;
		}
	}
	else {
		if (checkNum((str.size() - neg), this->size())) {
			return false;
		}
	}

	//perform actual number check
	AEBigint a;
	a.copyFromString(str, false);

	if constexpr (greaterThan) {
		return this->operator>(a);
	}
	else {
		return a.operator>(*this);
	}
}


template<typename T, const bool greaterThan>
inline bool AEBigint::compareFloat(const T flt) const requires(std::is_integral<T>::value) {

	if (this->isNegative() != (flt < 0) || this->size() > ace::math::lengthOfFloat(flt)) {
		if constexpr (greaterThan) {
			return this->isPositive();
		}
		else {
			return this->isNegative();
		}
	}

	if constexpr (greaterThan) {
		return this->operator>(AEBigint(flt));
	}
	else {
		return this->operator<(AEBigint(flt));
	}
}

template<class T>
#if defined(_MSC_VER)
__forceinline
#else
inline
#endif
constexpr char* AEBigint::sectorToCString(char* b, const T i) noexcept
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




#endif // !ENGINE_BIGINT_DEFINITION_HPP
