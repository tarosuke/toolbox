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
#pragma once
#include <toolbox/td/vulkan/instance.h>
#include <toolbox/td/vulkan/deviceMemory.h>
#include <toolbox/type.h>

#include <vulkan/vulkan.h>



namespace TB {
	namespace VK {

		struct Image {
			Image(
				u32 width,
				u32 height,
				const void* initialData,
				VkFormat,
				VkMemoryPropertyFlags,
				VkImageLayout);
			~Image();

			void Store(const void* data);

		private:
			Instance instance;
			VkImage image;
			DeviceMemory imageMemory;
			VkImageView imageView;
			const u32 size;

			VkImage MakeImage(u32, u32, VkImageLayout);
		};

		struct ColorImage : public Image {
			struct Def {
				u32 width;
				u32 height;
				const void* initialData;
			};

			ColorImage(u32 width, u32 height, const void* initialData)
				: Image(
					  width,
					  height,
					  initialData,
					  VK_FORMAT_R8G8B8A8_SRGB,
					  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
						  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
					  VK_IMAGE_LAYOUT_UNDEFINED){};
			ColorImage(const Def& def)
				: ColorImage(def.width, def.height, def.initialData){};
		};

		struct DepthStencilImage : public Image {
			DepthStencilImage(u32 width, u32 height)
				: Image(
					  width,
					  height,
					  0,
					  VK_FORMAT_D24_UNORM_S8_UINT,
					  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){};
			void Store(const void*) = delete; //ホストからは書かない
		};
	}
}
