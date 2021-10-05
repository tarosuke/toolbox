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


		XTD::XTD(
			unsigned width,
			unsigned height,
			const M44& proj,
			const Shaders* shaders)
			: TD(proj, shaders), window(width, height) {
			auto attr(window.GetAttributes());

			const VkXlibSurfaceCreateInfoKHR sInfo{
				.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.dpy = ::TB::X::Display::xdisplay,
				.window = window.xwindow,
			};
			Posit(!vkCreateXlibSurfaceKHR(instance, &sInfo, nullptr, &surface));

			VkBool32 bSupportsPresent(VK_FALSE);
			Posit(!vkGetPhysicalDeviceSurfaceSupportKHR(
				instance,
				instance.PhysicalFamilyIndex(),
				surface,
				&bSupportsPresent));
			Posit(bSupportsPresent);

			Posit(!vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
				instance,
				surface,
				&capabilities));

			const VkSwapchainCreateInfoKHR sc{
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.pNext = 0,
				.flags = 0,
				.surface = surface,
				.minImageCount = capabilities.minImageCount,
				.imageFormat = format = VK_FORMAT_B8G8R8A8_SRGB,
				.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
				.imageExtent =
					extent = {(unsigned)attr.width, (unsigned)attr.height},
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = 0,
				.preTransform = capabilities.currentTransform,
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = VK_PRESENT_MODE_FIFO_KHR,
				.clipped = VK_TRUE,
				.oldSwapchain = VK_NULL_HANDLE,
			};

			Posit(!vkCreateSwapchainKHR(instance, &sc, 0, &swapchain));

			unsigned numImages(sc.minImageCount);
			Posit(!vkGetSwapchainImagesKHR(
				instance,
				swapchain,
				&numImages,
				nullptr));
			swapchainImages.resize(numImages);
			Posit(!vkGetSwapchainImagesKHR(
				instance,
				swapchain,
				&numImages,
				swapchainImages.data()));


			// prepare swapchainImageView
			swapchainImageViews.resize(swapchainImages.size());
			for (unsigned n(0); n < swapchainImageViews.size(); ++n) {
				VkImageViewCreateInfo cInfo{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext = 0,
					.flags = 0,
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
					instance,
					&cInfo,
					nullptr,
					&swapchainImageViews[n]));
			}

			framebuffers.resize(swapchainImageViews.size());
			for (unsigned n(0); n < framebuffers.size(); ++n) {
				VkImageView attachments[] = {swapchainImageViews[n]};

				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType =
					VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = extent.width;
				framebufferInfo.height = extent.height;
				framebufferInfo.layers = 1;

				Posit(!vkCreateFramebuffer(
					instance,
					&framebufferInfo,
					nullptr,
					&framebuffers[n]));
			}

			Init();
		}

		VkFramebuffer XTD::NextFramebuffer() {
			// imageIndex, commandIndexを更新
			vkAcquireNextImageKHR(
				instance,
				swapchain,
				UINT64_MAX,
				imageAvailableSemaphore,
				VK_NULL_HANDLE,
				&frameIndex);
			return framebuffers[frameIndex];
		}

		void XTD::Draw(const M44& view) {
			// 描画
			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
			VkPipelineStageFlags waitStages[] = {
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;
			VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;
			Posit(!vkQueueSubmit(instance, 1, &submitInfo, VK_NULL_HANDLE));

			// 表示
			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 0;
			presentInfo.pWaitSemaphores = nullptr;

			VkSwapchainKHR swapChains[] = {swapchain};
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &frameIndex;

			presentInfo.pResults = nullptr; // Optional

			Posit(!vkQueuePresentKHR(instance, &presentInfo));
			Posit(!vkQueueWaitIdle(instance));
		}

		XTD::~XTD() {
			vkDeviceWaitIdle(instance);

			for (auto f : framebuffers) {
				vkDestroyFramebuffer(instance, f, nullptr);
			}
			for (auto imageView : swapchainImageViews) {
				vkDestroyImageView(instance, imageView, nullptr);
			}
			vkDestroySwapchainKHR(instance, swapchain, 0);
			vkDestroySurfaceKHR(instance, surface, 0);
		}

		Base::Extension<VkInstance> XTD::instanceExtensions(
			{VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
			 VK_KHR_SURFACE_EXTENSION_NAME});
		Base::Extension<VkDevice>
			XTD::driverExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
	}
}
