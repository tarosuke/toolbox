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

			physcalDevices.resize(numGpu);

			if (vkEnumeratePhysicalDevices(
					instance,
					&numGpu,
					physcalDevices.data()) != VK_SUCCESS) {
				throw -1;
			};
		}

		void Instance::GetQueue(unsigned index) {
			if (physcalDevices.size() <= index) {
				throw -1;
			}

			auto& dev(physcalDevices[index]);
			unsigned numQ(0);
			vkGetPhysicalDeviceQueueFamilyProperties(dev, &numQ, nullptr);
			if (numQ < 1) {
				throw -1;
			}
			queue.resize(numQ);
			vkGetPhysicalDeviceQueueFamilyProperties(dev, &numQ, queue.data());
		}
	}
}
