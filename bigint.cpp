#include "bigint.hpp"

//////////////////
//member functions
//////////////////


//constructors

//empty constructor
//makes Bigint with value 0;
Bigint::Bigint() {
	dprintf("Constructing an empty bigint");
	this->clear();
}
//constructs Bigint with value from another Bigint
Bigint::Bigint(const Bigint& bint) : Bigint() {
	dprintf("Constructing Bigint with another Bigint");
	this->copyFromBigint(bint);
}
//constructs Bigint with value from std::string
Bigint::Bigint(const std::string& strnum) : Bigint() {
	dprintf("Constructing Bigint with a std::string");
	this->copyFromString(strnum);
}
//constructs Bigint with value from c-string
Bigint::Bigint(const char* strnum) {
	dprintf("Constructing with a c-string");
	this->copyFromCString(strnum);
}
//constructs Bigint with value from long long int(llint)
Bigint::Bigint(const llint num) {
	dprintf("Constructing with a llint");
	this->copyFromInt(num);
}
//constructs Bigint with value from int
Bigint::Bigint(const int num) {
	dprintf("Constructing with a llint");
	this->copyFromInt(num);
}


//assignments
Bigint& Bigint::operator=(const Bigint& bint){
	dprintf("Assigning from another Bigint");
	this->copyFromBigint(bint);
	return *this;
}
Bigint& Bigint::operator=(const std::string& strnum){
	dprintf("Assigning from another Bigint");
	this->copyFromString(strnum);
	return *this;
}
Bigint& Bigint::operator=(const char* strnum){
	dprintf("Assigning from another Bigint");
	this->copyFromCString(strnum);
	return *this;
}
Bigint& Bigint::operator=(const llint num){
	dprintf("Assigning from another Bigint");
	this->copyFromInt(num);
	return *this;
}

//comparisons
bool Bigint::operator==(const Bigint& bint) const {
    if(this->m_iNumLength != bint.m_iNumLength ||
        this->m_bNegative != bint.m_bNegative){
        
        return false;
    }
    
    for(ullint i = 0; i < this->m_iNumLength; i++){
        if(this->getDigitChar(i) != bint.getDigitChar(i)){
            return false;
        }
    }
    
    return true;
}

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

void Bigint::copyFromBigint(const Bigint& bint){
	this->m_iNumLength = bint.m_iNumLength;
	this->m_vNumList = bint.m_vNumList;
	this->m_bNegative = bint.m_bNegative;
}

void Bigint::copyFromString(const std::string& strint){
	//dprintf("Copying data from string: %s", strint.c_str())
	this->clear();
	bool startedNumber = false;  //if the number, not leading 0's, started
	
	ullint arrsz = strint.size();
	for(ullint i = 0; i < arrsz; i++){
		if(strint[i] > '/' && strint[i] < ':'){
			if(startedNumber || strint[i] != '0'){
				this->setDigit(CHAR_TO_INT(strint[i]), arrsz-i-1, true);
				startedNumber = true;
			}
		}
	}
	if(arrsz && strint[0] == '-'){
		this->m_bNegative = true;
	}
}

void Bigint::copyFromCString(const char* arr){
	this->clear();
	bool startedNumber = false;  //if the number, not leading 0's, started
	
	ullint arrsz = strlen(arr);
	for(ullint i = 0; i < arrsz; i++){
		if(arr[i] > '/' && arr[i] < ':'){
			if(startedNumber || arr[i] != '0'){
				this->setDigit(CHAR_TO_INT(arr[i]), arrsz-i-1, true);
				startedNumber = true;
			}
		}
	}
	if(arrsz && arr[0] == '-'){
		this->m_bNegative = true;
	}
}

void Bigint::copyFromInt(const llint intnum){
	this->clear();	
	ullint num = 0; // *just* to be able to hold the minimum of the long long int
	
	if(intnum < 0){
		this->m_bNegative = true;
		num = (-1*intnum);
	}
	else{
		this->m_bNegative = false;
		num = intnum;
	}
	
	{
		int i = 0;
		do{
			this->setDigit(num % 10, i, true);
			num/=10;
			i++;
		}
		while(num != 0);
	}
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
