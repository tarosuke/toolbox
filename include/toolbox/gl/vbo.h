/** 頂点バッファオブジェクトとインデクスバッファオブジェクト
 * Copyright (C) 2017,2019 tarosuke<webmaster@tarosuke.net>
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
 * 追加データがある場合、VBOの子クラスを用意してDrawでAttributeとして設定する
 */
#pragma once



namespace TB{

	class VBO{
		VBO();
		VBO(const VBO&);
		void operator=(const VBO&);
	public:
		enum DrawType{
			triangles,
			quads,
			points,
			triangleStrip,
			triangleFan
		};

		struct V2{
			float u;
			float v;
		}__attribute__((packed));
		struct V3{
			float x;
			float y;
			float z;
		}__attribute__((packed));

		struct V_UV{
			V3 vertex;
			V2 texture;
		}__attribute__((packed));
		struct V_UV_NORMAL{
			V3 vertex;
			V2 texture;
			V3 normal;
		}__attribute__((packed));

		template<typename T> static VBO* New(
			unsigned noi,
			const unsigned* index,
			unsigned nov,
			const T* vertex, //NOTE:vertex に多次元配列を与えないこと
			DrawType type = triangles){
			Init i;
			return SetupBuffer(i, noi, index, nov, vertex, sizeof(T), type) ? new VBO(i) : 0;
		}

		virtual void Draw();

		virtual ~VBO();

	protected:
		const unsigned indexBuffer;
		const unsigned vertexBuffer;
		const unsigned numOfVertex;
		const int drawType;

		struct Init{
			unsigned indexBuffer;
			unsigned vertexBuffer;
			unsigned numOfVertex;
			int drawType;
		};
		VBO(const Init&);

	private:
		static bool SetupBuffer(
			Init& i,
			unsigned numOfIndex,
			const unsigned* index,
			unsigned unmOfVertex,
			const void* vertex,
			unsigned size,
			DrawType type);
	};

}
