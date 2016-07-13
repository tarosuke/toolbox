/************************************************************************* 文字列
 */

#include "include/toolbox/string.h"
#include <string.h>



namespace TB{

	String::String(const String& t){
		*this = t;
	}


	String::String(const char* t){
		*this = t;
	}


	String& String::operator=(const String& t){
		length = t.length;
		if(length){
			Resize(length + 1); //+1はゼロ終端の分
			memcpy(&(*this)[0], t.GetRawBody(), length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator=(const char* t){
		length = t ? strlen(t) : 0;
		if(length){
			Resize(length + 1);
			memcpy(&(*this)[0], t, length + 1);
		}
		(*this)[length] = 0;
		return *this;
	}

	String& String::operator+=(const String& t){
		if(!t.length){ return *this; }
		const unsigned newLength(length + t.length);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t.GetRawBody(), t.length + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String& String::operator+=(const char* t){
		if(!t){ return *this; }
		const unsigned orgLength(strlen(t));
		if(!orgLength){ return *this; }
		const unsigned newLength(length + orgLength);
		Resize(newLength + 1);
		memcpy(&(*this)[length], t, orgLength + 1);
		length = newLength;
		(*this)[length] = 0;
		return *this;
	}
	String String::operator+(const String& t)const{
		String newString(*this);
		newString += t;
		return newString;
	}
	String String::operator+(const char* t)const{
		String newString(*this);
		newString += t;
		return newString;
	}

	String String::SubStr(unsigned b, unsigned l){
		String newString;

		if(!l || length <= b){
			//結果が空
			return newString;
		}

		if(length < b + l){
			//長さ調整
			l = length - b;
		}

		newString.Resize(l + 1);
		memcpy(&newString[0], GetRawBody() + b, l);
		newString[l] = 0;
		return newString;
	}

}
