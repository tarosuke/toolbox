/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <stdexcept>



namespace TB {
	namespace VK {

		TD::TD(const M44& proj, const Shaders* shaders)
			: ::TB::TD(proj),
			  vertexShader(
				  shaders && (*shaders).vertex ? (*shaders).vertex : 0),
			  fragmentShader(
				  shaders && (*shaders).fragment ? (*shaders).fragment : 0) {



			VkPipelineLayoutCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.setLayoutCount = 0, // Optional
				.pSetLayouts = nullptr, // Optional
				.pushConstantRangeCount = 0, // Optional
				.pPushConstantRanges = nullptr, // Optional
			};
			Posit(!vkCreatePipelineLayout(
				instance,
				&info,
				nullptr,
				&pipelineLayout));
		}

		TD::~TD() {
			vkDestroyPipelineLayout(instance, pipelineLayout, nullptr);
		}
	}
}
