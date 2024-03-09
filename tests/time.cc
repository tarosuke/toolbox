#include <tb/test.h>
#include <tb/time.h>



template <> tb::String::String(const u64& o) { Append(o); }
template <> tb::String::String(const nsec& o) { Append((u64)o); }
template <> tb::String::String(const usec& o) { Append((u64)o); }
template <> tb::String::String(const msec& o) { Append((u64)o); }
template <> tb::String::String(const sec& o) { Append((u64)o); }

int main() {
	const tb::nsec a(5000000000);
	const tb::usec b(a);
	const tb::msec c(a);
	const tb::sec d(a);

	assertEQ(a, (tb::u64)5000000000);
	assertEQ(a, (tb::usec)b);
	assertEQ(a, c);
	assertEQ(a, d);

	return 0;
}
