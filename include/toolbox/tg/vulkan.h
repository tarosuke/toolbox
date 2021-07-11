/************************************************************ toolbox graphics
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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

#include <toolbox/tg/vulkan/device.h>



namespace TB {
	namespace VK {
		class Image {
		public:
			Image(
				unsigned width,
				unsigned height,
				const VkImageCreateInfo& createInfo = colorBufferImageInfo);
			~Image();

			operator VkImage&() { return image; };
			operator VkFormat() { return info.format; };
			operator VkExtent3D&() { return info.extent; };
			operator VkImageUsageFlags() { return info.usage; };

			unsigned Width() { return info.extent.width; };
			unsigned Height() { return info.extent.height; };

			static const VkImageCreateInfo colorBufferImageInfo;
			static const VkImageCreateInfo depthBufferImageInfo;

		private:
			Device device;
			VkImage image;
			VkImageCreateInfo info;
		};


		class Canvas {
		public:
			Canvas(
				Image& baseImage,
				VkFormat depthFormat = VK_FORMAT_D24_UNORM_S8_UINT);
			~Canvas();

		private:
			Device device;
			Image& colorBuffer;
			Image depthBuffer;
			VkRenderPass renderPass;
			VkImageView views[2];
			VkFramebuffer framebuffer;
			void MakeImageView(VkImageView*, Image&);
			void MakeRenderPass(VkRenderPass*, VkFormat);
		};


		class FrameBuffer : public Image, public Canvas {
		public:
			FrameBuffer(
				unsigned width,
				unsigned height,
				VkFormat format = VK_FORMAT_B8G8R8_UINT)
				: Image(width, height, MakeImageInfo(format)),
				  Canvas((TB::VK::Image&)*this, format){};

		private:
			VkImageCreateInfo MakeImageInfo(VkFormat format) {
				VkImageCreateInfo info(Image::colorBufferImageInfo);
				info.format = format;
				return info;
			};
		};
	}
}
