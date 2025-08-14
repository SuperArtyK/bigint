#include "engine/AEBigint.hpp"



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


	//get lowest and highest length sector amounts
	// lowest is used for general addition between 2 numbers
	// highest is used for left-over carry stuff (and resizing)
	const std::size_t lowestLen = ace::math::min(this->getSectorAmount(), bint.getSectorAmount());
	const std::size_t highestLen = ace::math::max(this->getSectorAmount(), bint.getSectorAmount());
	this->m_vecSectors.reserve(highestLen + AEBI_RESERVE_SIZE + 1);
	this->m_vecSectors.resize(highestLen);

	// local storage for numbers
	// faster access (I think)
	AEBigintSector tempresult = 0;
	AEBigintSector carry = 0;

	std::size_t i = 0;

	for (; i < lowestLen; i++) {

		tempresult = this->getSector(i) + bint.getSector(i) + carry;
		carry = tempresult / _AEBI_MAX_SECTOR_STORE_P10;
		this->m_vecSectors[i] = tempresult % _AEBI_MAX_SECTOR_STORE_P10;
	}

	if (this->getSectorAmount() > bint.getSectorAmount()) {
		for (; i < highestLen && carry; i++) { // the numbers are already there, so care only for carry
			tempresult = this->m_vecSectors[i] + carry;
			carry = tempresult / _AEBI_MAX_SECTOR_STORE_P10;
			this->m_vecSectors[i] = tempresult % _AEBI_MAX_SECTOR_STORE_P10;
		}
	}
	else {
		// numbers aren't there, we need to copy them over
		// care for carry right now
		for (; i < highestLen && carry; i++) {
			tempresult = carry + bint.m_vecSectors[i];
			carry = tempresult / _AEBI_MAX_SECTOR_STORE_P10;
			this->m_vecSectors[i] = tempresult % _AEBI_MAX_SECTOR_STORE_P10;
		}
		// just so we don't continue costly division and modulo, carry is 0
		// (and if i is highestLen -- then we skip it too
		for (; i < highestLen; i++) {
			this->m_vecSectors[i] = bint.m_vecSectors[i];
		}
	}

	// edge case for "9999999...." number
	if (carry) { 
		this->m_vecSectors.emplace_back(carry);
	}

	//update the size
	this->updateDigitCount();
}