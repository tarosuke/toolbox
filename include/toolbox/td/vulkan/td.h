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
#include <toolbox/type.h>
#include <toolbox/td.h>
#include <toolbox/td/x.h>
#include <toolbox/td/vulkan/instance.h>

#include <vulkan/vulkan.h>


namespace TB {
	namespace VK {

		// 初期化にフレームバッファが必要なVulkan版基本クラス
		struct TD : public TB::TD {
		protected:
			Instance instance;
			VkFramebuffer frameBuffer;
			TD(const M44& proj) : ::TB::TD(proj){};
		};

		// フレームバッファ版TD
		struct FBTD : public TD {
			FBTD(const M44& proj, const S2& viewport) : TD(proj){};

		private:
			static VkFramebuffer* MakeFrameBuffer(const S2&);
		};

		// X窓フレームバッファ版TD
		struct XFBTD : public TD {
			XFBTD(unsigned width, unsigned height, const M44& proj);
			~XFBTD();

		private:
			static Base::Extension<VkInstance> instanceExtensions;
			static Base::Extension<VkDevice> driverExtensions;

			::TB::X::Window window;
			VkSurfaceKHR surface;
			VkSwapchainKHR swapchain;
			std::vector<VkImage> swapchainImages;
			VkFormat format;
			VkExtent2D extent;
			std::vector<VkImageView> swapchainImageViews;
			VkSurfaceCapabilitiesKHR capabilities;

			VkFramebuffer* MakeFrameBuffer(const TB::X::Window&);
		};



		struct Shader {
			Shader(const u32*, const u32*);
			Shader(const char* path);
			~Shader();

			operator VkShaderModule() { return shaderModule; };

		private:
			Instance instance;
			VkShaderModule shaderModule;
		};
	}
}


/** SHADER SYMBOL MEMO
 * シェーダの.spvファイルがDEBUG/td/vulkan/test.frag.spvだとすると
 * シェーダのバイナリは_binary_DEBUG_td_vulkan_test_frag_spv_startのシンボルから
 * 始まっていて、同endで終わり。サイズは同sizeに格納されている。
 * DEBUGの部分はmakefileにて_BUILD_TARGET_マクロとして設定されているので
 * ソース上ではビルドターゲットに関わらず
 *  _binary_##_BUILD_TARGET_##_td_vulkan_test_frag_spv_start
 * になる。ただしトークン結合演算子のほうが強いのでマクロ引数にして展開させる
 *
 * 下のマクロは_td_vulkan_test_frag_spvの部分を与えて全体を作るものである
 */
#define __ShaderName(target, path, postfix) _binary_##target##_##path##postfix
#define _ShaderName(target, path, postfix) __ShaderName(target, path, postfix)
#define ShaderStart(path) _ShaderName(_BUILD_TARGET_, path, _start)
#define ShaderEnd(path) _ShaderName(_BUILD_TARGET_, path, _end)
