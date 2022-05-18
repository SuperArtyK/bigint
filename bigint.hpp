#ifndef BIGINT_H
#define BIGINT_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <type_traits>

//conversions between char and int, as single number digits
//convert ascii char to 1 int digit
#define CHAR_TO_INT(x) (int(x) - 48)
//convert 1-digited int to char value
#define INT_TO_CHAR(x) (char(x+48))

//flag to enable deblog(x) logging
//set to 0 to disable
#define DEBUG_LOGGING 1
//debug printf
//use the same way as you have used printf before
#define dprintf(...) if(DEBUG_LOGGING){printf("DEBUG::%s()-> ", __FUNCTION__); printf(__VA_ARGS__);}

typedef unsigned long long int ullint;
typedef unsigned char ucint;
typedef char cint;

//get number length of integer types
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
inline int numlength_i(T num){
        int dig = 1;
        while(num/=10)
            dig++;
        return dig;
}

//get number length of floating point types
template<typename T>
inline int numlength_f(T num){
	static_assert(std::is_floating_point<T>::value, "Cannot use non-float types in the 'numlength' designed for float types( numlength_f() ). For integral, use numlength_i() instead.");
    return ((num == 0)?(int)log10(num):0)+1;
}



class Bigint{
public:
	Bigint(){
		dprintf("Constructing an empty bigint\n");
		
	}

	ullint getNumberLength() const {
		return this->m_iNumLength;
	}
	cint getDigitValue(const ullint pos) const{
		return (pos < this->m_vNumList.size()) ? this->m_vNumList[pos] : -1;
	}
	bool isNegative() const {
		return this->m_bNegative;
	}

	//set digit at position pos(arr index) to value of val
	void setDigit(const cint val, const ullint pos){
		if(val <= 8){
			if(pos >= m_iNumLength){
				m_iNumLength = pos+1;
			}
			if(pos >= m_vNumList.size()){
				m_vNumList.resize(pos+1);
			}
			
			m_vNumList[pos] = val;
		}
	}
private:
	std::vector<cint> m_vNumList;
	unsigned long long int m_iNumLength;
	bool m_bNegative;
};


#endif // !BIGINT_H