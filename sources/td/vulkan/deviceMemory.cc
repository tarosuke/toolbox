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
#include <toolbox/td/vulkan/deviceMemory.h>
#include <toolbox/exception.h>



namespace TB {
	namespace VK {


		DeviceMemory::DeviceMemory(
			u32 size, VkBuffer buffer, VkMemoryPropertyFlags propertyFlags) {
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
		}
		DeviceMemory::DeviceMemory(
			u32 size, VkImage image, VkMemoryPropertyFlags propertyFlags) {
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(instance, image, &memRequirements);

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
		}

		DeviceMemory::~DeviceMemory() {
			vkFreeMemory(instance, deviceMemory, nullptr);
		}

		unsigned DeviceMemory::FindMemoryType(
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

	}
}
