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
#include <toolbox/td.h>
#include <toolbox/td/x.h>
#include <toolbox/td/vulkan/instance.h>
#include <toolbox/td/vulkan/shader.h>
#include <toolbox/geometry/spread.h>

#include <vulkan/vulkan.h>


namespace TB {
	namespace VK {

		// Vulkan版基本クラス
		struct TD : public TB::TD {
			struct Shaders {
				char* vertex;
				char* fragment;
			};

		protected:
			Instance instance;
			VkFormat format;
			VkExtent2D extent;
			std::vector<VkFramebuffer> framebuffers;
			VkRenderPass renderPass;
			TD(const M44& proj, const Shaders* shaders = 0);
			~TD();
			void Init();
			virtual void FillFramebuffers(std::vector<VkFramebuffer>&) = 0;

		private:
			VertexShader vertexShader;
			FragmentShader fragmentShader;

			VkPipelineLayout pipelineLayout;
			VkPipeline graphicsPipeline;
			VkCommandPool commandPool;
			std::vector<VkCommandBuffer> commandBuffers;

			static const VkPipelineColorBlendAttachmentState opaqueBlend;
			static const VkPipelineColorBlendAttachmentState alphaBlend;
		};



		// フレームバッファ版TD
		struct FBTD : public TD {
			FBTD(
				const M44& proj, const S2& viewport, const Shaders* shaders = 0)
				: TD(proj, shaders) {
				extent = {viewport[0], viewport[1]};
			};

		private:
		};

		// X窓フレームバッファ版TD
		struct XFBTD : public TD {
			XFBTD(
				unsigned width,
				unsigned height,
				const M44& proj,
				const Shaders* shaders = 0);
			~XFBTD();

		private:
			static Base::Extension<VkInstance> instanceExtensions;
			static Base::Extension<VkDevice> driverExtensions;

			::TB::X::Window window;
			VkSurfaceKHR surface;
			VkSwapchainKHR swapchain;
			std::vector<VkImage> swapchainImages;
			std::vector<VkImageView> swapchainImageViews;
			VkSurfaceCapabilitiesKHR capabilities;

			void FillFramebuffers(std::vector<VkFramebuffer>&) final;
		};
	};
}
