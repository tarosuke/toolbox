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

#include "device.h"

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

			static const VkImageCreateInfo colorBufferImageInfo;
			static const VkImageCreateInfo depthBufferImageInfo;

		private:
			Device device;
			VkImage image;
			VkImageCreateInfo info;
		};

		class ImageView {
		public:
			ImageView(Image&);
			~ImageView();

			operator VkImageView&() { return imageView; };
			operator VkFormat() { return image; };
			operator VkExtent3D&() { return image; };

		private:
			Device device;
			Image& image;
			VkImageView imageView;
		};

		class FrameBuffer {
		public:
			FrameBuffer(
				unsigned width,
				unsigned height,
				VkFormat format = VK_FORMAT_B8G8R8_UINT,
				VkFormat depthForm = VK_FORMAT_D24_UNORM_S8_UINT);
			~FrameBuffer();

		private:
			Device device;
			ImageView color;
			ImageView depth;
		};
	}
}
