/******************************************* timestamp / cyclic sleeper:cyclic
 *
 */
#include <time.h>
#include <unistd.h>
#include <toolbox/cyclic/cyclic.h>


CYCLIC::CYCLIC(long ct, long pm) :
	prevTime(GetNanoStamp()),
	cycleTime(ct),
	preMargin(pm){
}

long CYCLIC::GetNanoStamp(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_nsec;
}

void CYCLIC::Sleep(){
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


