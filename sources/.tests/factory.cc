#include <tb/factory.h>
#include <tb/test.h>



template <> tb::String::String(const uint& o) { Append(o); }
template <> tb::String::String(const int& o) { Append(o); }

struct C {
	static uint r;
};
uint C::r(0);

struct D : C {
	struct F : tb::Factory<C> {
		uint Score() override { return 1; };
		D* New() override {
			r = 1;
			return 0;
		};
	};
};
struct E : C {
	struct F : tb::Factory<C> {
		uint Score() override { return 2; };
		E* New() override {
			r = 2;
			return 0;
		};
	};
};



template <> tb::Factory<C>* tb::Factory<C>::start(0);
D::F d;
E::F e;


int main() {
	tb::Factory<C>::Create();
	assertEQ(C::r, 2U);

	return 0;
}
