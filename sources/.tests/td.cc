#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>


#define path td_vulkan_test_frag_spv

extern const u32 ShaderStart(path)[];
extern const u32 ShaderEnd(path)[];

int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	TB::VK::Shader shader(ShaderStart(path), ShaderEnd(path));


	return 0;
}
