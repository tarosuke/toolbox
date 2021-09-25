#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>

#include <stdio.h>
#include <unistd.h>



int main() {
	// ManualTest;

	try {
		TB::VK::XFBTD fb(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));
	} catch (const char* message) { puts(message); }


	sleep(1);
	return 0;
}
