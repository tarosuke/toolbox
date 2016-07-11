/************************************************************************* 文字列
 */
#pragma once

#include <toolbox/container/array.h>



namespace TB{

	class String : public Array<char>{
	public:
		String() : length(0){ *GetRawBody() = 0; };
		String(const String&);
		String(const char*);

		String& operator=(const String&);
		String& operator=(const char*);
		operator char*(){
			return GetRawBody();
		};
		String& operator +=(const String&);
		String& operator +=(const char*);
		String operator +(const String&) const;
		String operator +(const char*) const;

	private:
		unsigned length;
	};

}
