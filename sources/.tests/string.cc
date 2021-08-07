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


	string += TB::String("/abc");
	assertEQ(string.Length(), 8, HU);
	assertEQ(string, "/abc/abc", H);

	string += string;
	string += string;
	assertEQ(string.Length(), 32, HU);
	assertEQ(string, "/abc/abc/abc/abc/abc/abc/abc/abc", H);

	TB::Array<TB::String> s(string.Split("/a"));
	assertEQ(s.Length(), 9, HU);
	assertEQ(s[0], "", H);
	for (unsigned n(1); n < 9; ++n) {
		assertEQ(s[n], "bc", H);
	};
	TB::Array<TB::String> p(string.Split("/abc"));
	assertEQ(s.Length(), 9, HU);
	for (unsigned n(0); n < 9; ++n) {
		assertEQ(s[n], "", H);
	};

	return 0;
}
