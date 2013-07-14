/****************************************** OpenGL adapter of quoternion:glqon
 *
 */
#ifndef _GLQON_
#define _GLQON_

#include "qon.h"

class GLQON{
public:
	GLQON(const QON& q) : qon(q){}
	void GetModel() const;
	void GetView() const;
private:
	const QON& qon;
};

class GLVQON{
public:
	GLVQON(const VQON& v) : vqon(v){}
	void GetModel() const;
	void GetView() const;
private:
	const VQON& vqon;
};


#endif
