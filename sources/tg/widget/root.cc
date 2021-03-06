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
#include <stdio.h>

#include <toolbox/tg/widget/root.h>
#include <toolbox/gl/gl.h>



namespace TG {

	const float RootWidget::scale(0.005); // mators per pixel
	const float RootWidget::virtualDistance(0.5); // small makes slow at center
	const float RootWidget::navigationRadious(1000);
	const float RootWidget::baseDepth(1);
	const float RootWidget::depthDiff(0.05);
	const TB::Rect<2, float> RootWidget::viewRect(
		TB::Vector<2, float>({-100, -100}), TB::Vector<2, float>({100, 100}));
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

		// 「前」をモデル系で回す
		TB::Vector<3, float> front(
			(const float[]){0.0f, 0.0f, virtualDistance});
		front = pose * front;

		//後ろ半分は使えないので角度を半分にする
		const float x(front[0]);
		const float y(front[1]);
		const float len2(sqrtf(x * x + y * y));
		const float a2(atan2f(len2, front[2]) * 0.5); //半分の角度
		const float l2(sinf(a2));
		const float z2(cosf(a2));

		const float distance(l2 / (z2 + 0.001));
		lookingPoint = {distance * x / len2, distance * y / len2};
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
			Find((const Query){lookingPoint, pointer, viewRect, 0}));

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
				trawCursor = DummyTrawCursor;
				(*found.widget)
					.AtPointerEnter((const PointerEvent){found.where, button});
			} else {
				// カーソルは「地べた」
				trawCursor = Cursor::Traw;
				Cursor::SetPosition(pointer);
			}
		} else if (moved) {
			// 単純移動
			if (found.widget) {
				// Move
				(*found.widget)
					.AtPointerMove((const PointerEvent){found.where, button});
			} else {
				// 「地べた」にいるカーソルの場所更新
				Cursor::SetPosition(pointer);
			}
		}

		//値の更新
		prev = found;
		button.Clear();
		moved = false;
	}


	void RootWidget::Bridge::Draw() {
		glPushMatrix();
		glTranslatef(root.lookingPoint[0], root.lookingPoint[1], 0.0f);
		glScalef(scale, scale, -1);
		root.Draw(viewRect);
		glPopMatrix();
	}
	void RootWidget::Bridge::Traw() {
		glPushMatrix();
		glTranslatef(root.lookingPoint[0], root.lookingPoint[1], 0.0f);
		glScalef(scale, scale, -1);
		root.Traw(viewRect);
		glTranslatef(0, 0, 1);
		(*trawCursor)(Cursor::none); //地べたカーソル描画
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
		moved = true;
	};
}
