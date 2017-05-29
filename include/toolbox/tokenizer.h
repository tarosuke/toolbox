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


namespace TB{

	class String;

	class Tokenizer{
		Tokenizer();
		Tokenizer(const Tokenizer&);
		void operator=(const Tokenizer&);
	public:

		Tokenizer(const char*);
		~Tokenizer();

		/** ワードの取得
		 */
		bool Get(unsigned&);
		bool Get(int&);
		bool Get(float&);
		bool GetSubToken(String&);
		bool Get(String&);

		/** 巻き戻し
		 */
		void Rewind();


		//廃止
		const char* GetToken();
		const char* GetNextToken();


	private:

		static const char commentChar = '#';

		FILE* const file;
		static const unsigned maxTokenLength = 256;
		char tokenBuffer[maxTokenLength];
		bool isNumeric;
	};

}

