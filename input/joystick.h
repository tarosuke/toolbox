/***************************************************** joystick input:joystick
 *
 */

#ifndef _JOYSTICK_
#define _JOYSTICK_


#include <pthread.h>

class JOYSTICK{
public:
	JOYSTICK();
	~JOYSTICK();
	float GetAxis(unsigned index) const{
		return index < maxAxis ?
			(1.0 / 32768) * axis[index] : 0.0;
	};
	unsigned GetButtons() const{ return buttons; };
private:
	static const unsigned maxAxis = 8;
	const int device;
	int OpenDevice();
	int axis[maxAxis];
	unsigned buttons;
	pthread_t sensorThread;
	static void* _JSThread(void*);
	void JSThread();
};

#endif
