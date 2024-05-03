#include "../include/AEBigint.hpp"
#include "../include/jeaiii_to_text.h"

/////////////////
// setters
/////////////////

void AEBigint::setDigit(const ullint dig, const ucint val) {
	const std::size_t digSector = dig / _AEBI_MAX_SECTOR_STORE_DIGITS;
	const int digp10 = dig % _AEBI_MAX_SECTOR_STORE_DIGITS;

	if (digSector < this->getSectorAmount()) {
		const ullint temp = this->m_vecSectors[digSector];
		this->setSector(digSector, (val * powerOf10Table[digp10] +
			temp - temp % powerOf10Table[digp10 + 1] +
			temp % powerOf10Table[digp10]
			)
		);
	}
	else {
		this->setSector(digSector, val * powerOf10Table[digp10]);
	}
}

void AEBigint::setSector(const std::size_t sector, const ullint val) {
	if ((sector + 1) >= this->m_vecSectors.size()) {
		if (sector >= this->m_vecSectors.size()) {
			this->m_vecSectors.reserve(sector + AEBI_RESERVE_SIZE + 1);
			this->m_vecSectors.resize(sector + 1);
			this->m_ullSize = sector * _AEBI_MAX_SECTOR_STORE_DIGITS;
		}
		this->m_ullSize += ace::math::lengthOfInt(val);
	}
	this->m_vecSectors[sector] = val;

}



[[nodiscard]] std::string AEBigint::toString() const {
	if (this->isZero()) {
		return "0";
	}
	std::string result(this->getSize() + this->isNegative(), '0');
	this->toCString2(result.data());
	return result;
}

std::ostream& operator<<(std::ostream& out, const AEBigint& bint) {
	if (bint.isZero()) {
		return (out << '0');
	}

	char buf[_AEBI_MAX_SECTOR_STORE_DIGITS + 1]{};
	
	if (bint.isNegative()) {
		out << '-';
	}

	std::string_view tmp(buf, ace::utils::ullToCString(bint.getLastSector(), buf));

	out << tmp;

	tmp = std::string_view(buf, _AEBI_MAX_SECTOR_STORE_DIGITS);

	for (std::size_t i = bint.getSectorAmount() - 1; i-- > 0;) {
		AEBigint::sectorToString(buf, bint.getSector(i));
		out << tmp;
	}
	
	return out;
}

void AEBigint::toCString(char* dataptr) const noexcept {
	if (!dataptr) {
		return;
	}

	if (this->isZero()) {
		std::memcpy(dataptr, "0", 2);
		return;
	}

	if (this->isNegative()) {
		*dataptr++ = '-';
	}

	dataptr += ace::utils::ullToCString(this->getLastSector(), dataptr);

	for (std::size_t i = this->getSectorAmount() - 1; i-- > 0;) {

		this->sectorToString(dataptr, this->getSector(i));
		dataptr += _AEBI_MAX_SECTOR_STORE_DIGITS;
	}

	*dataptr = AENULL; //null-terminate it, at least
}

void AEBigint::toCString2(char* dataptr) const noexcept {
	if (!dataptr) {
		return;
	}

	if (this->isZero()) {
		std::memcpy(dataptr, "0", 2);
		return;
	}

	if (this->isNegative()) {
		*dataptr++ = '-';
	}

	dataptr += ace::utils::ullToCString(this->getLastSector(), dataptr);
	//dataptr = jeaiii::to_text_from_integer(dataptr, this->getLastSector());

	char str[_AEBI_MAX_SECTOR_STORE_DIGITS + 1]{};

	int sz = 0;

	for (std::size_t i = this->getSectorAmount() - 1; i-- > 0;) {

		// HUUGE thanks to jeaiii for making this thing!
		// It's literally 100% faster than the previous "sectorToString" method
		// https://github.com/jeaiii/itoa
		
		//jeaiii::to_text_from_integer(dataptr, this->getSector(i), 19);
		//this->sectorToString(dataptr, this->getSector(i));
		
		sz = jeaiii::to_text_from_integer(str, this->getSector(i)) - str;

		std::memcpy(dataptr + (_AEBI_MAX_SECTOR_STORE_DIGITS - sz ) , str, sz);

		dataptr += _AEBI_MAX_SECTOR_STORE_DIGITS;
	}

	*dataptr = AENULL; //null-terminate it, at least
}

void AEBigint::sectorToString(char* const str, ullint val) noexcept {
	std::memset(str, '0', 19);

	if (val == 0) {
		return;
	}

	int i = 18;

	while (val > 9) {
		str[i--] = '0' + val % 10;
		val /= 10;
	}
	str[i] = '0' + val % 10;

}


/////////////////
// miscellania
/////////////////
void AEBigint::clear(const bool setToZero) {
	this->m_vecSectors.clear();
	this->m_vecSectors.reserve(AEBI_RESERVE_SIZE);

	if (setToZero) {
		this->m_vecSectors.push_back(0);
		this->m_ullSize = 1;
		this->m_bNegative = false;
	}

}
