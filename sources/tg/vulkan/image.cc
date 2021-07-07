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
#include <toolbox/tg/vulkan/image.h>



namespace TB {
	namespace VK {

		// 代表的な設定
		const VkImageCreateInfo Image::colorBufferImageInfo{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_B8G8R8_UINT,
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_LINEAR,
			.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

		const VkImageCreateInfo Image::depthBufferImageInfo{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_D24_UNORM_S8_UINT,
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_LINEAR,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.initialLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL};

		Image::Image(
			unsigned width,
			unsigned height,
			const VkImageCreateInfo& createInfo)
			: info(createInfo) {
			info.extent = {.width = width, .height = height, .depth = 1};

			if (vkCreateImage(device, &info, NULL, &image) != VK_SUCCESS) {
				throw 1;
			};
		}

		Image::~Image() { vkDestroyImage(device, image, NULL); }



		FrameBuffer::FrameBuffer(
			unsigned width,
			unsigned height,
			VkFormat colorFormat,
			VkFormat depthFormat)
			: colorBuffer(width, height, Image::colorBufferImageInfo),
			  depthBuffer(width, height, Image::depthBufferImageInfo),
			  colorPass(colorFormat) {
			MakeImageView(colorBuffer, &views[0]);
			MakeImageView(depthBuffer, &views[1]);

			const VkFramebufferCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.pNext = NULL,
				.flags = 0,
				.renderPass = colorPass,
				.attachmentCount = 2,
				.pAttachments = views,
				.width = width,
				.height = height,
				.layers = 1};
			if (vkCreateFramebuffer(device, &info, NULL, &framebuffer) !=
				VK_SUCCESS) {
				throw -1;
			}
		}

		FrameBuffer::~FrameBuffer() {
			vkDestroyFramebuffer(device, framebuffer, NULL);
			vkDestroyImageView(device, views[0], NULL);
			vkDestroyImageView(device, views[1], NULL);
		}

		void FrameBuffer::MakeImageView(Image& image, VkImageView* imageView) {
			const VkImageViewCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = 0,
				.image = image,
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = image,
				.components =
					{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					 .g = VK_COMPONENT_SWIZZLE_IDENTITY,
					 .b = VK_COMPONENT_SWIZZLE_IDENTITY,
					 .a = VK_COMPONENT_SWIZZLE_IDENTITY},
				.subresourceRange = {
					.aspectMask = (VkImageUsageFlags)image ==
										  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
									? VK_IMAGE_ASPECT_COLOR_BIT
									: VK_IMAGE_ASPECT_DEPTH_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1}};
			if (vkCreateImageView(device, &info, NULL, imageView) !=
				VK_SUCCESS) {
				throw -1;
			}
		}


		RenderPass::RenderPass(VkFormat format) {
			VkAttachmentDescription colorAttachment{
				.format = format,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL};
			VkAttachmentReference colorAttachmentRef{
				.attachment = 0,
				.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
			VkSubpassDescription subpass{
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.colorAttachmentCount = 1,
				.pColorAttachments = &colorAttachmentRef};
			VkRenderPassCreateInfo renderPassInfo{
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.attachmentCount = 1,
				.pAttachments = &colorAttachment,
				.subpassCount = 1,
				.pSubpasses = &subpass};

			if (vkCreateRenderPass(
					device,
					&renderPassInfo,
					nullptr,
					&renderPass) != VK_SUCCESS) {
				throw -1;
			}
		}
		RenderPass::~RenderPass() {
			// vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
			vkDestroyRenderPass(device, renderPass, nullptr);
		}
	}
}
