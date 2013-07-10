/***************************************************** joystick input:joystick
 *
 */

#ifndef _JOYSTICK_
#define _JOYSTICK_

class JOYSTICK{
public:
	struct STATUS{
		int x0
	};
	operator const STATUS(){ return status; };
	JOYSTICK();
	~JOYSTICK();
private:
	const int device;
	int OpenDevice();
	STATUS status;
};

#endif
