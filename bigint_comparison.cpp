/*
    file for bigint comparison operator stuff 
    definitions of:
        * comparison operators with Bigint, int, c-string and std::string:
            operator==
            operator<
            operator<=
            operator>
            operator>=
    that are not-inline(eg. too big for the header file)
*/

#include "bigint.hpp"

///////////////
//equality
///////////////

//bigint == bigint
bool Bigint::operator==(const Bigint& bint) const {
    if(this->m_bNegative != bint.m_bNegative || 
        this->m_iNumLength != bint.m_iNumLength){
        
        return false;
    }
    //checking last digit first
    //we don't care much about insignificant digits at first
    for(ullint i = this->m_iNumLength-1; i > 0; i--){
        if(this->getDigitChar(i) != bint.getDigitChar(i)){
            return false;
        }
    }
    //check for last one here
    //so uint doesnt wrap around, causing infinite loop at the very least
    if(this->getDigitChar(0) != bint.getDigitChar(0)){
        return false;
    }
    return true;
}

//bigint == c-string
bool Bigint::operator==(const char* strnum) const {
    if(this->m_bNegative != (strnum[0] == '-') ||
        this->m_iNumLength != strlen(strnum)){ 
        return false;
    }
    return this->operator==(Bigint(strnum));
}
//bigint == std::string
bool Bigint::operator==(const std::string& strnum) const {
    if(this->m_bNegative != (strnum[0] == '-') ||
        this->m_iNumLength != strnum.size()){ 
        return false;
    }
    return this->operator==(Bigint(strnum));
}

//bigint == long long
bool Bigint::operator==(const llint num) const{
    if((num < 0) != this->m_bNegative){//number doesn't have the same sign
        return false;
    }
    return this->operator==(Bigint(num));
}

///////////////
//less than
///////////////

//bigint < bigint
bool Bigint::operator<(const Bigint& bint) const{
    if(this->m_bNegative != bint.m_bNegative){
        return (this->m_bNegative && !bint.m_bNegative);
    }
    const llint numlendiff = (this->m_iNumLength*((this->m_bNegative)?-1:1)) - (bint.m_iNumLength*((bint.m_bNegative)?-1:1));
    if(numlendiff < 0){
        return true;
    }
    if(numlendiff == 0){
        //checking last digit first
        //we don't care much about insignificant digits at first
        for(ullint i = 0; i < this->m_iNumLength; i++){
            if(this->m_bNegative){
                if(this->getDigitChar(i) > bint.getDigitChar(i)){
                    return true;
                }
            }
            else{
                if(this->getDigitChar(i) < bint.getDigitChar(i)){
                    return true;
                }
            }
            
        }
    }
    return false;
}
//c-string
bool Bigint::operator<(const char* strnum) const {
    //quick check, before converting to Bigint and comparing
    if(this->m_bNegative && strnum[0] != '-'){
        return true;
    }
    return operator<(Bigint(strnum));
}
//std::string
bool Bigint::operator<(const std::string& strnum) const {
    //quick check, before converting to Bigint and comparing
    if(this->m_bNegative && strnum[0] != '-'){
        return true;
    }
    return operator<(Bigint(strnum));
}
//long long
bool Bigint::operator<(const llint num) const {
    //quick check, before converting to Bigint and comparing
    if(this->m_bNegative && num > 0){
        return true;
    }
    return operator<(Bigint(num));
}

///////////////
//greater than
///////////////

//bigint > bigint
bool Bigint::operator>(const Bigint& bint) const{
    if(this->m_bNegative != bint.m_bNegative){
        return (!this->m_bNegative && bint.m_bNegative);
    }
    const llint numlendiff = (this->m_iNumLength*((this->m_bNegative)?-1:1)) - (bint.m_iNumLength*((bint.m_bNegative)?-1:1));
    if(numlendiff > 0){
        return true;
    }
    if(numlendiff == 0){
        //checking last digit first
        //we don't care much about insignificant digits at first
        for(ullint i = 0; i < this->m_iNumLength; i++){
            if(this->m_bNegative){
                if(this->getDigitChar(i) < bint.getDigitChar(i)){
                    return true;
                }
            }
            else{
                if(this->getDigitChar(i) > bint.getDigitChar(i)){
                    return true;
                }
            }
            
        }
    }
    return false;
}
//c-string
bool Bigint::operator>(const char* strnum) const {
    //quick check, before converting to Bigint and comparing
    if(!this->m_bNegative && strnum[0] == '-'){
        return true;
    }
    return operator>(Bigint(strnum));
}
//std::string
bool Bigint::operator>(const std::string& strnum) const {
    //quick check, before converting to Bigint and comparing
    if(!this->m_bNegative && strnum[0] == '-'){
        return true;
    }
    return operator>(Bigint(strnum));
}
//long long
bool Bigint::operator>(const llint num) const {
    //quick check, before converting to Bigint and comparing
    if(!this->m_bNegative && num < 0){
        return true;
    }
    return operator>(Bigint(num));
}


