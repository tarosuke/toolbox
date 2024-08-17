/** linux input subsystem
 * Copyright (C) 2021, 2024 tarosuke<webmaster@tarosuke.net>
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
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <tb/linux/input.h>
#include <tb/types.h>



namespace tb {
	namespace linux {

		Input::Input(msec outTime, bool grab) : outms((int)(u64)outTime) {
			for (unsigned n(0); n < 32; ++n) {
				char path[32];
				sprintf(path, "/dev/input/event%u", n);
				const int fd(open(path, O_RDWR | O_NONBLOCK));
				if (fd < 0) {
					continue;
				}
				ioctl(fd, EVIOCGRAB, grab);
				evs.push_back({fd, POLLIN, 0});
			}
		}

		Input::~Input() {
			for (auto& e : evs) {
				close(e.fd);
			}
		}

		void Input::GetInput() {
			const int p = poll(evs.data(), evs.size(), outms);
			if (0 < p) {
				// イベント発生
				int rValue[AxisReport::nAxis]{};
				AxisReport relativeReport(rValue), absoluteReport(aValue);
				timestamp.Update();

				for (auto& e : evs) {
					if (~e.revents & POLLIN) {
						continue;
					}
					input_event ev;
					while (sizeof(ev) == read(e.fd, &ev, sizeof(ev))) {
						switch (ev.type) {
						case EV_KEY:
							if (ev.code < 256) {
								// キーボード
								switch (ev.value) {
								case 0: // up
									OnKeyUp(timestamp, ev.code);
									break;
								case 1: // down
									OnKeyDown(timestamp, ev.code);
									break;
								case 2: // repeat
									OnKeyRepeat(timestamp, ev.code);
									break;
								}
							} else {
								switch (ev.code & 0xfff0) {
								case BTN_MOUSE:
									mouseButton += ev;
									break;
								case BTN_WHEEL:
									wheelButton += ev;
									break;
								case BTN_GAMEPAD:
									gamepad += ev;
									break;
								}
							}
							break;
						case EV_REL:
							relativeReport.Event(ev.code, ev.value);
							break;
						case EV_ABS:
							absoluteReport.Event(ev.code, ev.value);
							break;
						default:
							break;
						}
					}
				}

				if (relativeReport) {
					// マウス移動はまとめて通知
					OnRelMoved(timestamp, relativeReport);
				}
				if (absoluteReport) {
					// まとめて通知
					OnAbsMoved(timestamp, absoluteReport);
				}
				if (mouseButton) {
					OnMouseButton(timestamp, mouseButton);
					mouseButton.Clear();
				}
				if (wheelButton) {
					OnWheel(timestamp, wheelButton);
					wheelButton.Clear();
				}
				if (gamepad) {
					OnGamepad(timestamp, gamepad);
					gamepad.Clear();
				}
			}
		}
	}
}
