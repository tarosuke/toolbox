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
#include <toolbox/tg/widget/root.h>



namespace TG {
	RootWidget::RootWidget() { Scene::RegisterRoot(*this); }
	void RootWidget::Tick() {
		//注視点計算
		const auto& pose(Scene::GetHeadPose());

		TB::Matrix<1, 4> front((const float[]){0.0f, 0.0f, 1.0f, 0.0f});
		TB::Matrix<1, 4> point(pose * front);

		//後ろ半分は使えないので角度を半分にする
		const float x(point[0][0]);
		const float y(point[0][1]);
		const float len2(sqrtf(x * x + y * y));
		const float a2(atan2f(len2, point[0][2]) * 0.5); //半分の角度
		const float l2(sinf(a2));
		const float z2(cosf(a2));

		const float lp[] = {(x * l2) / (z2 * len2), (y * l2) / (z2 * len2)};
		lookingPoint = lp;

		// sub窓へ処理を渡す
		Scene::Object::Tick();
	}
}
