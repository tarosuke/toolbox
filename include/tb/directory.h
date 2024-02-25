/**
 * Copyright (C) 20172 2024 tarosuke<webmaster@tarosuke.net>
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
 *
 * NOTE:ディレクトリを開けなかった場合intをthrowする
 */
#pragma once


extern "C" {
	struct dirent;
}


#include <tb/list.h>
#include <tb/string.h>


namespace tb {

	class Directory {
		Directory(const Directory&);
		void operator=(const Directory&);

	public:
		// 各ファイルエントリ
		class Node : public List<Node>::Node {
			Node();
			Node(const Node&);
			void operator=(const Node&);

		public:
			Node(const dirent&);

			bool IsDir() const;
			bool IsRegular(bool includeSymLinks = true) const;
			bool IsBlock() const; // block device
			bool IsChar() const; // char device
			bool IsFifo() const; // named pipe
			bool IsLink() const; // symbolic link
			bool IsSocket() const; // UNIX domain socket

			unsigned GetATime() const { return atime; };
			unsigned GetMTime() const { return mtime; };
			unsigned GetCTime() const { return ctime; };

			operator const char*() const { return (const char*)name; };

		private:
			String name; // ファイル名
			const unsigned char type; // direntのd_typeと同じ
			unsigned atime;
			unsigned mtime;
			unsigned ctime;
		};

		// 反復子
		class Itor : public List<Node>::I {
			Itor();
			Itor(const Itor&);
			void operator=(const Itor&);

		public:
			Itor(Directory& d) : List<Node>::I(d.entries){};
		};

		// ソート用ウエイト
		class Weight {
		public:
			Weight()
				: originOrder(0), random(0), name(0), ctime(0), mtime(0),
				  atime(0), extra(0){};
			float originOrder;
			float random;
			float name;
			float ctime;
			float mtime;
			float atime;
			float extra;
		};

		// エントリの読み込み
		Directory(const char* path, const Weight* = 0);
		~Directory();

	protected:
		Directory(){};
		void Load(const char* path, const Weight*);

		/** Filter
		 * return true if the node is valid
		 */
		virtual bool IsValid(const dirent&) const { return true; };

		/** scorer of sorter
		 */
		virtual float ScoreExtra(const Node&) { return 0; };

	private:
		List<Node> entries;
		Weight weight;

		static int Sign(int v) { return !v ? 0.0f : v < 0 ? -1.0 : 1.0; };
	};

}
