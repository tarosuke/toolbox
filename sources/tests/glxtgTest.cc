#include <toolbox/tg/glxtg.h>
#include <toolbox/tg/x.h>

#include <assert.h>
#include <unistd.h>



int main() {
	XTG::Display display;
	assert(display);

	const TG::Scene::Frustum frustum = {-1, 1, 1, -1, 0.01, 100000.0};
	TG::GLXScene window(640, 480, 0, frustum);
	assert(window.IsReady());
	window.Draw();
	display.Run();
	return 0;
}
