#include <toolbox/tg/glx.h>
#include <toolbox/tg/x.h>
#include <toolbox/tg/scenery.h>

#include <assert.h>
#include <unistd.h>



int main() {
	XTG::Display display;
	assert(display);

	const TG::Scene::Frustum frustum = {
		left : -0.01,
		right : 0.01,
		top : 0.01,
		bottom : -0.01,
		near : 0.01,
		far : 100000.0
	};
	TG::GLXScene window(640, 640, 0, frustum);
	window.RegisterScenery(
		TG::Scenery::New("/home/tarosuke/Pictures/sceneries/Cubic30.png"));

	assert(window.IsReady());
	window.Draw();
	display.Run();
	return 0;
}
