/***** シリアライズ／デシリアライズ付きの型
 */


#pragma once



namespace TB{

	template <typename T> class Type{
	public:
		Type(const T& t) : body(t){};
		const T& operator=(const T& t){ body = t; return body; };
		operator const T&() const { return body; };

		//de-serializer
		const T& operator=(const char*);

	private:
		T body;
	};

}

