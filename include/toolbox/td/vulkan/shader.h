/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021, 2022 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/container/list.h>

#include <vulkan/vulkan.h>



namespace TB {
	namespace VK {

		struct Shader : public TB::List<Shader>::Node {
			struct Def {
				VkShaderStageFlagBits stage;
				struct {
					const char* start; // 開始位置
					const char* end; // 終了位置
				};
			};

			Shader() = delete;
			Shader(
				VkShaderStageFlagBits stage,
				const char* path,
				const char* end = 0);
			Shader(const Def& def) : Shader(def.stage, def.start, def.end){};
			~Shader();

			operator VkShaderModule() const { return shaderModule; };
			operator const VkPipelineShaderStageCreateInfo&() const {
				return stageInfo;
			};

		private:
			Instance instance;
			VkShaderModule shaderModule;
			VkPipelineShaderStageCreateInfo stageInfo;
		};

	}
}