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
#include <toolbox/tg/vulkan/instance.h>

#include <vector>



namespace TB {
	namespace VK {

		Instance::Instance() : instance(MakeInstance()) {
			GetPhysicalDevices();
			GetQueue();
			GetDevices();
		}


		VkInstance Instance::MakeInstance() {
			std::vector<char*> extentionNames;

			//このへんで機能拡張をextensionNamesに格納しておく

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
				.enabledExtensionCount = (unsigned)extentionNames.size(),
				.ppEnabledExtensionNames = extentionNames.data(),
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
			devQ.resize(numQ);
			vkGetPhysicalDeviceQueueFamilyProperties(
				dev,
				&numQ,
				queueFamilies.data());
		}

		void Instance::GetDevices() {
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
				const VkDeviceCreateInfo createInfo = {
					.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
					.pNext = NULL,
					.flags = 0,
					.queueCreateInfoCount = 1,
					.pQueueCreateInfos = &qInfo,
					.enabledLayerCount = 0,
					.enabledExtensionCount = 0,
					.pEnabledFeatures = NULL,
				};
				DevQ& dq(devQ[n]);
				if (vkCreateDevice(
						physicalDevices[physicalDeviceIndex],
						&createInfo,
						nullptr,
						&dq.device) != VK_SUCCESS) {
					throw -1;
				}
				vkGetDeviceQueue(dq.device, n, 0, &dq.queue);
			}
		}
	}
}
