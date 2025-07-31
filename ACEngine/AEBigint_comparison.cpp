#include "engine/AEBigint.hpp"

// equality
/////////////////

bool AEBigint::operator==(const AEBigint& bint) const noexcept {

	if (this->isNegative() != bint.isNegative() ||
		this->size() != bint.size()) {

		return false;
	}

	for (std::size_t i = this->m_vecSectors.size(); i-- > 0;) {
		if (this->getSector(i) != bint.getSector(i)) {
			return false;
		}
	}


	return true;
}

bool AEBigint::operator==(const std::string_view str) const {
	if (!ace::utils::isNum<false>(str) || 
		this->isNegative() != (str[0] == '-') || 
		this->size() != (str.size() - (str[0] == '-') )  ) {

		return false;
	}

	AEBigint b;
	b.copyFromString(str, false);
	return this->operator==(b);
}


// inequality
/////////////////

bool AEBigint::operator>(const AEBigint& bint) const noexcept {
	const bool neg = this->isNegative();

	if (neg != bint.isNegative()) {
		return !neg; // let the most positive win)
	}
	
	const auto checkNum = (neg)
		? [](const AEBigintSector num1, const AEBigintSector num2) noexcept { return num1 > num2; }
		: [](const AEBigintSector num1, const AEBigintSector num2) noexcept { return num1 < num2; };

	if (checkNum(this->size(), bint.size())) {
		return false;
	}

	bool diff = false;

	for (std::size_t i = this->getSectorAmount(); i-- > 0;) {
		if (checkNum(this->getSector(i), bint.getSector(i))) {
			return false;
		}
		if (this->getSector(i) != bint.getSector(i)) {
			diff = true;
		}

	}

	// mechanism against numbers being exactly the same
	// if is different -- true, if not -- false
	return diff; 
}


