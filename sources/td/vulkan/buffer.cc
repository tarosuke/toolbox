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
#include <toolbox/exception.h>
#include <toolbox/td/vulkan/buffer.h>

#include <string.h>



namespace TB {
	namespace VK {


		unsigned Buffer::FindMemoryType(
			unsigned filter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(instance, &memProperties);
			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if (filter & (1 << i) &&
					(memProperties.memoryTypes[i].propertyFlags & properties) ==
						properties) {
					return i;
				}
			}
			Posit(false);
		}


		Buffer::Buffer(
			const void* orgData,
			unsigned size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags propertyFlags,
			VkSharingMode shareMode) {

			VkBufferCreateInfo bufferInfo{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.flags = 0,
				.size = size,
				.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				.sharingMode = shareMode};

			Posit(!vkCreateBuffer(instance, &bufferInfo, nullptr, &buffer));

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(instance, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = size,
				.memoryTypeIndex = FindMemoryType(
					memRequirements.memoryTypeBits,
					propertyFlags)};
			Posit(!vkAllocateMemory(
				instance,
				&allocInfo,
				nullptr,
				&deviceMemory));
			Posit(!vkBindBufferMemory(instance, buffer, deviceMemory, 0));

			// 頂点転送
			void* data;
			Posit(!vkMapMemory(
				instance,
				deviceMemory,
				0,
				bufferInfo.size,
				0,
				&data));
			memcpy(data, orgData, (size_t)bufferInfo.size);
			vkUnmapMemory(instance, deviceMemory);
		}

		Buffer::~Buffer() {
			vkDestroyBuffer(instance, buffer, nullptr);
			vkFreeMemory(instance, deviceMemory, nullptr);
		}
	}
}
