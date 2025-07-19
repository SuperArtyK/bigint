#include "../include/AEBigint.hpp"

/////////////////
// constructors
/////////////////
AEBigint::AEBigint(const AEBigint& bint) : m_vecSectors(bint.m_vecSectors), m_ullSize(bint.m_ullSize), m_bNegative(bint.m_bNegative) {
	dprintf("Constructing AEBigint with another AEBigint (digits: %llu, negative: %s)", bint.size(), ace::utils::boolToString(bint.isNegative()).data());
	this->m_vecSectors.reserve(this->m_vecSectors.size() + AEBI_RESERVE_SIZE);
}

AEBigint::AEBigint(const std::string_view str) {
	dprintf("Constructing AEBigint with a std::string_view (characters: %llu)", ullint(str.size()) );
	if (!this->copyFromString(str)) {
		this->clear(true);
	}
}


AEBigint::~AEBigint() {
	dprintf("Destroying this AEBigint (digits: %llu, negative: %s)", this->size(), ace::utils::boolToString(this->isNegative()).data());
}


//////////////////////////////////
// assignment operators
//////////////////////////////////
AEBigint& AEBigint::operator=(const AEBigint& bint) {
	this->copyFromBigint(bint);
	return *this;
}

AEBigint& AEBigint::operator=(const std::string_view str) {
	dprintf("Assigning from a std::string_view (characters: %llu)", ullint(str.size()));
	if (this->copyFromString(str)) {
		this->clear(true);
	}
	return *this;
}


//////////////////////////////////
// copying
// AEBigint_construction.cpp
//////////////////////////////////

void AEBigint::copyFromBigint(const AEBigint& bint) {
	dprintf("Assigning from another Bigint (digits: %llu, negative: %s)", bint.size(), ace::utils::boolToString(bint.isNegative()).data());

	if (this == &bint || (this->isZero() && bint.isZero())) {
		dprintf("Tried self-assigning or assign to 0-bigint while 0 originally");
		return;
	}


	this->m_vecSectors = bint.m_vecSectors;
	this->m_vecSectors.reserve(bint.m_vecSectors.size() + AEBI_RESERVE_SIZE);
	this->m_ullSize = bint.m_ullSize;
	this->m_bNegative = bint.m_bNegative;
}

bool AEBigint::copyFromString(const std::string_view str, const bool check) {

	if (check && !ace::utils::isNum<false>(str)) {
		return false;
	}

	const char* start = str.data();
	if (start[0] == '0' && str.size() == 1) {
		this->clear(true);
		return true;
	}
	this->clear(false);

	
	if (start[0] == '-') {
		this->m_bNegative = true;
	}
	else
	{
		this->m_bNegative = false;
	}

	this->m_ullSize = str.length() - this->m_bNegative;
	start += str.size();

	{
		const std::size_t alloc = (this->m_ullSize - 1) / _AEBI_MAX_SECTOR_STORE_DIGITS + 1;
		this->m_vecSectors.reserve(alloc + AEBI_RESERVE_SIZE);
		this->m_vecSectors.resize(alloc);
	}

	constexpr auto toUllint = [](const char* const str, const ullint sz) {
		ullint result = 0;
		for (std::size_t i = 0; i < sz; i++) {
			result = result * 10 + str[i] - '0';
		}
		return result;
		};

	std::size_t i = this->m_ullSize;
	std::size_t a = 0;

	for (; i > _AEBI_MAX_SECTOR_STORE_DIGITS; i -= _AEBI_MAX_SECTOR_STORE_DIGITS) {
		this->m_vecSectors[a++] = toUllint(start -= _AEBI_MAX_SECTOR_STORE_DIGITS, _AEBI_MAX_SECTOR_STORE_DIGITS);
	}
	this->m_vecSectors[a] = toUllint(str.data() + this->m_bNegative, i);

	return true;
}