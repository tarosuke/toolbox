#include <toolbox/test.h>
#include <toolbox/version.h>

#include <stdio.h>



namespace {
	template <unsigned REVs> void H(const TB::Version<REVs>& t) {
		printf("%s", (const char*)t);
	}
}

int main() { return 0; }
