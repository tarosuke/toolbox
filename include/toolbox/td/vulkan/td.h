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
#include <toolbox/geometry/spread.h>

#include <vulkan/vulkan.h>



namespace TB {
	namespace VK {

		/***** Vulkan版基本クラス
		 * Vulkanの論理面が集約されている
		 * 神クラスとか思うかも知れないが分けても使うのが面倒になるだけだろう
		 */
		struct TD : public TB::TD {
			struct Layer {
				char* vertex;
				char* fragment;
			};

			/***** 描画物のインターフェイス
			 */
			struct Object : public List<Object>::Node {
				virtual void Draw() = 0;
				virtual bool IsTransparent() { return false; };
			};

			// 物体の登録(登録後は再描画)
			void AddHead(Object& o) {
				head.Add(o);
				redraw = true;
			};
			void AddExternal(Object& o) {
				external.Add(o);
				redraw = true;
			};
			void AddScenery(Object& o) {
				scenery.Add(o);
				redraw = true;
			};


			void Cyclic();


		protected:
			Instance instance;
			VkFormat format;
			VkExtent2D extent;
			VkRenderPass renderPass;

			TD(const M44& proj);
			~TD();
			void Init() {
				BuildPipeline(extent);
				BuildCommanBuffer();
			};
			void BuildPipeline(VkExtent2D);
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

			/***** Binder
			 * 指定したコマンドバッファとフレームバッファのセットにコマンドを
			 * 書き込むためのインフラで、Drawするとコマンドが書き込まれる
			 */
			struct Binder {
				Binder() = delete;
				Binder(TD&, VkFramebuffer);
				~Binder();
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


			/***** ブレンド方法
			 */
			static const VkPipelineColorBlendAttachmentState opaqueBlend;
			static const VkPipelineColorBlendAttachmentState alphaBlend;

			/***** シェーダ他
			 */
			VertexShader vertexShader;
			FragmentShader fragmentShader;

			VkPipelineLayout pipelineLayout;
			VkPipeline graphicsPipeline;

			VkCommandPool commandPool;


			/***** オブジェクトリスト
			 */
			struct Target {
				Target() : modified(false){};
				void Add(Object& o) {
					(o.IsTransparent() ? transparent : opaque).Add(o);
					modified = true;
				};
				void Draw() { opaque.Foreach(&Object::Draw); };
				void Traw() { transparent.Reveach(&Object::Draw); };

			private:
				List<Object> opaque;
				List<Object> transparent;
				bool modified; //コマンド再生成が必要
			} head, external, scenery;
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
			XTD(unsigned width,
				unsigned height,
				const M44& proj);
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
