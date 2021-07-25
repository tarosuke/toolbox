#include <toolbox/test.h>
#include <toolbox/string.h>

#include <stdio.h>



namespace {
	void H(const char* s) { printf("%s", (const char*)s); }
}

int main() {
	TB::String string;
	assert(string.IsEmpty());
	string += '/';
	assert(!string.IsEmpty());
	assert(string.Length() == 1);
	assertEQ(string, "/", H);

	string += "abc";
	assert(string.Length() == 4);
	assertEQ(string, "/abc", H);

	return 0;
}
