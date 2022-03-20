/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2022 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/blob.h>


#define vertPath td_vulkan_scenery_vert_spv
#define fragPath td_vulkan_scenery_frag_spv

BlobDeclare(vertPath);
BlobDeclare(fragPath);



namespace TB {
	namespace VK {

		/***** デフォルトのレイヤ定義
		 */
		const std::vector<TD::Layer::Def> TD::Layer::defaultLayerDefs = {
			{"scenery",
			 {{VK_SHADER_STAGE_VERTEX_BIT, Blob(vertPath)},
			  {VK_SHADER_STAGE_FRAGMENT_BIT, Blob(fragPath)}}},
		};



		/***** defsに従いレイヤをまとめて生成
		 * defsが0のときはデフォルトで
		 */
		void TD::Layer::NewLayers(
			TB::List<Layer>& storeTo,
			VkExtent2D extent,
			VkRenderPass renderPass,
			const std::vector<Def>* defs) {
			// Layerの生成
			for (auto&& d : defs ? *defs : defaultLayerDefs) {
				auto const l(new Layer(extent, renderPass, d));
				Posit(l);
				storeTo.Add(*l);
			}
		};

		TD::Layer::Layer(
			VkExtent2D extent, VkRenderPass renderPass, const Def& def)
			: Node(true), name(def.name) {
			/***** シェーダ生成
			 */
			std::vector<VkPipelineShaderStageCreateInfo> shadersInfo;
			for (auto&& d : def.shaderDefs) {
				Shader* const s(new Shader(d));
				Posit(s);
				shaders.Add(*s);
				shadersInfo.push_back(*s);
			}

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
			// VkDynamicState dynamicStates[] = {
			// 	VK_DYNAMIC_STATE_VIEWPORT,
			// 	VK_DYNAMIC_STATE_LINE_WIDTH};
			// VkPipelineDynamicStateCreateInfo dynamicState{
			// 	.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			// 	.pNext = 0,
			// 	.flags = 0,
			// 	.dynamicStateCount = 0,
			// 	.pDynamicStates = dynamicStates,
			// };
			VkGraphicsPipelineCreateInfo pipelineInfo{
				.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.pNext = 0,
				.flags = 0,
				.stageCount = (unsigned)shadersInfo.size(),
				.pStages = shadersInfo.data(),

				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling,
				.pDepthStencilState = nullptr, // Optional
				.pColorBlendState = &colorBlending,
				.pDynamicState = 0, //&dynamicState, // Optional

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
		}

		TD::Layer::~Layer() {
			vkDestroyPipeline(instance, graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(instance, pipelineLayout, nullptr);
		}

		void TD::Layer::Draw(VkCommandBuffer& cb) {
			for (auto o : objects) {
				o->Draw(cb);
			}
		}
	}
}