//// glewを併用するためのヘッダ(順番とか面倒なので)

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>


namespace GL{

	/** ScopeEnabler
	* RAIIでインスタンスが存在するスコープ内でのみ機能を有効にする
	* NOTE:スコープから出ると以前の状態とは無関係にdisableされる
	*/
	class Enabler{
		Enabler();
		Enabler(const Enabler&);
		void operator=(const Enabler&);
	public:
		Enabler(int function) : function(function){
			glEnable(function);
		};
		~Enabler(){
			glDisable(function);
		};
	private:
		const int function;
	};

}