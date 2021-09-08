#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>



int main() {
	ManualTest;

	TB::X::Window w(200, 200);
	TB::VK::XFBTD fb(TB::TD::Frustum(0.01, 0.01, 0.01, 100000), w);

	return 0;
}
