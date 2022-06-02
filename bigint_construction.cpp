/*
    file for bigint core stuff 
    definitions of:
        * Class Constructors
        * Class Assignment routines
        * functions copyFromX, which basically do all the work
            for constructors and assignment routies.
*/

#include "bigint.hpp"

//constructors

//empty constructor
//makes Bigint with value 0;
Bigint::Bigint() {
	dprintf("Constructing an empty bigint");
	this->clear();
}
//constructs Bigint with value from another Bigint
Bigint::Bigint(const Bigint& bint) {
	dprintf("Constructing Bigint with another Bigint");
	this->copyFromBigint(bint);
}
//constructs Bigint with value from std::string
Bigint::Bigint(const std::string& strnum) {
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


//copyFrom* are too considered 
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
