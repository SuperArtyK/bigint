#include "../include/AEBigint.hpp"

/////////////////
// constructors
/////////////////
AEBigint::AEBigint() {
	dprintf("Constructing an empty bigint");
	this->clear(true);
}

AEBigint::AEBigint(const AEBigint& bint) : m_vecSectors(bint.m_vecSectors), m_ullSize(bint.m_ullSize), m_bNegative(bint.m_bNegative) {
	dprintf("Constructing AEBigint with another AEBigint");
	m_vecSectors.reserve(AEBI_RESERVE_SIZE);
}

AEBigint::AEBigint(const llint num) {
	dprintf("Constructing with a llint");
	this->copyFromInt(num);
}

AEBigint::AEBigint(const ullint num) {
	dprintf("Constructing with a ullint");
	this->copyFromInt(num);
}






//////////////////////////////////
// assignment operators
//////////////////////////////////
AEBigint& AEBigint::operator=(const AEBigint& bint) {
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


void AEBigint::clear(const bool setToZero) {
	this->m_vecSectors.clear();
	this->m_vecSectors.reserve(AEBI_RESERVE_SIZE);

	if (setToZero) {
		this->m_vecSectors.push_back(0);
		this->m_ullSize = 1;
		this->m_bNegative = false;
	}

}



//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////
void AEBigint::copyFromInt(const ullint num) {
	this->clear(false);
	this->m_bNegative = false;

	if (num > _AEBI_MAX_SECTOR_OPERATION_VALUE) {

		this->m_vecSectors.emplace_back(num % _AEBI_MAX_SECTOR_STORE_P10);
		this->m_vecSectors.emplace_back(num / _AEBI_MAX_SECTOR_STORE_VALUE);
	}
	else {
		this->m_vecSectors[0] = num;
	}
	this->m_ullSize = ace::math::lengthOfInt<ullint>(num);
}

void AEBigint::copyFromInt(const llint num) {
	this->clear(false);

	if (num > 0) {
		this->m_vecSectors.emplace_back(num);
		this->m_bNegative = false;
	}
	else {
		this->m_vecSectors.emplace_back(ace::math::absval<llint>(num));
		this->m_bNegative = true;
	}
	this->m_ullSize = ace::math::lengthOfInt<llint>(num);
}
