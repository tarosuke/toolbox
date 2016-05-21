/** 数値型テンプレートクラス
 * 主に特別な数値を作るための親クラスとして、互換性のない数値を混ぜないために使う
 */
#pragma once

namespace wO{

	template<typename T> class Number{
	public:
		Number(T init) : body(init){};
		operator T(){ return body; };
		T operator+(const T& t) const{
			return body + t;
		};
		T operator+=(const T& t){
			body += t;
			return *this;
		};
		T operator-(const T& t) const{
			return body - t;
		};
		T operator-=(const T& t){
			body -= t;
			return *this;
		};
		T operator*(const T& t) const{
			return body * t;
		};
		T operator*=(const T& t){
			body *= t;
			return *this;
		};
		T operator/(const T& t) const{
			return body / t;
		};
		T operator+=(const T& t){
			body /= t;
			return *this;
		};

	protected:
		T body;
	};

}
