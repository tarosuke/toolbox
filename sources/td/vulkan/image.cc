/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2022 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/td/vulkan/image.h>
#include <toolbox/exception.h>

#include <string.h>



namespace TB {
	namespace VK {

		Image::Image(
			u32 width,
			u32 height,
			const void* data,
			VkFormat format,
			VkMemoryPropertyFlags propertyFlags)
			: image(MakeImage(width, height)),
			  imageMemory(width * height * 4, image, propertyFlags),
			  size(width * height * 4) {
			vkBindImageMemory(instance, image, imageMemory, 0);

			VkImageViewCreateInfo viewInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = image,
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = format,
				.subresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1},
			};
			Posit(!vkCreateImageView(instance, &viewInfo, nullptr, &imageView));

			// 初期化データがあるならそのデータで初期化
			if (data) {
				Store(data);
			}
		}

		Image::~Image() {
			vkDestroyImageView(instance, imageView, nullptr);
			vkDestroyImage(instance, image, 0);
		}

		void Image::Store(const void* data) {
			void* dest;
			Posit(!vkMapMemory(instance, imageMemory, 0, size, 0, &dest));
			memcpy(dest, data, size);
			vkUnmapMemory(instance, imageMemory);
		}



		VkImage Image::MakeImage(u32 width, u32 height) {
			VkImageCreateInfo imageInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.flags = 0, // SPARSEが可能
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_R8G8B8A8_SRGB,
				.extent{
					.width = width,
					.height = height,
					.depth = 1,
				},
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_LINEAR, // サンプルではOPTIMAL
				.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
						 VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};
			VkImage image;
			Posit(!vkCreateImage(instance, &imageInfo, nullptr, &image));
			return image;
		}
	}
}
