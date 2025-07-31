#include "engine/AEBigint.hpp"
#include "engine/jeaiii_to_text.h"

/////////////////
// setters
/////////////////

void AEBigint::setDigit(const AEBigintDigitIndex dig, const ucint val) {

	// get the sector and the position within it
	const std::size_t digSector = dig / _AEBI_MAX_SECTOR_STORE_DIGITS;
	const uint digp10 = dig % _AEBI_MAX_SECTOR_STORE_DIGITS;

	// set the value in the sector if it exists
	if (digSector < this->getSectorAmount()) {
		const AEBigintSector temp = this->m_vecSectors[digSector];
		this->setSector(digSector, (val * powerOf10Table[digp10] +
			temp - temp % powerOf10Table[digp10 + 1] +
			temp % powerOf10Table[digp10]
			)
		);
	}
	else {
		// otherwise, create the sector and set the value
		this->setSector(digSector, val * powerOf10Table[digp10]);
	}
}

void AEBigint::setSector(const std::size_t sector, const AEBigintSector val) {
	// if the sector (0-based) index is larger than the current amount of sectors, resize the vector
	// and get sum more reserve space
	if ((sector + 1) >= this->m_vecSectors.size()) {
		if (sector >= this->m_vecSectors.size()) {
			this->m_vecSectors.reserve(sector + AEBI_RESERVE_SIZE + 1);
			this->m_vecSectors.resize(sector + 1);
			this->m_ullSize = sector * _AEBI_MAX_SECTOR_STORE_DIGITS;
		}
		this->m_ullSize = (this->getSectorAmount()-1)*_AEBI_MAX_SECTOR_STORE_DIGITS + ace::math::lengthOfInt(val);
	}
	this->m_vecSectors[sector] = val;

}



[[nodiscard]] std::string AEBigint::toString() const {
	if (this->isZero()) {
		return "0";
	}
	std::string result(this->size() + this->isNegative(), '0');
	this->toCString(result.data());
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
	std::string_view tmp(buf, jeaiii::to_text_from_integer(buf, bint.getLastSector()) - buf);

	out << tmp;

	tmp = std::string_view(buf, _AEBI_MAX_SECTOR_STORE_DIGITS);

	for (std::size_t i = bint.getSectorAmount() - 1; i-- > 0;) {
		//AEBigint::sectorToCString(buf, bint.getSector(i));
		jeaiii::to_text_from_integer(buf, bint.getSector(i));
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

	dataptr = jeaiii::to_text_from_integer(dataptr, this->getLastSector());

	for (std::size_t i = this->getSectorAmount() - 1; i-- > 0;) {

		//AEBigint::sectorToCString(dataptr, this->getSector(i));
		jeaiii::to_text_from_integer(dataptr, this->getSector(i));
		dataptr += _AEBI_MAX_SECTOR_STORE_DIGITS;
	}

}

/////////////////
// miscellania
/////////////////
