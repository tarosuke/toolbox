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
#include <toolbox/td/vulkan/td.h>
#include <toolbox/exception.h>

#include <assert.h>



namespace TB {
	namespace VK {

		unsigned TD::StaticObject::FindMemoryType(
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


		TD::StaticObject::StaticObject(
			const std::vector<Vertex>& vertices,
			const std::vector<unsigned>& indexes)
			: nVertex(vertices.size()) {
			// 頂点バッファ確保
			VkBufferCreateInfo bufferInfo{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.flags = 0, // VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
				.size = sizeof(vertices[0]) * vertices.size(),
				.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE};

			Posit(
				!vkCreateBuffer(instance, &bufferInfo, nullptr, &vertexBuffer));

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(
				instance,
				vertexBuffer,
				&memRequirements);

			VkMemoryAllocateInfo allocInfo{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = vertices.size() * sizeof(Vertex),
				.memoryTypeIndex = FindMemoryType(
					memRequirements.memoryTypeBits,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
						VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)};
			Posit(!vkAllocateMemory(
				instance,
				&allocInfo,
				nullptr,
				&vertexBufferMemory));
			Posit(!vkBindBufferMemory(
				instance,
				vertexBuffer,
				vertexBufferMemory,
				0));

			// 頂点転送
			void* data;
			Posit(!vkMapMemory(
				instance,
				vertexBufferMemory,
				0,
				bufferInfo.size,
				0,
				&data));
			memcpy(data, vertices.data(), (size_t)bufferInfo.size);
			vkUnmapMemory(instance, vertexBufferMemory);
		}

		TD::StaticObject::~StaticObject() {
			vkDestroyBuffer(instance, vertexBuffer, nullptr);
			vkFreeMemory(instance, vertexBufferMemory, nullptr);
		}

		void TD::StaticObject::Draw(VkCommandBuffer& cb) {
			VkBuffer vertexBuffers[] = {vertexBuffer};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(cb, 0, 1, vertexBuffers, offsets);
			vkCmdDraw(cb, nVertex, 1, 0, 1);
		}
	}
}
