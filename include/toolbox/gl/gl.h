/** glewを併用するためのヘッダ(順番とか面倒なので)
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
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <syslog.h>


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

	bool ErrorCheck();

}