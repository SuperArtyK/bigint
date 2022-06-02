#include "bigint.hpp"

//////////////////
//member functions
//////////////////

//-----------
//other stuff
//-----------

//TODO:add the 2x compression method
void Bigint::setDigit(const ucint val, const ullint pos, const bool force){
	if(val <= 9){
		dprintf("Value is acceptable: %d", (int)val);
		
		if(pos >= m_iNumLength){
			if(val == 0 && !force && m_iNumLength > 1){
				dprintf("Cannot set a new digit to 0, only 1-9");
				dprintf("Returning...");
				return;
			}
			
			//implement compression part right here
			m_vNumList.resize(pos+1);
			m_iNumLength = pos+1;
			dprintf("Resized number: length %llu; size of the vector is %llu;", m_iNumLength, (ullint)m_vNumList.size());
			
		}
		//and right here too
		m_vNumList[pos] = val;
		//dprintf("%d", (int))
	}
	else{
		dprintf("Value is outside of the dec digit range! Check your code!: %d", (int)val);
	}
}

std::string Bigint::toString() const {
	//dprintf("Size of the number and vector: %llu, %llu", m_iNumLength, (ullint)m_vNumList.size());
	std::string temp;
	temp.reserve(this->m_iNumLength);
	if(this->m_bNegative){
		temp.push_back('-');
	}
	for(ullint i = 0; i < this->m_iNumLength; i++){
		temp.push_back(INT_TO_CHAR(this->getDigitChar(this->m_iNumLength-i-1)));
	}
	return temp;
}

void Bigint::clear(){
	this->m_vNumList.clear();
	this->m_vNumList.reserve(BIGINT_PREALLOC_SIZE);
	this->m_vNumList.push_back(0);
	this->m_iNumLength = 1;
	this->m_bNegative = false;
}





//////////////////
//friend functions
//////////////////
std::ostream& operator<<(std::ostream& out, const Bigint& bint){
	dprintf("Outputting through ostream");
	if(bint.isNegative()){
		out<<'-';
	}
	for(ullint i = 0; i < bint.m_iNumLength; i++){
		out<<INT_TO_CHAR(bint.getDigitChar(bint.m_iNumLength-i-1));
	}
	return out;
}
