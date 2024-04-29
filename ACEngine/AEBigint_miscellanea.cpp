#include "../include/AEBigint.hpp"


/////////////////
// setters
/////////////////

void AEBigint::setDigit(const ullint dig, const ucint val) {
	const std::size_t digSector = dig / _AEBI_MAX_SECTOR_STORE_DIGITS;
	const int digp10 = dig % _AEBI_MAX_SECTOR_STORE_DIGITS;

	if (digSector < this->getSectorAmount()) {
		const ullint temp = this->m_vecSectors[digSector];
		this->setSectorValue(digSector, (val * powerOf10Table[digp10] +
			temp - temp % powerOf10Table[digp10 + 1] +
			temp % powerOf10Table[digp10]
			)
		);
	}
	else {
		this->setSectorValue(digSector, val * powerOf10Table[digp10]);
	}

	

	
}

void AEBigint::setSectorValue(const std::size_t sector, const ullint val) {
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
		return "0"; // a quick shortcut and performance gain :)
	}

	std::string result;
	char buf[_AEBI_MAX_SECTOR_STORE_DIGITS + 1]{};
	char* dataptr;

	if (this->m_bNegative) {
		result.resize(this->m_ullSize + 1); // reserve space for the '-'
		result[0] = ('-');
		dataptr = result.data() + 1;
	}
	else {
		result.resize(this->m_ullSize);
		dataptr = result.data();
	}

	// It's okay that our c-string is 1 less character in length
	// We won't get numbers larger than 19 digits anyway
	// Otherwise...well definitely have gone wrong, if we're larger than _AEBI_MAX_SECTOR_STORE_DIGITS
	const ucint tmp = ace::utils::ullToCString(this->m_vecSectors[this->m_vecSectors.size() - 1], buf);

	std::memcpy(dataptr, buf, tmp);

	dataptr += tmp;

	for (std::size_t i = this->m_vecSectors.size() - 1; i > 0; i--) {

		std::memcpy(dataptr, this->sectorToString(buf, this->m_vecSectors[i - 1]), _AEBI_MAX_SECTOR_STORE_DIGITS);
		dataptr += _AEBI_MAX_SECTOR_STORE_DIGITS;
	}

	return result;
}

std::ostream& operator<<(std::ostream& out, const AEBigint& bint) {
	return (out << bint.toString());
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
