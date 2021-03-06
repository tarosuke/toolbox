/** linux input subsystem
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <toolbox/test.h>

#include <toolbox/input/input.h>

#include <stdio.h>



int main() {

	class Test : public TB::Input {
	public:
		Test() : Input(false){};
		void OnKeyDown(unsigned key) final { printf("keydown:%x.\n", key); };
		void OnKeyUp(unsigned key) final { printf("keyUp:%x.\n", key); };
		void OnKeyRepeat(unsigned key) final {
			printf("keyRepeat:%x.\n", key);
		};
		void OnButtonDown(unsigned button) final {
			printf("buttondown:%x.\n", button);
		};
		void OnButtonUp(unsigned button) final {
			printf("buttonup:%x.\n", button);
		};
		void OnMouseMove(unsigned axis, int diff) final {
			printf("mouseMove(%u):%d.\n", axis, diff);
		};
	} test;

	for (unsigned n(0); n < 1000000; ++n) {
		test.GetInput();
	}

	return 0;
}
