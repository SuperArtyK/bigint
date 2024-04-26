#include "../include/AEBigint.hpp"




std::string AEBigint::toString() const {

	std::string result;
	result.reserve(this->m_ullSize + (this->m_bNegative) ? 1 : 0);

	if (this->m_bNegative) {
		result.push_back('-');
	}

	char buf[20]{};
	snprintf(buf, sizeof(buf), "%llu", this->m_vecSectors[this->m_vecSectors.size()-1]);

	result.append(buf);

	for (int i = this->m_vecSectors.size() - 2; i >= 0; i--) {
		snprintf(buf, sizeof(buf), "%0.19llu", this->m_vecSectors[i]);
		result.append(buf);
	}



	return result;

}

std::ostream& operator<<(std::ostream& out, const AEBigint& bint) {
	out << bint.toString();

	return out;
}