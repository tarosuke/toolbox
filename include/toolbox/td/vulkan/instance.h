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

		struct Base {
			friend class Instance;

			// 拡張を使うクラスでstaticメンバとしてインスタンス化
			template <typename> struct Extension {
				Extension(const std::vector<const char*>& names)
					: next(root), names(names) {
					root = this;
				};
				static void GetExtensions(std::vector<const char*>& list) {
					for (Extension* e(root); e; e = (*e).next) {
						for (const auto& n : (*e).names) {
							list.push_back(n);
						}
					}
				};

			private:
				static Extension* root;
				Extension* const next;
				std::vector<const char*> names;
			};

		private:
			static Base* singleton;

			Base();
			~Base() {
				vkDeviceWaitIdle(device);
				vkDestroyDevice(device, nullptr);
				vkDestroyInstance(instance, nullptr);
			};

			static Base& GetInstance() {
				static Base instance;
				singleton = &instance;
				return instance;
			};

			static VkInstance MakeInstance();
			VkInstance instance;
			void GetPhysicalDevices();
			std::vector<VkPhysicalDevice> physicalDevices;
			void GetQueue(unsigned phyDeviceIndex = 0);
			unsigned physicalDeviceIndex;
			std::vector<VkQueueFamilyProperties> queueFamilies;
			unsigned presentFamilyIndex;
			void GetDevices();
			const float queuePriority;
			VkQueue queue;
			VkDevice device;
			static std::vector<const char*> layers;
		};

		struct Instance {
			operator VkInstance() { return Base::GetInstance().instance; };
			operator VkDevice() { return Base::GetInstance().device; };
			operator VkPhysicalDevice() {
				Base& instance(Base::GetInstance());
				return instance.physicalDevices[instance.physicalDeviceIndex];
			};
			operator std::vector<VkPhysicalDevice>&() {
				return Base::GetInstance().physicalDevices;
			};
			unsigned PhysicalFamilyIndex() {
				return Base::GetInstance().physicalDeviceIndex;
			};
			operator VkQueue() { return Base::GetInstance().queue; };
		};
	}
}
