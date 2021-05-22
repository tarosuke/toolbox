#include <toolbox/tg/glx.h>
#include <toolbox/tg/x.h>
#include <toolbox/tg/scenery.h>

#include <assert.h>
#include <unistd.h>



int main() {
	XTG::Display display;
	assert(display);

	const TG::Scene::Frustum frustum =
		{-0.01, 0.01, 0.01, -0.01, 0.01, 100000.0};
	TG::GLXScene window(640, 640, 0, frustum);
	assert(window.IsReady());
	window.Draw();
	display.Run();
	return 0;
}
