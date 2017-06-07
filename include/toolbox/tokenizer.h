/** Tokenizer
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 *
 * 要するに字句解析器
 * * 記号は単体で一つのトークンになる
 * * 現状では#から行末までコメント
 * * 読み込んだトークンは次にGetNextTokenを呼ぶまで有効
 */
#pragma once

#include <stdio.h>

#include "string.h"


namespace TB{

	class Tokenizer{
		Tokenizer();
		Tokenizer(const Tokenizer&);
		void operator=(const Tokenizer&);
	public:

		Tokenizer(const char*);
		~Tokenizer();

		/** ワードの取得
		 * トークンがそれぞれの型に適合するか調べて適合していれば値を取得してtrueを返す
		 */
		bool Get(unsigned&);
		bool Get(int&);
		bool Get(double&);
		const String& Get();

		/** 巻き戻し
		 */
		void Rewind();


		//廃止
		const char* GetToken();
		const char* GetNextToken();


	private:

		static const char commentChar = '#';

		FILE* const file;
		String token;
	};

}

