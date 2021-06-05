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
#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <toolbox/input/input.h>



namespace TB {

	Input::Input(bool grab) {
		for (unsigned n(0);; ++n) {
			char path[32];
			sprintf(path, "/dev/input/event%u", n);
			const int fd(open(path, O_RDWR | O_NONBLOCK));
			if (fd < 0) {
				return;
			}
			auto* const ev(new Event(*this, fd, grab));
			if (ev) {
				events.Add(*ev);
			}
		}
	}
	Input::~Input() {
		for (List<Event>::I i(events); ++i;) {
			delete i;
		}
	}
	void Input::GetInput() {
		for (List<Event>::I i(events); ++i;) {
			(*i).GetInput();
		}
	}

	Input::Event::Event(Input& input, int fd, bool grab)
		: input(input), fd(fd) {
		ioctl(fd, EVIOCGRAB, grab);
	}
	Input::Event::~Event() { ioctl(fd, EVIOCGRAB, 0); }
	void Input::Event::GetInput() {
		input_event ev;
		if (sizeof(ev) == read(fd, &ev, sizeof(ev))) {
			switch (ev.type) {
			case EV_KEY:
				switch (ev.code & 0xfff0) {
				case KEY_RESERVED: //キーボード
					switch (ev.value) {
					case 0: // up
						input.OnKeyUp(ev.code);
						break;
					case 1: // down
						input.OnKeyDown(ev.code);
						break;
					case 2: // repeat
						input.OnKeyRepeat(ev.code);
						break;
					}
					break;
				case BTN_MOUSE: //マウスボタン他
					switch (ev.value) {
					case 0: // up
						input.OnButtonUp(ev.code & 0xff);
						break;
					case 1: // down
						input.OnButtonDown(ev.code & 0xff);
						break;
					}
					break;
				}
				break;
			case EV_REL:
				if (ev.code < 3) {
					input.OnMouseMove(ev.code, ev.value);
				}
				break;
			default:
				break;
			}
		}
	}
}
