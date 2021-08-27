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
#include <toolbox/td/vulkan/td.h>
#include <toolbox/td/vulkan/instance.h>

#include <vulkan/vulkan_xlib.h>



namespace TB {
	namespace VK {

		VkFramebuffer* FBTD::MakeFrameBuffer(const S2& s) { return 0; }

		VkFramebuffer* XFBTD::MakeFrameBuffer(const TB::X::Window& w) {
			auto attr(w.GetAttributes());


			VkXlibSurfaceCreateInfoKHR sInfo{
				.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.dpy = ::TB::X::Display::xdisplay,
				.window = w.xwindow,
			};
			VkSurfaceKHR surface;
			vkCreateXlibSurfaceKHR(
				Instance::GetInstance(),
				&sInfo,
				0,
				&surface);

			VkSwapchainCreateInfoKHR sc{
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.surface = surface,
				.minImageCount = 2,
				.imageFormat = VK_FORMAT_B8G8R8A8_SRGB,
				.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
				.imageExtent = {(unsigned)attr.width, (unsigned)attr.height},
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = 0,
				.preTransform =
					VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, // TODO:あとでデフォルトを取得して差し替える
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = VK_PRESENT_MODE_FIFO_KHR,
				.clipped = VK_TRUE,
				.oldSwapchain = VK_NULL_HANDLE,
			};


			// vkCreateSwapchainKHR

			return 0;
		}
	}
}
