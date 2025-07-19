#include "../include/AEBigint_declaration.hpp"



void AEBigint::rawSelfAdd(const AEBigint& bint) {

	//check if bint is zero
	if (bint.isZero()) {
		return; // skip the unnecessary addition
	}

	//if this is zero
	if (this->isZero()) {
		this->copyFromBigint(bint); //copy the other one
		return;
	}

	const std::size_t len = ace::math::min(this->getSectorAmount(), bint.getSectorAmount());
	const std::size_t len2 = ace::math::max(this->getSectorAmount(), bint.getSectorAmount());
	this->m_vecSectors.reserve(len2 + AEBI_RESERVE_SIZE + 1);
	this->m_vecSectors.resize(len2);

	ullint part1 = 0;
	ullint part2 = 0;
	ullint carry = 0;

	std::size_t i = 0;
	for (; i < len; i++) {
		part1 = (this->getSector(i) % _AEBI_MAX_SECTOR_OPERATION_P10) + (bint.getSector(i) % _AEBI_MAX_SECTOR_OPERATION_P10) + carry;
		part2 = (this->getSector(i) / _AEBI_MAX_SECTOR_OPERATION_P10) + (bint.getSector(i) / _AEBI_MAX_SECTOR_OPERATION_P10) + (part1 / _AEBI_MAX_SECTOR_OPERATION_P10);
		carry = part2 / 10;
		part2 %= 10;
		part1 %= _AEBI_MAX_SECTOR_OPERATION_P10;

		this->m_vecSectors[i] = part1 + part2 * _AEBI_MAX_SECTOR_OPERATION_P10;
	}

	if (this->getSectorAmount() > bint.getSectorAmount()) {
		for (; i < len2 && carry; i++) { // the numbers are already there, so care only for carry
			this->m_vecSectors[i] += carry;
			carry = this->m_vecSectors[i] / _AEBI_MAX_SECTOR_STORE_P10;
			this->m_vecSectors[i] %= _AEBI_MAX_SECTOR_STORE_P10;
		}
	}
	else {
		for (; i < len2; i++) {
			this->m_vecSectors[i] = carry + bint.m_vecSectors[i];
			carry = this->m_vecSectors[i] / _AEBI_MAX_SECTOR_STORE_P10;
			this->m_vecSectors[i] %= _AEBI_MAX_SECTOR_STORE_P10;
		}
	}

	if (carry) { // edge case for "9999999...." number
		this->m_vecSectors.emplace_back(carry);
	}

	this->m_ullSize = (this->m_vecSectors.size() - 1) * _AEBI_MAX_SECTOR_STORE_DIGITS + ace::math::lengthOfInt(this->getLastSector());
}