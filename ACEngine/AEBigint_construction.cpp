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


//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////