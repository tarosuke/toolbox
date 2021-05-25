/************************************************************ toolbox graphics
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

#include <toolbox/geometry/matrix.h>
#include <toolbox/tg/tg.h>
#include <toolbox/container/list.h>
#include <toolbox/gl/vbo.h>
#include <toolbox/gl/texture.h>
#include <toolbox/image.h>



namespace TG {

	class Object : public TB::List<Object>::Node {
	public:
		virtual void Draw(){};
		virtual void Traw(){};
		virtual void Tick(){};

		virtual ~Object(){};

	protected:
	private:
	};


	class Mesh : public Object {
		Mesh();
		Mesh(const Mesh&);
		void operator=(const Mesh&);

	public:
		template <typename T> static Mesh*
		New(unsigned noi,
			const unsigned index[],
			unsigned nov,
			const T vertex[],
			const char* path) {
			if (auto* const image = TB::Image::New(path)) {
				auto* const body(New(noi, index, nov, vertex, *image));
				delete image;
				return body;
			}
		}
		template <typename T> static Mesh*
		New(unsigned noi,
			const unsigned index[],
			unsigned nov,
			const T vertex[],
			const TB::Image& image) {
			if (auto* const vbo = TB::VBO::New(noi, index, nov, vertex)) {
				return New(vbo, image);
			}
			return 0;
		};
		static Mesh* New(TB::VBO*, const TB::Image&);
		~Mesh() { delete vbo; }

	protected:
		Mesh(TB::VBO* vbo, const TB::Image& image) : vbo(vbo), texture(image){};
		void DrawMesh();

	private:
		TB::VBO* const vbo;
		TB::Texture texture;
	};


	class Group : public Object {
	public:
		void AddChild(Object& o) { children.Add(o); };
		void AddCHild(Group& g) { groups.Add(g); };

	protected:
		TB::List<Object> children;
		TB::List<Object> groups;

	private:
		TB::Matrix<4, 4, float> matrix;

		void SetMatrix(const TB::Matrix<4, 4, float>& m) { matrix = m; }
		void MulMatrix(const TB::Matrix<4, 4, float>& m) {
			matrix = matrix * m;
		}
		void Draw() final;
		void Traw() final;
		void Tick() final;
	};

}
