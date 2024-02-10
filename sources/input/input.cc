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
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

#include <tb/input.h>
#include <tb/types.h>



namespace tb {

	const int Input::relDirs[] = {1, -1, 1, 1, 1, 1, 1, 1};

	Input::Input(msec outTime, bool grab) : outms((int)(u64)outTime) {
		for (unsigned n(0);; ++n) {
			char path[32];
			sprintf(path, "/dev/input/event%u", n);
			const int fd(open(path, O_RDWR | O_NONBLOCK));
			if (fd < 0) {
				return;
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
			for (auto& e : evs) {
				if (~e.revents & POLLIN) {
					continue;
				}
				input_event ev;
				while (sizeof(ev) == read(e.fd, &ev, sizeof(ev))) {
					switch (ev.type) {
					case EV_KEY:
						switch (ev.code & 0xff00) {
						case KEY_RESERVED: // キーボード
							switch (ev.value) {
							case 0: // up
								OnKeyUp(ev.code);
								break;
							case 1: // down
								OnKeyDown(ev.code);
								break;
							case 2: // repeat
								OnKeyRepeat(ev.code);
								break;
							}
							break;
						case BTN_MOUSE: // マウスボタン他
							switch (ev.value) {
							case 0: // up
								OnButtonUp(ev.code & 0xff);
								break;
							case 1: // down
								OnButtonDown(ev.code & 0xff);
								break;
							}
							break;
						}
						break;
					case EV_REL:
						if (ev.code < elementsOf(relDirs)) {
							OnMouseMove(ev.code, relDirs[ev.code] * ev.value);
						}
						break;
					case EV_ABS:
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
