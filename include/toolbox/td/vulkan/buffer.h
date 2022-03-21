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

#include <vulkan/vulkan.h>

#include <vector>



namespace TB {
	namespace VK {

		struct Buffer {
			Buffer(
				const void*,
				unsigned,
				VkBufferUsageFlagBits usage,
				VkSharingMode = VK_SHARING_MODE_EXCLUSIVE);

			template <typename T> Buffer(
				const std::vector<T>& data,
				VkBufferUsageFlagBits usage,
				VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE)
				: Buffer(
					  data.data(), sizeof(T) * data.size(), usage, shareMode){};
			~Buffer();

			operator VkBuffer() { return buffer; };
			operator VkDeviceMemory() { return deviceMemory; };

		private:
			Instance instance;
			VkBuffer buffer;
			VkDeviceMemory deviceMemory;

			unsigned FindMemoryType(unsigned, VkMemoryPropertyFlags);
		};
	}
}
