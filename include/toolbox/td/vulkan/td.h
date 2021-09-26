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

		/***** Vulkan版基本クラス
		 * Vulkanの論理面が集約されている
		 * 神クラスとか思うかも知れないが分けても使うのが面倒になるだけだろう
		 */
		struct TD : public TB::TD {
			struct Shaders {
				char* vertex;
				char* fragment;
			};

			/***** RenderPass
			 * 指定したコマンドバッファとフレームバッファのセットにコマンドを
			 * 書き込むためのインフラで、Drawするとコマンドが書き込まれる
			 */
			struct RenderPass {
				RenderPass() = delete;
				RenderPass(TD&);
				~RenderPass();
				void Draw(
					unsigned vertexIndex,
					unsigned vertexes,
					unsigned instances = 1,
					unsigned firstInstance = 1);

			private:
				TD& td;
				VkFramebuffer fb;
				VkCommandBuffer cb;
			};

		protected:
			Instance instance;
			VkFormat format;
			VkExtent2D extent;
			VkRenderPass renderPass;

			const M44 projectile;

			TD(const M44& proj, const Shaders* shaders = 0);
			~TD();
			void Init();
			virtual void FillFramebuffers(std::vector<VkFramebuffer>&) = 0;

			// 描画処理
			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;
			VkSemaphore signalSemaphore;

			unsigned imageIndex; // アクティブなフレームバッファ
			unsigned commandIndex; // アクティブなコマンドバッファ
			// void Prepare() override;
			// void Draw(const M44&) final;

		private:
			VertexShader vertexShader;
			FragmentShader fragmentShader;

			VkPipelineLayout pipelineLayout;
			VkPipeline graphicsPipeline;
			std::vector<VkFramebuffer> framebuffers;
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
		struct XTD : public TD {
			XTD(unsigned width,
				unsigned height,
				const M44& proj,
				const Shaders* shaders = 0);
			~XTD();

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
			void Prepare() final;
			void Draw(const M44&) final;
		};
	};
}
