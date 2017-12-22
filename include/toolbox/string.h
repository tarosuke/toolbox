/************************************************************************* 文字列
 */
#pragma once

#include <toolbox/container/array.h>



namespace TB{

	class String : public Array<char>{
	public:
		String() : length(0){ (*this)[0] = 0; };
		String(const String&);
		String(const char*);

		String& operator=(const String&);
		String& operator=(const char*);
		operator char*(){
			return GetRawBody();
		};
		operator const char*() const {
			return GetRawBody();
		};
		String& operator +=(const String&);
		String& operator +=(const char*);
		String& operator +=(char);
		String operator +(const String&) const;
		String operator +(const char*) const;

		bool operator==(const char*) const;

		bool IsEmpty()const{ return !length; };
		unsigned Length(){ return length; };

		String SubStr(unsigned start, unsigned length)const;
		void Clear(){ (*this)[0] = 0; length = 0; };

	private:
		unsigned length;
	};

}
