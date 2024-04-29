#include "../include/AEBigint.hpp"


/////////////////
// setters
/////////////////
void AEBigint::setDigit(const ullint dig, const ucint val) {
	const std::size_t digSector = dig / _AEBI_MAX_SECTOR_STORE_DIGITS;
	const int digp10 = dig % _AEBI_MAX_SECTOR_STORE_DIGITS;
	if (dig >= this->m_ullSize) {
		this->m_ullSize = dig+1;
		this->m_vecSectors.reserve(digSector + AEBI_RESERVE_SIZE + 1);
		this->m_vecSectors.resize(digSector + 1);
	}
	this->m_vecSectors[digSector] =
		this->m_vecSectors[digSector] - this->m_vecSectors[digSector] % powerOf10Table[digp10 + 1] +
		this->m_vecSectors[digSector] % powerOf10Table[digp10] +
		val * powerOf10Table[digp10];

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

	const ucint tmp = ace::utils::ullToCString(this->m_vecSectors[this->m_vecSectors.size() - 1], buf);

	std::memcpy(dataptr, buf, tmp);

	dataptr += tmp;

	for (std::size_t i = this->m_vecSectors.size() - 1; i > 0; i--) {

		std::memcpy(dataptr, this->sectorToString2(buf, this->m_vecSectors[i - 1]), _AEBI_MAX_SECTOR_STORE_DIGITS);
		dataptr += _AEBI_MAX_SECTOR_STORE_DIGITS;
	}

	return result;
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
