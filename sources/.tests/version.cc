#include <toolbox/test.h>
#include <toolbox/version.h>

#include <stdio.h>



namespace {
	template <unsigned REVs> void H(const TB::Version<REVs>& t) { t.Print(); }
}

template <unsigned REVs> void TB::Version<REVs>::TestPrint() {}

int main() { return 0; }
