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
#ifdef BIGINT_ENABLE_FLOATS
/// Constructs Bigint with value from normal float.
Bigint::Bigint(const float num){
	dprintf("Constructing with a float");
	this->copyFromFloat((long double)num);
}
/// Constructs Bigint with value from double.
Bigint::Bigint(const double num){
	dprintf("Constructing with a double");
	this->copyFromFloat((long double)num);
}
/// Constructs Bigint with value from long double.
Bigint::Bigint(const long double num){
	dprintf("Constructing with a long double");
	this->copyFromFloat(num);
}
#endif // !BIGINT_ENABLE_FLOATS


//assignments
Bigint& Bigint::operator=(const Bigint& bint){
	dprintf("Assigning from another Bigint");
	this->copyFromBigint(bint);
	return *this;
}
Bigint& Bigint::operator=(const std::string& strnum){
	dprintf("Assigning from std::string");
	this->copyFromString(strnum);
	return *this;
}
Bigint& Bigint::operator=(const char* strnum){
	dprintf("Assigning from c-string");
	this->copyFromCString(strnum);
	return *this;
}
Bigint& Bigint::operator=(const llint num){
	dprintf("Assigning from long long");
	this->copyFromInt(num);
	return *this;
}
#ifdef BIGINT_ENABLE_FLOATS
/// Copies float value to itself.
Bigint& Bigint::operator=(const float num){
	dprintf("Assigning from float");
	this->copyFromFloat((long double)num);
	return *this;
}
/// Copies double value to itself.
Bigint& Bigint::operator=(const double num){
	dprintf("Assigning from double");
	this->copyFromFloat((long double)num);
	return *this;
}
/// Copies long double value to itself.
Bigint& Bigint::operator=(const long double num){
	dprintf("Assigning from long double");
	this->copyFromFloat(num);
	return *this;
}
#endif // !BIGINT_ENABLE_FLOATS

//copyFrom* are too considered 
void Bigint::copyFromBigint(const Bigint& bint){
	this->m_iNumLength = bint.m_iNumLength;
	this->m_vNumList = bint.m_vNumList;
	this->m_bNegative = bint.m_bNegative;
	this->m_bIsZero = bint.m_bIsZero;
}

void Bigint::copyFromString(const std::string& strint, bool bSkipDecimal){
	//dprintf("Copying data from string: %s", strint.c_str())
	this->clear();
	bool startedNumber = false;  //if the number, not leading 0's, started
	
	ullint arrsz = strint.size();
	ullint decPlace = 0;
	
	//and now we iterate through the list
	for(ullint i = arrsz; i-- > 0;){
		//assign the digit only if the value is in range and is either forced or not 0
		if(strint[i] > '/' && strint[i] < ':' && (startedNumber || strint[i] != '0')){
			this->setDigit(CHAR_TO_INT(strint[i]), decPlace);
			this->m_bIsZero = false;
			startedNumber = true;
			decPlace++;
		}
		else{
			//and if we find a dot, we skip. 
			//I guess, we've reached the decimal point of the given number
			if(strint[i] == '.' && bSkipDecimal){
				break;
			}
		}
	}
	if(arrsz && strint[0] == '-'){
		this->m_bNegative = true;
	}
}

void Bigint::copyFromCString(const char* arr, bool bSkipDecimal){
	this->clear();
	bool startedNumber = false;  //if the number, not leading 0's, started
	
	ullint arrsz = strlen(arr);
	ullint decPlace = 0;
	
	//and now we iterate through the list
	for(ullint i = arrsz; i-- > 0;){
		//assign the digit only if the value is in range and is either forced or not 0
		if(arr[i] > '/' && arr[i] < ':'){
			this->setDigit(CHAR_TO_INT(arr[i]), decPlace);
			this->m_bIsZero = false;
			startedNumber = true;
			decPlace++;
		}
		else{
			//and if we find a dot, we skip. 
			//I guess, we've reached the decimal point of the given number
			if(arr[i] == '.' && bSkipDecimal){
				break;
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
	if(intnum != 0){
		this->m_bIsZero = false;
	}
	
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

#ifdef BIGINT_ENABLE_FLOATS
/// Copies value to 'this' from given floating point value
void Bigint::copyFromFloat(const long double num){
	//as usual, clear the number first
	this->clear();
	
	//if float is less than 1, we don't care
	//it'll just have 0 for that case
	if(fabsl(num) < 1.0L){
		return;
	}
	this->m_bIsZero = false;
	
	//get char representation of that float
	//almost 5 kb of memory just for that
	//*hmpf*
	char fltnum[LDBL_MAX_10_EXP+LDBL_DIG+4]{};
	snprintf(fltnum, sizeof(fltnum), "%.*Lf", LDBL_DIG, num);
	
	//remove the decimal point
	//since ints just truncate
	(strchr(fltnum, '.'))[0] = '\0';
	//size of the altered formatted string
	ullint arrsz = strlen(fltnum);
	
	//iterate from last to first
	//copy digits
	for(ullint i = arrsz; i-- > 0;){
		//cool ascii char values hack
		if(fltnum[i] > '/' && fltnum[i] < ':'){
			this->setDigit(CHAR_TO_INT(fltnum[i]), arrsz-i-1, true);
		}
	}
	
	//and get the Negative
	if(fltnum[0] == '-'){
		this->m_bNegative = true;
	}
}
#endif // !BIGINT_ENABLE_FLOATS
 