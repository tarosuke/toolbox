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
										  Image::depthBufferImageInfo),
			  renderPass((VkFormat)baseImage) {
			MakeImageView(&views[0], colorBuffer);
			MakeImageView(&views[1], depthBuffer);
			MakeFrameBuffer();
		}

		Canvas::~Canvas() {
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


		void Canvas::MakeFrameBuffer() {
			const VkFramebufferCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.pNext = NULL,
				.flags = 0,
				.renderPass = renderPass,
				.attachmentCount = 2,
				.pAttachments = views,
				.width = colorBuffer.Width(),
				.height = colorBuffer.Height(),
				.layers = 1};
			if (vkCreateFramebuffer(device, &info, NULL, &framebuffer) !=
				VK_SUCCESS) {
				throw -1;
			}
		}
	}
}
