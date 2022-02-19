/************************************************************************* time
 * 時間関係のインフラ
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
#include <toolbox/time.h>

#include <time.h>



namespace TB {
	void TimeBase::Update() {
		timespec ts; //<-こいつを使いたくないがためだけに作った
		clock_gettime(CLOCK_BOOTTIME, &ts);
		*this = (u64)(nsec(ts.tv_nsec) + sec(ts.tv_sec));
	}


	void Timestamp::Update() {
		nsec nns;
		nns.Update();
		nns -= start;
		delta = nns - uptime;
		uptime = nns;
	}
}
