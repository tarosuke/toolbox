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
#include <toolbox/tg/openvr.h>
#include <toolbox/gl/gl.h>
#include <toolbox/exception/exception.h>



namespace TG {

	OpenVR::OpenVR()
		: openVR(GetOpenVR()), renderSize(GetRenderSize(openVR)),
		  eyes{
			  {openVR, vr::Eye_Left, renderSize},
			  {openVR, vr::Eye_Right, renderSize}} {

		//基本設定
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
	}
	OpenVR::~OpenVR() { vr::VR_Shutdown(); }

	void OpenVR::Draw(const TB::Matrix<4, 4, float>& view) {
		for (auto& eye : eyes) {
			TB::Framebuffer::Key key(eye.framebuffer);
			glViewport(0, 0, renderSize.width, renderSize.height);

			glMatrixMode(GL_PROJECTION);
			glLoadTransposeMatrixf(&eye.projecionMatrix.m[0][0]);
			glMatrixMode(GL_MODELVIEW);
			Scene::Draw(eye.eye2HeadMatrix);
		}
	}

	bool OpenVR::Finish() {
		static unsigned n(0);
		return ++n < 450;
	}

	vr::IVRSystem& OpenVR::GetOpenVR() {
		Exception exception = {"Failed to initialize OpenVR"};
		if (vr::IVRSystem* const o =
				vr::VR_Init(&exception.code, vr::VRApplication_Scene)) {
			return *o;
		}
		throw exception;
	}

	TB::Framebuffer::Size OpenVR::GetRenderSize(vr::IVRSystem& o) {
		TB::Framebuffer::Size size;
		o.GetRecommendedRenderTargetSize(&size.width, &size.height);
		return size;
	}

	//
	//片目分
	//
	OpenVR::Eye::Eye(
		vr::IVRSystem& hmd, vr::EVREye eye, TB::Framebuffer::Size& size)
		: side(eye), framebuffer(size),
		  projecionMatrix(hmd.GetProjectionMatrix(side, nearClip, farClip)),
		  eye2HeadMatrix(hmd.GetEyeToHeadTransform(side)),
		  fbFeature((vr::Texture_t){
			  (void*)(uintptr_t)framebuffer.GetColorBufferID(),
			  vr::TextureType_OpenGL,
			  vr::ColorSpace_Gamma}) {}
}
