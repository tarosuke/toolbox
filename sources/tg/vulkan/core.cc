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
#include <toolbox/tg/vulkan.h>



namespace TB {
	namespace VK {

		Canvas::Canvas(Image& baseImage, VkFormat depthFormat)
			: colorBuffer(baseImage), depthBuffer(
										  baseImage.Width(),
										  baseImage.Height(),
										  Image::depthBufferImageInfo) {
			MakeImageView(&views[0], colorBuffer);
			MakeImageView(&views[1], depthBuffer);
			MakeRenderPass(&renderPass, (VkFormat)baseImage);

			const VkFramebufferCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.pNext = NULL,
				.flags = 0,
				.renderPass = renderPass,
				.attachmentCount = 2,
				.pAttachments = views,
				.width = baseImage.Width(),
				.height = baseImage.Height(),
				.layers = 1};
			if (vkCreateFramebuffer(device, &info, NULL, &framebuffer) !=
				VK_SUCCESS) {
				throw -1;
			}
		};

		Canvas::~Canvas() {
			vkDestroyRenderPass(device, renderPass, nullptr);
			vkDestroyFramebuffer(device, framebuffer, NULL);
			vkDestroyImageView(device, views[0], NULL);
			vkDestroyImageView(device, views[1], NULL);
		}



		void Canvas::MakeImageView(VkImageView* imageView, Image& image) {
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

		void Canvas::MakeRenderPass(VkRenderPass* renderPass, VkFormat format) {
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
					renderPass) != VK_SUCCESS) {
				throw -1;
			}
		}
	}
}
