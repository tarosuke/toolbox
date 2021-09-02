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

		struct Instance {
			Instance();
			~Instance() {
				vkDestroyDevice(device, nullptr);
				vkDestroyInstance(instance, nullptr);
			};

			struct Property {
				VkInstance& instance;
				VkPhysicalDevice& physicalDevice;
				VkDevice& device;
				unsigned& physicalDeviceIndex;
				std::vector<VkPhysicalDevice>& physicalDevices;
				unsigned& presentFamilyIndex;
			};
			static Property GetProperty() {
				return Property{
					.instance = singleton.instance,
					.physicalDevice =
						singleton
							.physicalDevices[singleton.physicalDeviceIndex],
					.device = singleton.device,
					.physicalDeviceIndex = singleton.physicalDeviceIndex,
					.physicalDevices = singleton.physicalDevices,
					.presentFamilyIndex = singleton.presentFamilyIndex};
			};


			// 拡張を使うクラスでstaticメンバとしてインスタンス化
			struct Extension {
				Extension(const char* name) : next(root), name(name) {
					root = this;
				};
				static void GetExtensions(std::vector<const char*>&);

			private:
				static Extension* root;
				Extension* const next;
				const char* name;
			};

		private:
			static Instance singleton;

			static VkInstance MakeInstance();
			VkInstance instance;
			void GetPhysicalDevices();
			std::vector<VkPhysicalDevice> physicalDevices;
			void GetQueue(unsigned phyDeviceIndex = 0);
			unsigned physicalDeviceIndex;
			std::vector<VkQueueFamilyProperties> queueFamilies;
			unsigned presentFamilyIndex;
			void GetDevices();
			VkQueue queue;
			VkDevice device;
		};
	}
}
