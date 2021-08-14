/************************************************************** riff container
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/type.h>



namespace TB {



#define S2ID(s)                                                                \
	(((u32)((s)[3]) << 24) | ((u32)((s)[2]) << 16) | ((u32)((s)[1]) << 8) |    \
	 (u32)((s)[0]))


	struct RIFF {
		struct Chunk {
			u32 id;
			u32 size;
			char body[0];
			Chunk(u32 id, u32 size) : id(id), size(size - sizeof(*this)){};
		};

		RIFF(const char* format);
		virtual ~RIFF();

		void Add(Chunk*); //チャンクを追加する
		void AddChild(Chunk*); //子チャンクを追加して子チャンクの次を待つ
		void Close(); //チャンクを閉じる(一つ親チャンクに戻る)
		void End(); //すべてのチャンクを閉じる

	protected:
		virtual void Write(const void*, unsigned size) = 0;
		virtual void Read(unsigned size, void*) = 0;

		static u32 Str2ID(const char*);

	private:
		struct RIFFChunk : public Chunk {
			u32 formType;
			RIFFChunk(u32 id)
				: Chunk(S2ID("RIFF"), sizeof(*this)), formType(id){};
		} chunk;
	};



	struct WAV : public RIFF {

		// enum struct ChunkID : u32 {
		// 	RIFF = S2ID("RIFF"),
		// 	format = S2ID("fmt\0"),
		// 	format = S2ID("data"),
		// };

		struct FMT : public Chunk {
		private:
			enum FormatTag : u16 {
				PCM = 0x0001,
			} formatTag;
			u16 channels; // おそらく2のみが有効
			u32 samplesPerSec;
			u32 bytesPerSec;
			u16 blockAlign;
			u16 bitsPerSample;
		};

		struct PCM : public Chunk {
		private:
			struct Frame {
				i16 left;
				i16 right;
			} data[0];

			PCM(const Frame data[], unsigned frames);
		};

	private:
		static const char type[4]; //"wave"
	};



	// このあたりの構造を管理するのにマップしたファイルのイメージをそのまま使うか、管理領域は別にしてデータを指して使うかの判断をファイルの読み書きそれぞれに対してする必要がある。

	// チャンクのヘッダ以外の部分は「葉」にしかないので管理領域は別に作る方がいいかも知れない。親チャンクへのポインタを持てるし
	// 葉と枝の追加方法はちょっと違う...かも

	// 作るときはArrayに格納？読むときもか。Arrayを使うならデータ上のインデックスはポインタではなく配列インデックスになる
	// チャンクに何か追加するときには親チャンクにも同じだけのサイズを追加する必要があるので、チャンクの追加先は重要


}
