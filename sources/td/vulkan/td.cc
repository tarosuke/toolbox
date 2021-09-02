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
#include <toolbox/exception.h>

#include <vulkan/vulkan_xlib.h>
#include <stdexcept>



namespace TB {
	namespace VK {

		VkFramebuffer* FBTD::MakeFrameBuffer(const S2& s) { return 0; }

		VkFramebuffer* XFBTD::MakeFrameBuffer(const TB::X::Window& w) {
			auto attr(w.GetAttributes());
			auto instance(Instance::GetProperty());

			VkXlibSurfaceCreateInfoKHR sInfo{
				.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.dpy = ::TB::X::Display::xdisplay,
				.window = w.xwindow,
			};
			if (const VkResult r = vkCreateXlibSurfaceKHR(
					instance.instance,
					&sInfo,
					nullptr,
					&surface)) {
				THROW;
			}

			VkBool32 bSupportsPresent(VK_FALSE);
			if (const VkResult r = vkGetPhysicalDeviceSurfaceSupportKHR(
					instance.physicalDevice,
					instance.presentFamilyIndex,
					surface,
					&bSupportsPresent)) {
				THROW;
			}
			if (bSupportsPresent == VK_FALSE) {
				THROW;
			}



			if (const VkResult r = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
					instance.physicalDevice,
					surface,
					&capabilities)) {
				THROW;
			}

			uint32_t nFormat;
			if (const VkResult r = vkGetPhysicalDeviceSurfaceFormatsKHR(
					instance.physicalDevice,
					surface,
					&nFormat,
					nullptr)) {
				THROW;
			}
			if (nFormat) {
				formats.resize(nFormat);
				vkGetPhysicalDeviceSurfaceFormatsKHR(
					instance.physicalDevice,
					surface,
					&nFormat,
					formats.data());
			}



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
					VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, // TODO:あとでデフォルトを取得して差し替える(でないと画面を回しているときに画面と合わないことが予想される)
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = VK_PRESENT_MODE_FIFO_KHR,
				.clipped = VK_TRUE,
				.oldSwapchain = VK_NULL_HANDLE,
			};


			if (const VkResult r =
					vkCreateSwapchainKHR(instance.device, &sc, 0, &swapchain)) {
				THROW;
			}

			return 0;
		}

		XFBTD::~XFBTD() {
			auto instance(Instance::GetProperty());
			vkDestroySwapchainKHR(instance.device, swapchain, 0);
			vkDestroySurfaceKHR(instance.instance, surface, 0);
		}

		Instance::Extension
			XFBTD::extensionKey(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
	}
}
