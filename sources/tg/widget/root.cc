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
#include <float.h>
#include <math.h>

#include <toolbox/tg/widget/root.h>
#include <toolbox/gl/gl.h>



namespace TG {

	const float RootWidget::navigationRadious(1000);
	const TB::Rect<2, float> RootWidget::viewRect(
		TB::Vector<2, float>({-1, -1}), TB::Vector<2, float>({1, 1}));
	Cursor::TrawHandler RootWidget::trawCursor(DummyTrawCursor);


	void RootWidget::Tick() {
		//注視点計算
		CalcLoockingPoint();

		// sub窓へ処理を渡す
		Widget::Tick();

		EmitEvent();
	}

	void RootWidget::CalcLoockingPoint() {
		const auto& pose(Scene::GetHeadPose());

		TB::Matrix<1, 4, float> front((const float[]){0.0f, 0.0f, 1.0f, 0.0f});
		TB::Matrix<1, 4, float> point(pose * front);

		//後ろ半分は使えないので角度を半分にする
		const float x(point[0][0]);
		const float y(point[0][1]);
		const float len2(sqrtf(x * x + y * y));
		const float a2(atan2f(len2, point[0][2]) * 0.5); //半分の角度
		const float l2(sinf(a2));
		const float z2(cosf(a2));

		static const float offset(0.01);
		lookingPoint = {
			(x * l2) / (z2 * len2 + offset),
			(y * l2) / (z2 * len2 + offset)};
	}
	void RootWidget::EmitEvent() {
		//入力状態取得
		GetInput();

		//ポインタが画面外に出るのを防ぐ
		TB::Vector<2, float> pointerPosition(pointer - lookingPoint);
		if (navigationRadious * navigationRadious < pointerPosition.Length2()) {
			pointerPosition.Normalize();
			pointerPosition *= navigationRadious;
		}

		//ポインタがある窓を探す
		auto const found(
			Find((const Query){lookingPoint, pointer, viewRect, FLT_MAX}));

		//ボタンイベント
		if (found.widget && (button.pressed || button.released)) {
			(*found.widget).OnButton((const PointerEvent){found.where, button});
		}

		// ポインタ移動イベント
		if (found.widget != prev.widget) {
			// Enter/Leave
			if (prev.widget) {
				// Leave
				(*prev.widget)
					.AtPointerLeave((const PointerEvent){prev.where, button});
			}
			if (found.widget) {
				// Enter
				(*found.widget)
					.AtPointerEnter((const PointerEvent){found.where, button});
			} else {
				// カーソルは「地べた」
				trawCursor = Cursor::Traw;
				Cursor::SetPosition(pointer);
			}
		} else if (prev.where != found.where) {
			// 単純移動
			if (found.widget) {
				// Move
				(*found.widget)
					.AtPointerMove((const PointerEvent){found.where, button});
			} else {
				// カーソルの場所更新
				Cursor::SetPosition(pointer);
			}
		}

		//値の更新
		prev = found;
		button.Clear();
	}


	void RootWidget::Bridge::Draw() {
		glPushMatrix();
		glScalef(1, 1, -1);
		glTranslatef(root.lookingPoint[0], root.lookingPoint[1], 0);
		root.Draw(viewRect);
		glPopMatrix();
	}
	void RootWidget::Bridge::Traw() {
		glPushMatrix();
		glScalef(1, 1, -1);
		glTranslatef(root.lookingPoint[0], root.lookingPoint[1], 0);
		root.Traw(viewRect);
		(*trawCursor)(Cursor::none);
		glPopMatrix();
	}


	// inputデバイスからの入力
	void RootWidget::OnKeyDown(unsigned key){};
	void RootWidget::OnKeyUp(unsigned key){};
	void RootWidget::OnKeyRepeat(unsigned key){};
	void RootWidget::OnButtonDown(unsigned b) {
		const unsigned p(1U << b);
		button.pressed |= p;
		button.state |= p;
	};
	void RootWidget::OnButtonUp(unsigned b) {
		const unsigned p(1U << b);
		button.released |= p;
		button.state &= ~p;
	};
	void RootWidget::OnMouseMove(unsigned axis, int diff) {
		if (axis < 2) {
			pointer[axis] += diff;
		}
	};
}
