/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021,2022 tarosuke<webmaster@tarosuke.net>
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

		TD::TD(const M44& proj) : TB::TD(proj) {}


		void TD::BuildRenderPass() {
			VkAttachmentDescription colorAttachment{
				.flags = 0,
				.format = format,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			};
			VkAttachmentReference colorAttachmentRef{
				.attachment = 0,
				.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			};
			VkSubpassDescription subpass{
				.flags = 0,
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.colorAttachmentCount = 1,
				.pColorAttachments = &colorAttachmentRef,
			};
			VkRenderPassCreateInfo renderPassInfo{
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.attachmentCount = 1,
				.pAttachments = &colorAttachment,
				.subpassCount = 1,
				.pSubpasses = &subpass,
			};
			Posit(!vkCreateRenderPass(
				instance,
				&renderPassInfo,
				nullptr,
				&renderPass));
		}

		void TD::BuildCommanBuffer() {
			/***** コマンドバッファ関連
			 */
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = instance.PhysicalFamilyIndex();
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			Posit(!vkCreateCommandPool(
				instance,
				&poolInfo,
				nullptr,
				&commandPool));

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;
			Posit(!vkAllocateCommandBuffers(
				instance,
				&allocInfo,
				&commandBuffer));

			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			Posit(!vkCreateSemaphore(
				instance,
				&semaphoreInfo,
				nullptr,
				&imageAvailableSemaphore));
			Posit(!vkCreateSemaphore(
				instance,
				&semaphoreInfo,
				nullptr,
				&renderFinishedSemaphore));
		}


		TD::~TD() {
			vkDestroySemaphore(instance, renderFinishedSemaphore, nullptr);
			vkDestroySemaphore(instance, imageAvailableSemaphore, nullptr);
			vkDestroyCommandPool(instance, commandPool, nullptr);
			vkDestroyRenderPass(instance, renderPass, nullptr);
		}

		const VkPipelineColorBlendAttachmentState TD::opaqueBlend{
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
			.colorBlendOp = VK_BLEND_OP_ADD, // Optional
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
			.alphaBlendOp = VK_BLEND_OP_ADD, // Optional
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		};
		const VkPipelineColorBlendAttachmentState TD::alphaBlend{
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		};

		TD::Binder::Binder(TD& td, VkFramebuffer fb, Layer& layer)
			: td(td), fb(fb), cb(td.commandBuffer) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			// VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			// VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			beginInfo.pInheritanceInfo = nullptr;

			Posit(!vkBeginCommandBuffer(cb, &beginInfo));

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = td.renderPass;
			renderPassInfo.framebuffer = fb;

			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = td.extent;

			VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(
				cb,
				&renderPassInfo,
				VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, layer);
		}

		TD::Binder::~Binder() {
			vkCmdEndRenderPass(cb);
			vkEndCommandBuffer(cb);
		}

		void TD::Binder::Draw(
			unsigned firstVertex,
			unsigned vertexes,
			unsigned instances,
			unsigned firstInstance) {
			vkCmdDraw(cb, vertexes, instances, firstVertex, firstInstance);
		}



		void TD::Cyclic() {
			Target* const targets[] = {&head, &external, &scenery};

			const nsec start;
			keep = true;
			redraw = true; // NOTE:描画内容を作るまでの仮設定
			Posit(redraw); //描画するものがない
			for (nsec ns; keep;) {
				VkFramebuffer f(NextFramebuffer());
				{
					// 必要であれば再描画
					if (redraw) {
						Binder rp(*this, f, layer);
						for (auto t : targets) {
							(*t).Draw();
						}
						rp.Draw(0, 3);
						// redraw = false;
					}
				}

				// 描画
				Draw(view);

				// 周回処理(必要ならコマンドバッファ更新リクエスト)
				nsec nns;
				nns -= start;
				timestamp.delta = nns - timestamp.uptime;
				timestamp.uptime = nns;
				Tick(timestamp);
			}
		}



	}
}
