/** ShereMap
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
#include <math.h>

#include <toolbox/gl/gl.h>
#include <toolbox/tg/scenery.h>



namespace TG {

	unsigned sphereMark;
	class SphereMap : public Scenery {
	public:
		SphereMap(TG::Mesh* mesh) : Scenery(mesh){};

		static Scenery* New(const TB::Image& image) {
			if (auto* const mesh = Mesh::New(
					(unsigned)(sizeof(buffers.ib) / sizeof(unsigned)),
					(unsigned*)buffers.ib,
					(unsigned)(sizeof(buffers.vb) / sizeof(TB::VBO::V_UV)),
					(TB::VBO::V_UV*)buffers.vb,
					image)) {
				return new SphereMap(mesh);
			}
			return 0;
		};
		static unsigned Score(const TB::Image& image) {
			const unsigned w(image.GetWidth());
			const unsigned h(image.GetHeight());
			if (!w || !h) {
				return 0;
			}
			const float r((1.0 * w) / (2.0 * h));
			const float d(1.0 - r);
			return 65536 / (d * d + 1);
		};

	private:
		static const int scale = 4500;
		static const unsigned hDivs = 64;
		static const unsigned vDivs = 32;

		static class Buffers {
		public:
			Buffers();
			unsigned ib[vDivs - 1][hDivs][6];
			TB::VBO::V_UV vb[vDivs][hDivs + 1]; //ループのu/v座標のため一つ多い
		} buffers;

		static Factory factory;
	};


	Scenery::Factory SphereMap::factory(SphereMap::New, SphereMap::Score);
	SphereMap::Buffers SphereMap::buffers;


	SphereMap::Buffers::Buffers() {
		auto& ib(buffers.ib);
		auto& vb(buffers.vb);

		//頂点生成
		for (unsigned v(0); v < vDivs; ++v) {
			const float tv((float)v / (vDivs - 1));
			const float ev(M_PI * tv - 0.5 * M_PI);
			const float evc(cosf(ev));
			const float y(sinf(ev) * scale);
			for (unsigned h(0); h <= hDivs; ++h) {
				const float th((float)h / hDivs);
				const float az(2 * M_PI * th);

				auto& target(vb[v][h]); //対象の頂点
				target.vertex.x = evc * cosf(az) * scale;
				target.vertex.z = evc * sinf(az) * scale;
				target.vertex.y = -y;
				target.texture.u = th;
				target.texture.v = tv;
			}
		}

		//インデックス生成
		for (unsigned v(0); v < vDivs - 1; ++v) {
			for (unsigned h(0); h < hDivs; ++h) {
				unsigned* buffers(ib[v][h]);
				buffers[0] = v * (hDivs + 1) + h;
				buffers[1] = (v + 1) * (hDivs + 1) + h;
				buffers[2] = (v + 1) * (hDivs + 1) + h + 1;
				buffers[3] = v * (hDivs + 1) + h;
				buffers[4] = (v + 1) * (hDivs + 1) + h + 1;
				buffers[5] = v * (hDivs + 1) + h + 1;
			}
		}
	}
}
