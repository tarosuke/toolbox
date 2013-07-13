/****************************************** OpenGL adapter of quoternion:glqon
 *
 */
#ifndef _GLQON_
#define _GLQON_

#include <qon/qon.h>

class GLQON{
public:
	GLQON(const QON& q) : qon(q){}
	void GetModel();
	void GetView();
private:
	const QON& qon;
};

class GLVQON{
public:
	GLVQON(const VQON& v) : vqon(v){}
	void GetModel();
	void GetView();
private:
	const VQON& vqon;
};


#endif
