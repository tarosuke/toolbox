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

#include <openvr/openvr.h>

#include <toolbox/tg/tg.h>
#include <toolbox/gl/framebuffer.h>



namespace TG {

	class OpenVR : public Scene {
	public:
		struct Exception {
			const char* message;
			vr::HmdError code;
		};

		OpenVR();
		~OpenVR();

	private:
		void* display;
		struct Pose : public TB::Matrix<4, 4> {
			Pose(){};
			Pose(const vr::HmdMatrix44_t& o) { *this = o; };
			Pose(const vr::HmdMatrix34_t& o) { *this = o; };
			void operator=(const vr::HmdMatrix44_t& o) { Transpose(o.m); };
			void operator=(const vr::HmdMatrix34_t& o) {
				TransposeAffine(o.m);
			};
		};

		static constexpr float nearClip = 0.01;
		static constexpr float farClip = 10000;
		static vr::TrackedDevicePose_t devicePoses[];
		vr::IVRSystem& openVR;

		//フレームバッファ他
		TB::Framebuffer::Size renderSize;
		struct Eye {
			Eye(vr::IVRSystem&, vr::EVREye, TB::Framebuffer::Size&);
			const vr::EVREye side;
			TB::Framebuffer framebuffer;
			vr::HmdMatrix44_t projecionMatrix;
			Pose eye2HeadMatrix;
			vr::Texture_t fbFeature;
		} eyes[2];

		//初期化サポート
		static vr::IVRSystem& GetOpenVR(); //失敗したら例外
		static TB::Framebuffer::Size GetRenderSize(vr::IVRSystem&);

		void Draw(const TB::Matrix<4, 4, float>&) final;
	};
}
