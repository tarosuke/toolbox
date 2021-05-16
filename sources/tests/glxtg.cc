#include <toolbox/tg/glxtg.h>
#include <toolbox/tg/x.h>

#include <assert.h>
#include <unistd.h>



int main() {
	XTG::Display display;
	assert(display);
	XTG::Window window(display, 640, 480);
	assert(window);
	display.Run();
	//失敗時assertするか非0で返る
	// assert(false);
	return 0;
}
