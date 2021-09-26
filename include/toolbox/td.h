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


		// 描画物(頂点バッファ＋テクスチャ)
		struct Object : public List<Object>::Node {
			virtual void Draw(/*未定*/) = 0;
			virtual bool IsTransparent() { return true; };
		};

		// タイムスタンプ
		struct Timestamp {
			Timestamp() : delta(1){};
			nsec uptime;
			nsec delta;
		};

		void AddHead(Object& o) { head.Add(o); };
		void AddExternal(Object& o) { external.Add(o); };
		void AddScenery(Object& o) { scenery.Add(o); };


		void Run();


	protected:
		M44 view;

		TD(){};
		virtual ~TD(){};
		void Quit() { keep = false; };

		// 周回処理中の定形処理
		virtual void Prepare(){}; // 描画先の準備
		virtual void Tick(const Timestamp&){}; // 描画以外の準備
		virtual void Draw() = 0; //コマンドバッファ更新
		virtual void Draw(const M44& view) = 0; // 描画、出力

		// オブジェクトの再描画

	private:
		bool keep;

		Timestamp timestamp;

		struct Target {
			Target() : modified(false){};
			void Add(Object& o) {
				(o.IsTransparent() ? transparent : opaque).Add(o);
				modified = true;
			};
			void DrawOpaque() { opaque.Foreach(&Object::Draw); };
			void DrawTransparent() { transparent.Reveach(&Object::Draw); };

		private:
			List<Object> opaque;
			List<Object> transparent;
			bool modified; //コマンド再生成が必要
		} head, external, scenery;
	};
}
