#include "../include/AEBigInt.hpp"


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

	if (!ace::utils::isNum<false>(str)) {
		return false;
	}

	AEBigint b;
	b.copyFromString(str, false);
	return this->operator==(b);
}