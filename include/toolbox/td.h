/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/container/list.h>
#include <toolbox/geometry/matrix.h>
#include <toolbox/geometry/spread.h>
#include <toolbox/type.h>
#include <toolbox/time.h>



namespace TB {

	struct TD {
		using M44 = Matrix<4, 4, float>;
		using S2 = Spread<2, unsigned>;

		static M44 Frustum(float width, float height, float near, float far);

		// タイムスタンプ
		struct Timestamp {
			Timestamp() : delta(1){};
			nsec uptime;
			nsec delta;
		};

	protected:
		const M44 projectile;

		Timestamp timestamp;
		bool keep;
		bool redraw;

		TD() = delete;
		TD(const M44& p) : projectile(p), redraw(false){};
		virtual ~TD(){};
		void Quit() { keep = false; };

		// 周回処理中の定形処理
		virtual void Tick(const Timestamp&){}; // 描画以外の準備
		virtual void Draw(const M44& view) = 0; // 描画、出力

	private:
	};
}
