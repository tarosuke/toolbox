#include <toolbox/test.h>
#include <toolbox/string.h>

#include <stdio.h>



template <> TB::String::String(const int& v) { From(v); }
template <> TB::String::String(const unsigned& v) { From(v); }

int main() {
	TB::String string;
	assert(string.IsEmpty());
	string << '/';
	assert(!string.IsEmpty());
	assertEQ(string.Length(), 1);
	assertEQ(string, "/");

	string << "abc";
	assertEQ(string.Length(), 4);
	assertEQ(string, "/abc");


	string << TB::String("/abc");
	assertEQ(string.Length(), 8);
	assertEQ(string, "/abc/abc");

	string << string;
	string << string;
	assertEQ(string.Length(), 32);
	assertEQ(string, "/abc/abc/abc/abc/abc/abc/abc/abc");

	TB::Array<TB::String> s(string.Split("/a"));
	assertEQ(s.Length(), 9);
	assertEQ(s[0], "");
	for (unsigned n(1); n < 9; ++n) {
		assertEQ(s[n], "bc");
	};
	TB::Array<TB::String> p(string.Split("/abc"));
	assertEQ(p.Length(), 9);
	for (unsigned n(0); n < 9; ++n) {
		assertEQ(p[n], "");
	};

	return 0;
}
