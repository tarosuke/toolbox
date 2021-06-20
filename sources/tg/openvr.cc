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
#include <toolbox/gl/glx.h>
#include <toolbox/exception/exception.h>

#include <X11/X.h>



namespace {
	::Display* GetDisplay() {
		::Display* display(XOpenDisplay(""));

		/** デフォルトのビジュアルフォーマット
		 */
		static int attr[] = {
			GLX_USE_GL,
			GLX_LEVEL,
			0,
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE,
			8,
			GLX_GREEN_SIZE,
			8,
			GLX_BLUE_SIZE,
			8,
			GLX_ALPHA_SIZE,
			8,
			GLX_DEPTH_SIZE,
			24,
			GLX_STENCIL_SIZE,
			8,
			GLX_ACCUM_RED_SIZE,
			0,
			GLX_ACCUM_GREEN_SIZE,
			0,
			GLX_ACCUM_BLUE_SIZE,
			0,
			GLX_ACCUM_ALPHA_SIZE,
			0,
			None};
		XVisualInfo* visual(
			glXChooseVisual(display, DefaultScreen(display), attr));
		GLXContext context(glXCreateContext(display, visual, NULL, True));
		glXMakeCurrent(display, DefaultRootWindow(display), context);

		// glew初期化
		if (GLEW_OK != glewInit()) {
			throw "GLEWが使えません";
		}

		return display;
	}
}

namespace TG {

	OpenVR::OpenVR()
		: display(GetDisplay()), openVR(GetOpenVR()),
		  renderSize(GetRenderSize(openVR)),
		  eyes{
			  {openVR, vr::Eye_Right, renderSize},
			  {openVR, vr::Eye_Left, renderSize}} {

		//基本設定
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
	}
	OpenVR::~OpenVR() {
		XCloseDisplay((::Display*)display);
		vr::VR_Shutdown();
	}

	void OpenVR::Draw(const TB::Matrix<4, 4, float>& view) {
		//全デバイスの姿勢を取得
		vr::VRCompositor()
			->WaitGetPoses(devicePoses, vr::k_unMaxTrackedDeviceCount, NULL, 0);
		for (unsigned n(0); n < vr::k_unMaxTrackedDeviceCount; ++n) {
			if (devicePoses[n].bPoseIsValid) {
				switch (openVR.GetTrackedDeviceClass(n)) {
				case vr::TrackedDeviceClass_HMD:
					headPose = devicePoses[n].mDeviceToAbsoluteTracking;
					headPose.InvertAffine();
					break;
				default:
					break;
				}
			}
		}

		TG::Scene::SetHeadPose(headPose);

		for (auto& eye : eyes) {
			TB::Framebuffer::Key key(eye.framebuffer);
			glViewport(0, 0, renderSize.width, renderSize.height);

			glMatrixMode(GL_PROJECTION);
			glLoadTransposeMatrixf(&eye.projecionMatrix.m[0][0]);
			glMatrixMode(GL_MODELVIEW);
			Scene::Draw(eye.eye2HeadMatrix * headPose * view);
			vr::VRCompositor()->Submit(eye.side, &eye.fbFeature);
		}
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
		  eye2HeadMatrix(hmd.GetEyeToHeadTransform(eye)),
		  fbFeature((vr::Texture_t){
			  (void*)(uintptr_t)framebuffer.GetColorBufferID(),
			  vr::TextureType_OpenGL,
			  vr::ColorSpace_Gamma}) {
		eye2HeadMatrix.InvertAffine();
	}
}
