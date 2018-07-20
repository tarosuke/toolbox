/**
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
 #include <assert.h>
#include <syslog.h>

#include <toolbox/gl/gl.h>
#include <toolbox/gl/vbo.h>


namespace TB{

	bool VBO::SetupBuffer(
		Init& i,
		unsigned noi,
		const unsigned* index,
		unsigned nov,
		const void* vertex,
		unsigned size,
		bool quad){
		syslog(LOG_DEBUG, "VBO: i:%u/%p v:%u/%p size:%u", noi, index, nov, vertex, size);
		if(64 < size){
			syslog(LOG_WARNING, "VBO: size of vertex element(%u) over 64bytes", size);
		}

		//頂点バッファ確保と読み込み
		unsigned vBuff(0);
		glGenBuffers(1, &vBuff);
		glBindBuffer(GL_ARRAY_BUFFER, vBuff);
		glBufferData(
			GL_ARRAY_BUFFER,
			size * nov,
			vertex,
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//インデックスバッファ確保と読み込み
		unsigned iBuff(0);
		glGenBuffers(1, &iBuff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(int) * noi,
			index,
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		i.indexBuffer = iBuff;
		i.vertexBuffer = vBuff;
		i.numOfVertex = noi;
		i.drawType = quad ? GL_QUADS : GL_TRIANGLES;

		if(glGetError() == GL_NO_ERROR){
			return true;
		}

		//確保失敗
		syslog(LOG_ERR, "failed to assign VBO");
		if(iBuff){ glDeleteBuffers(1, &iBuff); }
		if(vBuff){ glDeleteBuffers(1, &vBuff); }

		return false;
	}

	VBO::VBO(const Init& i) :
		indexBuffer(i.indexBuffer),
		vertexBuffer(i.vertexBuffer),
		numOfVertex(i.numOfVertex),
		drawType(i.drawType){}

	VBO::~VBO(){
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &vertexBuffer);
	}

	void VBO::Draw(){
		//インデックスバッファを有効化
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		//使うバッファを有効化
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//頂点バッファをセットアップ
		const unsigned stride(sizeof(V_UV));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexPointer(3, GL_FLOAT, stride, 0);
		glTexCoordPointer(2, GL_FLOAT, stride, &(*(V_UV*)0).texture);

		//描画
		glDrawElements(drawType, numOfVertex, GL_UNSIGNED_INT, 0);

		//後始末
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	//UV付きVBO
	class VBO_NORMAL : public VBO{
	public:
		VBO_NORMAL(const Init& i) : VBO(i){};
		void Draw() override{
			//インデックスバッファを有効化
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

			//使うバッファを有効化
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			//頂点バッファをセットアップ
			const unsigned stride(sizeof(V_UV_NORMAL));
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexPointer(3, GL_FLOAT, stride, NULL);
			glTexCoordPointer(2, GL_FLOAT, stride, &(*(V_UV_NORMAL*)0).texture);
			glNormalPointer(GL_FLOAT, stride, &(*(V_UV_NORMAL*)0).normal);

			//描画
			glDrawElements(drawType, numOfVertex, GL_UNSIGNED_INT, (void*)0);

			//後始末
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		};
	};

}
