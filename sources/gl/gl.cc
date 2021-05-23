#include <syslog.h>

#include <toolbox/gl/gl.h>



namespace GL{

	bool ErrorCheck(){
		const GLenum code(glGetError());
		if(code){
			syslog(LOG_ERR, "GL return error:%d.\n", code);
			return false;
		}
		return true;
	}

}