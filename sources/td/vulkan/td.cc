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
				  shaders && (*shaders).fragment ? (*shaders).fragment : 0) {}


		void TD::Init() {
			/***** フレームバッファまで
			 */
			VkPipelineLayoutCreateInfo info{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
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


			VkPipelineShaderStageCreateInfo shaderStages[] = {
				vertexShader,
				fragmentShader,
			};
			VkPipelineVertexInputStateCreateInfo vertexInputInfo{
				.sType =
					VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.vertexBindingDescriptionCount = 0,
				.pVertexBindingDescriptions = nullptr, // Optional
				.vertexAttributeDescriptionCount = 0,
				.pVertexAttributeDescriptions = nullptr, // Optional
			};
			VkPipelineInputAssemblyStateCreateInfo inputAssembly{
				.sType =
					VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE,
			};
			VkViewport viewport{
				.x = 0.0f,
				.y = 0.0f,
				.width = (float)extent.width,
				.height = (float)extent.height,
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};
			VkRect2D scissor{
				.offset = {0, 0},
				.extent = extent,
			};
			VkPipelineViewportStateCreateInfo viewportState{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.viewportCount = 1,
				.pViewports = &viewport,
				.scissorCount = 1,
				.pScissors = &scissor,
			};
			VkPipelineRasterizationStateCreateInfo rasterizer{
				.sType =
					VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = VK_POLYGON_MODE_FILL,
				.cullMode = VK_CULL_MODE_BACK_BIT,
				.frontFace = VK_FRONT_FACE_CLOCKWISE,
				.depthBiasEnable = VK_FALSE,
				.depthBiasConstantFactor = 0.0f, // Optional
				.depthBiasClamp = 0.0f, // Optional
				.depthBiasSlopeFactor = 0.0f, // Optional
				.lineWidth = 1.0f,
			};
			VkPipelineMultisampleStateCreateInfo multisampling{
				.sType =
					VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
				.sampleShadingEnable = VK_FALSE,
				.minSampleShading = 1.0f, // Optional
				.pSampleMask = nullptr, // Optional
				.alphaToCoverageEnable = VK_FALSE, // Optional
				.alphaToOneEnable = VK_FALSE, // Optional
			};
			VkPipelineColorBlendStateCreateInfo colorBlending{
				.sType =
					VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.logicOpEnable = VK_FALSE,
				.logicOp = VK_LOGIC_OP_COPY, // Optional
				.attachmentCount = 1,
				.pAttachments = &opaqueBlend,
				.blendConstants =
					{
						0.0f, // Optional
						0.0f, // Optional
						0.0f, // Optional
						0.0f, // Optional
					},
			};
			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_LINE_WIDTH};
			VkPipelineDynamicStateCreateInfo dynamicState{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.dynamicStateCount = 2,
				.pDynamicStates = dynamicStates,
			};
			VkGraphicsPipelineCreateInfo pipelineInfo{
				.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.stageCount = 2,
				.pStages = shaderStages,

				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling,
				.pDepthStencilState = nullptr, // Optional
				.pColorBlendState = &colorBlending,
				.pDynamicState = &dynamicState, // Optional

				.layout = pipelineLayout,

				.renderPass = renderPass,
				.subpass = 0,

				.basePipelineHandle = VK_NULL_HANDLE, // Optional
				.basePipelineIndex = -1, // Optional
			};
			Posit(!vkCreateGraphicsPipelines(
				instance,
				VK_NULL_HANDLE,
				1,
				&pipelineInfo,
				nullptr,
				&graphicsPipeline));


			FillFramebuffers(framebuffers);

			/***** コマンドバッファ関連
			 */
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = instance.PhysicalFamilyIndex();
			poolInfo.flags = 0; // Optional
			Posit(!vkCreateCommandPool(
				instance,
				&poolInfo,
				nullptr,
				&commandPool));

			commandBuffers.resize(framebuffers.size());
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
			Posit(!vkAllocateCommandBuffers(
				instance,
				&allocInfo,
				commandBuffers.data()));
		}

		TD::~TD() {
			vkDestroyCommandPool(instance, commandPool, nullptr);
			for (auto f : framebuffers) {
				vkDestroyFramebuffer(instance, f, nullptr);
			}
			vkDestroyPipeline(instance, graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(instance, pipelineLayout, nullptr);
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
	}
}
