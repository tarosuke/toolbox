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

#include <vulkan/vulkan.h>

#include <vector>



namespace TB {
	namespace VK {

		class Instance {
		public:
			Instance();
			~Instance() {
				for (auto&& dq : devQ) {
					vkDestroyDevice(dq.device, nullptr);
				}
				vkDestroyInstance(instance, nullptr);
			};

		private:
			static VkInstance MakeInstance();
			VkInstance instance;
			void GetPhysicalDevices();
			std::vector<VkPhysicalDevice> physicalDevices;
			void GetQueue(unsigned phyDeviceIndex = 0);
			unsigned physicalDeviceIndex;
			std::vector<VkQueueFamilyProperties> queueFamilies;
			void GetDevices();
			struct DevQ {
				VkDevice device;
				VkQueue queue;
			};
			std::vector<DevQ> devQ;
		};
	}
}
