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
#include <toolbox/td.h>
#include <toolbox/td/x.h>
#include <toolbox/td/vulkan/instance.h>
#include <toolbox/td/vulkan/shader.h>
#include <toolbox/td/vulkan/buffer.h>
#include <toolbox/td/vulkan/image.h>
#include <toolbox/geometry/spread.h>
#include <toolbox/string.h>
#include <toolbox/canvas.h>

#include <vulkan/vulkan.h>

#include <vector>



namespace TB {
	namespace VK {

		/***** Vulkan版基本クラス
		 * Vulkanの論理面が集約されている
		 */
		struct TD : public TB::TD {
			/***** 描画レイヤ
			 * 実質的にShaderとObjectのセット
			 */
			struct Layer : public TB::List<Layer>::Node {
				struct Def {
					const char* name;
					std::vector<Shader::Def> shaderDefs;
					std::vector<VkVertexInputBindingDescription>
						bindingDescriptions;
					std::vector<VkVertexInputAttributeDescription>
						attributeDescriptions;
				};

				/***** 描画物
				 */
				struct Object : public TB::List<Object>::Node {
					struct Vertex {
						struct {
							float x;
							float y;
							float z;
						} pos;
						unsigned : 32;
						struct {
							float u;
							float v;
						} texCoord;
					};

					Object(const std::vector<Vertex>&, const std::vector<u16>&);
					~Object();

					void Draw(VkCommandBuffer&);

				private:
					const unsigned nVertex;
					const unsigned nIndex;
					Instance instance;
					VertexBuffer vertexBuffer;
					IndexBuffer indexBuffer;
					Image texture;
				};


				Layer() = delete;
				Layer(VkExtent2D, VkRenderPass, const Def&);
				~Layer();

				operator VkPipeline&() { return graphicsPipeline; };

				static void NewLayers(
					TB::List<Layer>& storeTo,
					VkExtent2D,
					VkRenderPass,
					const std::vector<Def>* = 0);
				const TB::String& Name() { return name; };

				void Add(Object& o) { objects.Add(o); };
				void Insert(Object& o) { objects.Insert(o); };
				void Draw(VkCommandBuffer&);

			private:
				static const std::vector<Def> defaultLayerDefs;
				Instance instance;
				TB::List<Shader> shaders;

				VkPipelineLayout pipelineLayout;
				VkPipeline graphicsPipeline;

				const TB::String name;

				TB::List<Object> objects;
			};
			TB::List<Layer> layers;



			/***** 名前でレイヤを探す
			 */
			Layer& FindLayer(const char* name);


			/***** 周回処理
			 * NOTE: ::TDへ移動予定
			 */
			void Cyclic();


		protected:
			Instance instance;
			VkFormat format;
			VkExtent2D extent;
			VkRenderPass renderPass;

			TD(const M44& proj);
			~TD();
			void Init() {
				BuildRenderPass();
				Layer::NewLayers(layers, extent, renderPass);
				BuildCommanBuffer();
			};
			void BuildRenderPass();
			void BuildCommanBuffer();

			// 描画処理
			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;

			unsigned commandIndex; // アクティブなコマンドバッファ
			// void Prepare() override;
			// void Draw(const M44&) final;

			VkCommandBuffer commandBuffer;


			virtual VkFramebuffer NextFramebuffer() = 0;

		private:
			M44 view;
			M44 model;

			/***** RenderPass
			 * 指定したコマンドバッファとフレームバッファのセットにコマンドを
			 * 書き込むためのインフラで、Drawするとコマンドが書き込まれる
			 */
			struct RenderPass {
				RenderPass() = delete;
				RenderPass(TD&, VkFramebuffer, Layer&);
				~RenderPass();
				operator VkCommandBuffer&() { return cb; };

			private:
				TD& td;
				VkFramebuffer fb;
				VkCommandBuffer cb;
			};


			/***** ブレンド方法
			 */
			static const VkPipelineColorBlendAttachmentState opaqueBlend;
			static const VkPipelineColorBlendAttachmentState alphaBlend;

			/***** コマンド他
			 */
			VkCommandPool commandPool;
		};



		// フレームバッファ版TD
		struct FBTD : public TD {
			FBTD(const M44& proj, const S2& viewport) : TD(proj) {
				extent = {viewport[0], viewport[1]};
			};

		private:
		};



		// X窓フレームバッファ版TD
		struct XTD : public TD {
			XTD(unsigned width, unsigned height, const M44& proj);
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
			std::vector<VkFramebuffer> framebuffers;
			uint32_t frameIndex;

			VkFramebuffer NextFramebuffer() final;
			void Draw(const M44&) final;
		};
	};
}
