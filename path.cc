/** Path search
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <sys/stat.h>

#include <toolbox/path.h>



namespace TB{

	/** パスを合成しながらファイルの存在をチェックしてファイルが存在したパスを返す
	 * 以下の場合はpathのコピーを返す。
	 * 1. pathが絶対パス
	 * 2. カレントディレクトリにpathが示すファイルがあった
	 * 3. ファイルが見つからなかった
	 * でなければresPath+pathをチェックし、ファイルがあればそれを返す。
	 */
	String Path::StatPath(const String& resPath, const String& path){
		if(!path.Length() || path[0] == '/'){
			//絶対パスか空文字列
			return path;
		}

		//カレントディレクトリを確認
		if(Is(path)){
			//カレントディレクトリにあったのでpathをそのまま返す
			return path;
		}

		//resPathを追加して確認
		String p(resPath + path);
		if(Is(p)){
			//resPath以下にあったのでそのパスを返す
			return p;
		}

		//ファイルがなかったのでpathをそのまま返す(帰った先でエラーになる)
		return path;
	}

	/** 与えられたパスにファイルが存在するかどうかチェック
	 * 通常ファイルかシンボリックリンクの先が通常ファイルである場合に真を返す
	 */
	bool Path::Is(const String& p){
		struct stat statBuff;
		if(!stat(p, &statBuff)){
			return S_ISREG(statBuff.st_mode);
		}
		return false;
	}


	/*** 構築子
	 */
	Path::Path(const String& r, const char* p) : String(StatPath(r, p)){}
	Path::Path(const String& r, const String& p) : String(StatPath(r, p)){}
	Path::Path(const String& r, const Path& p) : String(StatPath(r, p)){}

}
