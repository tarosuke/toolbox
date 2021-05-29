/** Skybox
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
 */

#include <assert.h>
#include <syslog.h>

#include <toolbox/tg/scenery.h>
#include <toolbox/type.h>



namespace TG {

	unsigned skyboxMark;
	class Skybox : public Scenery {
	public:
	private:
		Skybox(TG::Mesh* mesh) : Scenery(mesh) {}

		static Scenery* New(const TB::Image&);
		static unsigned Score(const TB::Image& image) {
			const unsigned w(image.GetWidth());
			const unsigned h(image.GetHeight());
			if (!w || !h) {
				return 0;
			}
			const float r((3.0 * w) / (4.0 * h));
			const float d(1.0 - r);
			return 65536 / (d * d + 1);
		};

		static const int scale = 4500;
		static Factory factory;
		static unsigned indexes[];
		static TB::VBO::V_UV vertexes[];
	};


	Scenery::Factory Skybox::factory(Skybox::New, Skybox::Score);

	unsigned Skybox::indexes[] = {
		0,	2,	1, 2,  3,  1, // left
		2,	4,	3, 4,  5,  3, // front
		4,	6,	5, 6,  7,  5, // right
		6,	8,	7, 8,  9,  7, // rear
		2,	10, 4, 10, 11, 4, // top
		12, 13, 9, 13, 7,  9 // bottom
	};

	TB::VBO::V_UV Skybox::vertexes[] = {
		// 0:左後方
		{{-scale, scale, scale}, {0.0 / 4, 1.0 / 3}},
		{{-scale, -scale, scale}, {0.0 / 4, 2.0 / 3}},
		// 2:左前方
		{{-scale, scale, -scale}, {1.0 / 4, 1.0 / 3}},
		{{-scale, -scale, -scale}, {1.0 / 4, 2.0 / 3}},
		// 4:右前方
		{{scale, scale, -scale}, {2.0 / 4, 1.0 / 3}},
		{{scale, -scale, -scale}, {2.0 / 4, 2.0 / 3}},
		// 6:右後方
		{{scale, scale, scale}, {3.0 / 4, 1.0 / 3}},
		{{scale, -scale, scale}, {3.0 / 4, 2.0 / 3}},
		// 8:左後方(後方テクスチャ用)
		{{-scale, scale, scale}, {4.0 / 4, 1.0 / 3}},
		{{-scale, -scale, scale}, {4.0 / 4, 2.0 / 3}},
		// 10:上後方左右
		{{-scale, scale, scale}, {1.0 / 4, 0.0 / 3}},
		{{scale, scale, scale}, {2.0 / 4, 0.0 / 3}},
		// 12:下後方左右
		{{-scale, -scale, -scale}, {4.0 / 4, 3.0 / 3}},
		{{scale, -scale, -scale}, {3.0 / 4, 3.0 / 3}},
	};

	Scenery* Skybox::New(const TB::Image& image) {
		{
			//スカイボックス画像の準備
			TB::Image::Raw raw(image);
			const unsigned w(raw.width / 4);
			const unsigned w2(raw.width / 2);
			const unsigned w3(raw.width * 3 / 4);
			const unsigned h(raw.height / 3);
			const unsigned h2(raw.height * 2 / 3);
			//底移動反転
			for (unsigned y(0); y < h; ++y) {
				for (unsigned x(0); x < w; ++x) {
					raw[h2 + y][w3 + x] = raw[raw.height - 1 - y][w2 - 1 - x];
				}
			}
			//エッジの対面を転送(転送先が横)
			for (unsigned x(0); x < w; ++x) {
				const unsigned y(x * h / w);
				raw[0][w3 + x] = raw[h2 - 1][w2 - 1 - x];
				raw[h - 1][x] = raw[w][y];
				raw[h - 1][w2 + x] = raw[h - 1 - y][w2 - 1];
				raw[h - 1][w3 + x] = raw[0][w2 - 1 - x];
				raw[h2][x] = raw[raw.height - 1 - y][w];
				raw[h2][w2 + x] = raw[h2 + x][w2 - 1];
			}
			//エッジの対面を転送(転送先が縦)
			for (unsigned y(0); y < h; ++y) {
				const unsigned x(y * w / h);
				raw[h2 + y][0] = raw[h2 - 1][x];
				raw[y][w - 1] = raw[h][x];
				raw[h2 + y][w - 1] = raw[h2 - 1][w - 1 - x];
				raw[y][w2] = raw[h2 - 1][w3 - 1 - x];
				raw[h2 + y][w2] = raw[h2 - 1][w2 + x];
				raw[h2 + y][w3 - 1] = raw[h2 - 1][w3 - 1 - x];
			}
		}

		if (auto* const mesh = Mesh::New(
				elementsOf(indexes),
				indexes,
				elementsOf(vertexes),
				vertexes,
				image)) {
			return new Skybox(mesh);
		}
		return 0;
	};
}
