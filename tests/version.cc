#include <toolbox/test.h>
#include <toolbox/version.h>

#include <stdio.h>



template <> TB::String::String(const TB::Version<3>& v) { *this = v; }

int main() {
	TB::Version<3> ver("2250.1.25");
	TB::Version<3> v2(ver);
	assertEQ(ver, v2);

	TB::String s(ver);
	assertEQ(s, "2250.1.25");

	return 0;
}
