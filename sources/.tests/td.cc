#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>
#include <toolbox/blob.h>


#define vert td_vulkan_test_vert_spv
#define frag td_vulkan_test_frag_spv

BlobDeclare(vert);
BlobDeclare(frag);


int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	TB::VK::FragmentShader fShader(BlobStart(frag), BlobEnd(frag));
	TB::VK::VertexShader vShader(BlobStart(vert), BlobEnd(vert));


	return 0;
}
