#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>


#define vert td_vulkan_test_vert_spv
#define frag td_vulkan_test_frag_spv

extern const u32 ShaderStart(vert)[];
extern const u32 ShaderEnd(vert)[];
extern const u32 ShaderStart(frag)[];
extern const u32 ShaderEnd(frag)[];

int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	TB::VK::FragmentShader fShader(ShaderStart(frag), ShaderEnd(frag));
	TB::VK::VertexShader vShader(ShaderStart(vert), ShaderEnd(vert));


	return 0;
}
