/** Tokenizer
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 */
#include <ctype.h>

#include <toolbox/tokenizer.h>


namespace TB{

	Tokenizer::Tokenizer(const char* path) :
		file(fopen(path, "r")), isNumeric(false){
		GetNextToken();
	}

	Tokenizer::~Tokenizer(){
		if(file){ fclose(file); }
	}

	const char* Tokenizer::GetToken(){
		//エラーかEOFでなければトークンを返す
		return (!file || !*tokenBuffer) ? GetNextToken() : tokenBuffer;
	}

	const char* Tokenizer::GetNextToken(){
		if(!file){ return 0; }

		//書き込み位置を初期化
		char* wp(tokenBuffer);

		//トークンの読み込み
		for(int c, isNumeric = true; 0 <= (c = fgetc(file));){
			//ホワイトスペース
			if(isspace(c)){
				if(wp == tokenBuffer){
					//まだトークンが読まれてなければ読み飛ばす
					continue;
				}
				//現時点までをトークンとする
				break;
			}

			//コメント
			if(c == commentChar){
				//行末まで読み飛ばす
				while(0 <= (c = fgetc(file)) && (c == '\r' || c == '\n'));
				continue;
			}

			//普通のトークンなのでトークンに追加
			isNumeric &= !!isdigit(c); //数値チェック
			*wp++ = c;
		}

		//ターミネーターを書き込んでトークンを返す
		*wp++ = 0;
		return GetToken();
	}

	void Tokenizer::Rewind(){
		rewind(file);
		GetNextToken();
	}

}
