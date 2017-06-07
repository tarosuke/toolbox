/** Tokenizer
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 */
#include <ctype.h>

#include <toolbox/tokenizer.h>


namespace TB{

	Tokenizer::Tokenizer(const char* path) : file(fopen(path, "r")){
		GetNextToken();
	}

	Tokenizer::~Tokenizer(){
		if(file){ fclose(file); }
	}

	const char* Tokenizer::GetToken(){
		//エラーかEOFでなければトークンを返す
//		return (!file || !*tokenBuffer) ? GetNextToken() : tokenBuffer;
		return 0;
	}

	const char* Tokenizer::GetNextToken(){
		if(!file){ return 0; }

		//トークンの読み込み
		for(int c; 0 <= (c = fgetc(file));){
			//ホワイトスペース
			if(isspace(c)){
				if(token.IsEmpty()){
					//トークン蓄積中でなければ読み飛ばす
					continue;
				}
				//トークン蓄積中ならトークンが終了したので終了
				break;
			}

			//コメント
			if(c == commentChar){
				//行末まで読み飛ばす
				while(0 <= (c = fgetc(file)) && (c == '\r' || c == '\n'));
				continue;
			}

			//トークンなので蓄積
			token += c;
		}

		return 0;
	}


	bool Tokenizer::Get(unsigned& v){
		char* np;
		const long long value(strtoll(token, &np, 0));

		if(!*np && 0 <= value){
			v = value;
			GetNextToken();
			return true;
		}
		return false;
	}

	bool Tokenizer::Get(int& v){
		char* np;
		const int value(strtol(token, &np, 0));

		if(!*np){
			v = value;
			GetNextToken();
			return true;
		}
		return false;
	}

	bool Tokenizer::Get(double& v){
		char* np;
		const double value(strtod(token, &np));

		if(!*np){
			v = value;
			GetNextToken();
			return true;
		}
		return false;
	}

	const String& Tokenizer::Get(){
		return token;
	}

	void Tokenizer::Rewind(){
		rewind(file);
		token.Clear();
		GetNextToken();
	}

}
