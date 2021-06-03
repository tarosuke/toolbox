/************************************************************ toolbox graphics
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
#include <toolbox/tg/widget.h>
#include <toolbox/prefs.h>

#include <math.h>


namespace TG {

	void Widget::Draw() { subs.Foreach(&Widget::Draw); };
	void Widget::Traw() { subs.Reveach(&Widget::Traw); };
	void Widget::Tick() { subs.Foreach(&Widget::Tick); };

	Widget::Found Widget::Find(const Query& q) {
		Found found;
		for (TB::List<Widget>::I i(subs); ++i;) {
			const Found f((*i).Find(q));
			if (f.widget && f.depth < found.depth) {
				found = f;
			}
		}
		return found;
	}
}
