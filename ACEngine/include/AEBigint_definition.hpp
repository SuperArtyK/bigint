#pragma once

#ifndef ENGINE_BIGINT_DEFINITION_HPP
#define ENGINE_BIGINT_DEFINITION_HPP

#include "AEBigint_declaration.hpp"


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
inline void AEBigint::copyFromInt(const T num) requires(std::is_integral<T>::value) {
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

	if constexpr (std::is_signed<T>::value) {
		if (this->isNegative() != (num < 0)) {
			return false;
		}
		if (this->getSectorAmount() != 1 || this->getFirstSector() != ace::math::absval(num)) {
			return false;
		}
	}
	else {
		if (this->isNegative()) { // no negatives allowed
			return false;
		}

		if constexpr (IS_SAME_NOCV(T, ullint)) {
			if (num >= powerOf10Table[19]) { //doesn't fit into 1 sector
				if (this->getSectorAmount() != 2 ||
					this->getSector(0) != (num % powerOf10Table[19]) ||
					this->getSector(1) != (num / powerOf10Table[19])) {
					return false;
				}
			}
			else { // sadly I have to repeat the code, or I'll have an unnecessary branch again
				if (this->getSectorAmount() != 1 || this->getFirstSector() != num) {
					return false;
				}
			}
		}
		else {
			if (this->getSectorAmount() != 1 || this->getFirstSector() != num) {
				return false;
			}
		}
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

template<typename T, const bool greaterThan>
inline bool AEBigint::compareInt(const T num) const noexcept requires(std::is_integral<T>::value) {

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
		if constexpr (IS_SAME_NOCV(T, ullint)) {
			if (num > powerOf10Table[19]) { // 2 sectors wide
				if constexpr (greaterThan) {
					if (this->getSectorAmount() > 2) {
						return true;
					}
					if ((this->getSectorAmount() < 2) ||
						(this->getFirstSector() <= (num % powerOf10Table[19]) &&
							this->getLastSector() <= (num / powerOf10Table[19])
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
						(this->getFirstSector() >= (num % powerOf10Table[19]) &&
							this->getLastSector() >= (num / powerOf10Table[19])
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
	if (!ace::utils::isNum<false>(str)) {
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
		? [](const ullint num1, const ullint num2) noexcept { return num1 > num2; }
	: [](const ullint num1, const ullint num2) noexcept {return num1 < num2; };

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




#endif // !ENGINE_BIGINT_DEFINITION_HPP
