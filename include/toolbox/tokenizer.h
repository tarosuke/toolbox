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

	class Tokenizer{
		Tokenizer();
		Tokenizer(const Tokenizer&);
		void operator=(const Tokenizer&);
	public:

		Tokenizer(const char*);
		~Tokenizer();

		/** トークンの取得
		 * トークンを取得する。ファイルが開いていない、ファイルが終端に達したなどで
		 * 取得できなかった場合は0を返す
		 */
		const char* GetToken(); //トークンバッファに入っている内容を取得
		const char* GetNextToken(); //次のトークンを取得して取得

		/** トークンの種類チェック
		 */
		bool IsNumeric(){ return isNumeric; };

		/** 巻き戻し
		 */
		void Rewind();

	private:

		static const char commentChar = '#';

		FILE* const file;
		static const unsigned maxTokenLength = 256;
		char tokenBuffer[maxTokenLength];
		bool isNumeric;
	};

}

