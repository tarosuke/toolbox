#include <toolbox/test.h>
#include <toolbox/container/array.h>

#include <stdio.h>



namespace {
	void H(const TB::Array<int>& t) {
		printf("{ ");
		for (unsigned n(0); n < t.Length(); ++n) {
			printf("%d ", t[n]);
		}
		printf("}");
	}
}

int main() {
	TB::Array<int> a((const int[]){-1, 0, 1, 2, 7}, 5);
	TB::Array<int> b(a);
	assertEQ(a, b, H);


	return 0;
}
