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

	};
}
