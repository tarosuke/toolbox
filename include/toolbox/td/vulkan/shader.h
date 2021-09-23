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
#pragma once
#include <toolbox/type.h>
#include <toolbox/td/vulkan/instance.h>

#include <vulkan/vulkan.h>



namespace TB {
	namespace VK {

		struct Shader {
			Shader() = delete;
			Shader(
				VkShaderStageFlagBits stage,
				const char* path,
				const char* end = 0);
			~Shader();

			operator VkShaderModule() { return shaderModule; };
			operator const VkPipelineShaderStageCreateInfo&() {
				return stageInfo;
			};

		private:
			Instance instance;
			VkShaderModule shaderModule;
			VkPipelineShaderStageCreateInfo stageInfo;
		};
		struct VertexShader : public Shader {
			VertexShader(const char* path = 0);
		};
		struct FragmentShader : public Shader {
			FragmentShader(const char* path = 0);
		};

	}
}