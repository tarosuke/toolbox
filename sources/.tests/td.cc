#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>


#define C(t, p) _binary_##t##_td_vulkan_test_frag_spv_## p
#define A(t) C(t, start)
#define B(t) C(t, end)


extern const u32 A(_BUILD_TARGET_)[];
extern const u32 B(_BUILD_TARGET_)[];

int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	TB::VK::Shader shader(A(_BUILD_TARGET_), B(_BUILD_TARGET_));


	return 0;
}
