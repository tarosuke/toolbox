#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>



extern const u32 _binary_DEBUG_td_vulkan_test_frag_spv_start[];
extern const u32 _binary_DEBUG_td_vulkan_test_frag_spv_end[];

int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	TB::VK::Shader shader(
		_binary_DEBUG_td_vulkan_test_frag_spv_start,
		_binary_DEBUG_td_vulkan_test_frag_spv_end);


	return 0;
}
