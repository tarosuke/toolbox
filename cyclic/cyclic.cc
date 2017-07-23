/******************************************* timestamp / cyclic sleeper:cyclic
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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

#include <time.h>
#include <unistd.h>
#include <toolbox/cyclic/cyclic.h>

namespace TB{

	CYCLIC::CYCLIC(long ct) :
		prevTime(GetNanoStamp()),
		cycleTime(ct){
	}

	long CYCLIC::GetNanoStamp(){
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return ts.tv_nsec;
	}

	void CYCLIC::SleepTo(float ratio){
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
	}




	DURATION::DURATION() : prevTime(Now()){};

	double DURATION::GetDuration(){
		const double now(Now());
		const double duration(now - prevTime);
		prevTime = now;
		return duration;
	}

	double DURATION::Now(){
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return ts.tv_sec + 0.000000001 * ts.tv_nsec;
	}


}
