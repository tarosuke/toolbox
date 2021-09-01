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
#include <toolbox/td/vulkan/instance.h>

#include <vector>
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>



namespace TB {
	namespace VK {

		Instance Instance::singleton;

		Instance::Instance() : instance(MakeInstance()) {
			GetPhysicalDevices();
			GetQueue();
			GetDevices();
		}


		VkInstance Instance::MakeInstance() {
			// 拡張リストの収集
			std::vector<const char*> extensionNames;
			Extension::GetExtensions(extensionNames);


			const VkApplicationInfo appInfo = {
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName = "Toolbox graphics",
				.applicationVersion = 1,
				.pEngineName = nullptr,
				.engineVersion = 1,
				.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
			};

			const VkInstanceCreateInfo cinfo = {
				.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				.pNext = NULL,
				.pApplicationInfo = &appInfo,
				.enabledExtensionCount = (unsigned)extensionNames.size(),
				.ppEnabledExtensionNames = extensionNames.data(),
			};

			VkInstance instance;
			if (vkCreateInstance(&cinfo, NULL, &instance) != VK_SUCCESS) {
				throw -1;
			}

			return instance;
		}

		void Instance::GetPhysicalDevices() {
			unsigned numGpu;
			if (vkEnumeratePhysicalDevices(instance, &numGpu, NULL) !=
				VK_SUCCESS) {
				throw -1;
			};

			physicalDevices.resize(numGpu);

			if (vkEnumeratePhysicalDevices(
					instance,
					&numGpu,
					physicalDevices.data()) != VK_SUCCESS) {
				throw -1;
			};
		}

		void Instance::GetQueue(unsigned index) {
			if (physicalDevices.size() <= index) {
				throw -1;
			}
			physicalDeviceIndex = index;

			auto& dev(physicalDevices[index]);
			unsigned numQ(0);
			vkGetPhysicalDeviceQueueFamilyProperties(dev, &numQ, nullptr);
			if (numQ < 1) {
				throw -1;
			}
			queueFamilies.resize(numQ);
			vkGetPhysicalDeviceQueueFamilyProperties(
				dev,
				&numQ,
				queueFamilies.data());
		}

		void Instance::GetDevices() {
			std::vector<VkDeviceQueueCreateInfo> qInfos;
			unsigned presentFamilyIndex(0);
			for (unsigned n(0); n < queueFamilies.size(); ++n) {
				float priority(1.0);
				const VkDeviceQueueCreateInfo qInfo = {
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext = NULL,
					.flags = 0,
					.queueFamilyIndex = n,
					.queueCount = 1,
					.pQueuePriorities = &priority,
				};
				qInfos.push_back(qInfo);
				if (queueFamilies[n].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					presentFamilyIndex = n;
				}
			}

			const VkDeviceCreateInfo createInfo = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.pNext = NULL,
				.flags = 0,
				.queueCreateInfoCount = (unsigned)qInfos.size(),
				.pQueueCreateInfos = qInfos.data(),
				.enabledLayerCount = 0,
				.enabledExtensionCount = 0,
				.pEnabledFeatures = NULL,
			};
			if (vkCreateDevice(
					physicalDevices[physicalDeviceIndex],
					&createInfo,
					nullptr,
					&device) != VK_SUCCESS) {
				throw -1;
			}
			vkGetDeviceQueue(device, presentFamilyIndex, 0, &queue);
		}

		Instance::Extension* Instance::Extension::root(0);
		void
		Instance::Extension::GetExtensions(std::vector<const char*>& list) {
			for (Extension* e(root); e; e = (*e).next) {
				list.push_back((*e).name);
			}
		}
	}
}
