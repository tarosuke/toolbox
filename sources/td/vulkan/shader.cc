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



namespace TB {
	namespace VK {

		Shader::Shader(
			VkShaderStageFlagBits stage,
			const unsigned char* start,
			const unsigned char* end) {
			VkShaderModuleCreateInfo ci{
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.codeSize = (size_t)(end - start),
				.pCode = (unsigned*)start,
			};
			Posit(!vkCreateShaderModule(instance, &ci, nullptr, &shaderModule));

			stageInfo.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = stage;
			stageInfo.module = shaderModule;
			stageInfo.pName = "main";
		}

		Shader::~Shader() {
			vkDestroyShaderModule(instance, shaderModule, nullptr);
		}
	}
}
