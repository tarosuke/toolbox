#include <toolbox/test.h>
#include <toolbox/version.h>

#include <stdio.h>



namespace {
	template <unsigned REVs> void H(const TB::Version<REVs>& t) {
		printf("%s", (const char*)(const TB::String)t);
	}
	void HS(const char* s) { printf("%s", (const char*)s); }
	// void HU(unsigned s) { printf("%u", s); }
}

int main() {
	TB::Version<3> ver("2250.1.25");
	TB::Version<3> v2(ver);
	assertEQ(ver, v2, H);

	TB::String s(ver);
	assertEQ(s, "2250.1.25", HS);

	return 0;
}
