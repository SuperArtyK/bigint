#include "../include/AEBigint.hpp"

[[nodiscard]] std::string AEBigint::toString() const {

	if (this->isZero()) {
		return "0"; // a quick shortcut and performance gain :)
	}

	std::string result;

	
	

	if (this->m_bNegative) {
		result.reserve(this->m_ullSize + 1); // reserve space for the '-'
		result.push_back('-');
	}
	else {
		result.reserve(this->m_ullSize);
	}

	char buf[20]{};
	snprintf(buf, sizeof(buf), "%llu", this->m_vecSectors[this->m_vecSectors.size() - 1]);

	result.append(buf);

	for (int i = this->m_vecSectors.size() - 2; i >= 0; i--) {
		snprintf(buf, sizeof(buf), "%0.19llu", this->m_vecSectors[i]);
		result.append(buf);
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
