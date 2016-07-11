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
		Resize(length + 1); //+1はゼロ終端の分
		memcpy(GetRawBody(), t.GetRawBody(), length + 1);
		return *this;
	}
	String& String::operator=(const char* t){
		length = strlen(t);
		Resize(length + 1);
		memcpy(GetRawBody(), t, length + 1);
		return *this;
	}

	String& String::operator+=(const String& t){
		const unsigned newLength(length + t.length);
		Resize(newLength + 1);
		memcpy(GetRawBody() + length, t.GetRawBody(), t.length + 1);
		length = newLength;
		return *this;
	}
	String& String::operator+=(const char* t){
		const unsigned orgLength(strlen(t));
		const unsigned newLength(length + orgLength);
		Resize(newLength + 1);
		memcpy(GetRawBody() + length, t, orgLength + 1);
		length = newLength;
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

}
