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
#include <string.h>



namespace TB {
	namespace VK {

		VkFramebuffer* FBTD::MakeFrameBuffer(const S2& s) { return 0; }

		VkFramebuffer* XFBTD::MakeFrameBuffer(const TB::X::Window& w) {
			auto attr(w.GetAttributes());
			auto instance(Instance::GetInstance());

			const VkXlibSurfaceCreateInfoKHR sInfo{
				.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.dpy = ::TB::X::Display::xdisplay,
				.window = w.xwindow,
			};
			Posit(!vkCreateXlibSurfaceKHR(
				instance.instance,
				&sInfo,
				nullptr,
				&surface));

			VkBool32 bSupportsPresent = VK_FALSE;
			Posit(!vkGetPhysicalDeviceSurfaceSupportKHR(
				instance.physicalDevice,
				instance.presentFamilyIndex,
				surface,
				&bSupportsPresent));
			Posit(bSupportsPresent);

			const VkSwapchainCreateInfoKHR sc{
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.surface = surface,
				.minImageCount = 2,
				.imageFormat = format = VK_FORMAT_B8G8R8A8_SRGB,
				.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
				.imageExtent =
					extent = {(unsigned)attr.width, (unsigned)attr.height},
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

			Posit(!vkCreateSwapchainKHR(instance.device, &sc, 0, &swapchain));

			unsigned numImages;
			Posit(!vkGetSwapchainImagesKHR(
				instance.device,
				swapchain,
				&numImages,
				nullptr));
			swapchainImages.resize(numImages);
			Posit(!vkGetSwapchainImagesKHR(
				instance.device,
				swapchain,
				&numImages,
				swapchainImages.data()));


			// prepare swapchainImageView
			swapchainImageViews.resize(swapchainImages.size());
			for (unsigned n(0); n < swapchainImageViews.size(); ++n) {
				VkImageViewCreateInfo cInfo{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.image = swapchainImages[n],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = format,
					.components{
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY,
					},
					.subresourceRange{
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					},
				};
				Posit(!vkCreateImageView(
					instance.device,
					&cInfo,
					nullptr,
					&swapchainImageViews[n]));
			}
			return 0;
		}

		XFBTD::~XFBTD() {
			auto instance(Instance::GetInstance());
			for (auto imageView : swapchainImageViews) {
				vkDestroyImageView(instance.device, imageView, nullptr);
			}
			vkDestroySwapchainKHR(instance.device, swapchain, 0);
			vkDestroySurfaceKHR(instance.instance, surface, 0);
		}

		Instance::Extension<VkInstance> XFBTD::instanceExtensions(
			{VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
			 VK_KHR_SURFACE_EXTENSION_NAME});
		Instance::Extension<VkDevice>
			XFBTD::driverExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
	}
}
