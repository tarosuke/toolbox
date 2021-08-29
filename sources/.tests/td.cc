#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>



int main() {
	ManualTest;

	TB::X::Window w(200, 200);
	TB::VK::XFBTD fb(TB::TD::Frustum(), w);

	return 0;
}
