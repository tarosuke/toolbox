#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>



int main() {
	ManualTest;

	TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));


	return 0;
}
