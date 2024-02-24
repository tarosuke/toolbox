/** linux input subsystem
 * Copyright (C) 2022 2024 tarosuke<webmaster@tarosuke.net>
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
#pragma once

#include <linux/input.h>
#include <tb/input.h>



namespace tb {
	namespace linux {

		struct Input : public virtual tb::Input {
			Input(msec outTime = 0, bool grab = false);
			~Input();

		private:
			struct LBR : tb::Input::ButtonReport {
				void operator+=(const input_event& e) {
					const unsigned mask(1 << (e.code & 0x0f));
					if (e.value) {
						down |= mask;
						state |= mask;
					} else {
						up |= mask;
						state &= ~mask;
					}
				};
			};

			LBR mouseButton;
			LBR wheelButton;
			LBR gamepad;

			const int outms;
			std::vector<pollfd> evs;

			void GetInput() override;
		};
	}
}
