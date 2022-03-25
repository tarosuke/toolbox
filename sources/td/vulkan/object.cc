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

		TD::Layer::Object::Object(
			const std::vector<Vertex>& vertices,
			const std::vector<u16>& indexes,
			const ColorImage::Def& imageDef)
			: nVertex(vertices.size()), nIndex(indexes.size()),
			  vertexBuffer(vertices), indexBuffer(indexes),
			  texture(256, 256, 0) {}

		TD::Layer::Object::~Object() {}

		void TD::Layer::Object::Draw(VkCommandBuffer& cb) {
			VkBuffer vertexBuffers[] = {vertexBuffer};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(cb, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(cb, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(cb, nIndex, 1, 0, 0, 0);
		}
	}
}
