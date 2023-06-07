/** Path search
 * Copyright (C) 2018 tarosuke<webmaster@tarosuke.net>
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

#include <string>



namespace TB {

	class Path {
	public:
		Path() = default;
		Path(const char* path) : path(path){};
		Path(const std::string& path) : path(path){};

		operator std::string() const { return path; };
		operator const std::string&() const { return path; };

		/* パスの最後のセグメントを取り出す
		 * 戻り値はこのインスタンスが変更されるまで有効
		 * 永続性が必要ならstd::strnigにでもコピーすること
		 */
		const char* LastSegment() const;

		// ファイルの存在チェック
		bool IsFile();
		bool IsDirectory();

	private:
		std::string path;
	};
}
