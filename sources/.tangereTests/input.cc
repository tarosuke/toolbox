#include <tb/test.h>

#include <tb/input.h>
#include <tb/linux/input.h>

#include <stdio.h>



int main() {
	class Test : virtual tb::Input, public tb::linux::Input {
	public:
		Test() : tb::linux::Input(1000, false){};
		void OnKeyDown(unsigned key) final { printf("keydown:%x.\n", key); };
		void OnKeyUp(unsigned key) final { printf("keyUp:%x.\n", key); };
		void OnKeyRepeat(unsigned key) final {
			printf("keyRepeat:%x.\n", key);
		};
		void OnMouseDown(unsigned button) final {
			printf("buttondown:%x.\n", button);
		};
		void OnMouseUp(unsigned button) final {
			printf("buttonup:%x.\n", button);
		};
		void OnRelMoved(const AxisReport& r) final {
			printf("mouse moved:%+2d %+2d.\n", r.value[0], r.value[1]);
		};
		void OnAbsMoved(const AxisReport& r) final {
			printf(
				"stick moved:(%+2d %+2d %+2d) - (%+2d %+2d %+2d).\n",
				r.value[0],
				r.value[1],
				r.value[2],
				r.value[3],
				r.value[4],
				r.value[5]);
		};
	} test;

	tb::Input& t(test);

	for (unsigned n(0); n < 30; ++n) {
		t.GetInput();
	}

	return 0;
}
