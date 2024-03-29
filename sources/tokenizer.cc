/** Tokenizer
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 */
#include <ctype.h>

#include <toolbox/tokenizer.h>


namespace TB{

	Tokenizer::Tokenizer(const char* path) : file(fopen(path, "r")), line(1){
		GetNextToken();
	}

	Tokenizer::~Tokenizer(){
		if(file){ fclose(file); }
	}

	bool Tokenizer::GetNextToken(){
		if(!file){ return false; }

		//トークンの読み込み
		for(int c; 0 <= (c = fgetc(file));){
			if(c == '\n'){
				++line;
			}

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
				while(0 <= (c = fgetc(file)) && c == '\n');
				continue;
			}

			//トークンなので蓄積
			token << c;
		}
		return !token.IsEmpty();
	}


	bool Tokenizer::Get(unsigned& v){
		if(!file){ return false; }
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
		if(!file){ return false; }
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
		if(!file){ return false; }
		char* np;
		const double value(strtod(token, &np));

		if(!*np){
			v = value;
			GetNextToken();
			return true;
		}
		return false;
	}

	bool Tokenizer::Get(String& v){
		if(!file){ return false; }
		v = token;
		GetNextToken();
		return true;
	}

	void Tokenizer::Rewind(){
		rewind(file);
		token.Clear();
		line = 1;
		GetNextToken();
	}

}
