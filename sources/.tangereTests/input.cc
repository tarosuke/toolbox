#include <tb/test.h>

#include <tb/linux/input.h>

#include <stdio.h>



int main() {
	class Test : virtual tb::Input, public tb::linux::Input {
	public:
		Test() : tb::linux::Input(1000, false){};
		void OnKeyDown(const tb::Timestamp&, unsigned key) final {
			printf("keydown:%x.\n", key);
		};
		void OnKeyUp(const tb::Timestamp&, unsigned key) final {
			printf("keyUp:%x.\n", key);
		};
		void OnKeyRepeat(const tb::Timestamp&, unsigned key) final {
			printf("keyRepeat:%x.\n", key);
		};
		void OnMouseButton(const tb::Timestamp&, const ButtonReport& b) final {
			printf(
				"mouse down:%04x up:%04x state:%04x .\n",
				b.up,
				b.down,
				b.state);
		};
		void OnWheel(const tb::Timestamp&, const ButtonReport& b) final {
			printf(
				"wheel down:%04x up:%04x state:%04x .\n",
				b.up,
				b.down,
				b.state);
		};
		void OnGamepad(const tb::Timestamp&, const ButtonReport& b) final {
			printf(
				"gamepad down:%04x up:%04x state:%04x .\n",
				b.up,
				b.down,
				b.state);
		};
		void OnRelMoved(const tb::Timestamp&, const AxisReport& r) final {
			printf("mouse moved:%+2d %+2d.\n", r.value[0], r.value[1]);
		};
		void OnAbsMoved(const tb::Timestamp&, const AxisReport& r) final {
			printf("stick moved:");
			for (unsigned n(0); n < 18; ++n) {
				printf("%+5d ", r.value[n]);
			}
			puts("");
		};
	} test;

	tb::Input& t(test);

	for (unsigned n(0); n < 30; ++n) {
		t.GetInput();
	}

	return 0;
}
