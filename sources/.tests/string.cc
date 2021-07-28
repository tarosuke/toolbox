#include <toolbox/test.h>
#include <toolbox/string.h>

#include <stdio.h>



namespace {
	void H(const char* s) { printf("%s", (const char*)s); }
	void HU(unsigned s) { printf("%u", s); }
}

int main() {
	TB::String string;
	assert(string.IsEmpty());
	string += '/';
	assert(!string.IsEmpty());
	assertEQ(string.Length(), 1, HU);
	assertEQ(string, "/", H);

	string += "abc";
	assertEQ(string.Length(), 4, HU);
	assertEQ(string, "/abc", H);

	TB::String s2("/abc");
	string += s2;
	assertEQ(string.Length(), 8, HU);
	assertEQ(string, "/abc/abc", H);

	string += string;
	string += string;
	assertEQ(string.Length(), 32, HU);
	assertEQ(string, "/abc/abc/abc/abc/abc/abc/abc/abc", H);

	return 0;
}
