#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP

#include "include/AEMath.hpp"
#include "include/AEUtilsMacros.hpp"
#include "include/AEUtils.hpp"
#include "include/AETypedefs.hpp"

constexpr ullint powerOf10Table[20] {
	1,                  powerOf10Table[0] * 10,  powerOf10Table[1] * 10,  powerOf10Table[2] * 10,  powerOf10Table[3] * 10,        //power 0-4
	powerOf10Table[4] * 10,  powerOf10Table[5] * 10,  powerOf10Table[6] * 10,  powerOf10Table[7] * 10,  powerOf10Table[8] * 10,   //power 5-9
	powerOf10Table[9] * 10,  powerOf10Table[10] * 10, powerOf10Table[11] * 10, powerOf10Table[12] * 10, powerOf10Table[13] * 10,  //power 10-14
	powerOf10Table[14] * 10, powerOf10Table[15] * 10, powerOf10Table[16] * 10, powerOf10Table[17] * 10, powerOf10Table[18] * 10,  //power 11-19
};




/// The largest representable value per number sector
#define _AEBI_MAX_SECTOR_VALUE (powerOf10Table[19] - 1)
#define _AEBI_RESERVE_SIZE 32





class AEBigInt {

public:

	AEBigInt() : m_vecSectors({}), m_ullSize(0), m_bNegative(false) {

	}






	[[nodiscard]] bool isZero() const {
		return this->m_ullSize == 0;
	}

private:

	std::vector<ullint> m_vecSectors;
	ullint m_ullSize;
	bool m_bNegative;

};

#endif // !ENGINE_BIGINT_HPP