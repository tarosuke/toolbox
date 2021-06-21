#include <toolbox/tg/glx.h>
#include <toolbox/tg/x.h>
#include <toolbox/tg/scenery.h>
#include <toolbox/tg/widget/root.h>
#include <toolbox/tg/widget/border.h>
#include <toolbox/tg/widget/cursor.h>

#include <assert.h>
#include <unistd.h>



int main() {
	XTG::Display display;
	assert(display);

	const TG::Scene::Frustum frustum = {
		left : -0.01,
		right : 0.01,
		bottom : -0.01,
		top : 0.01,
		near : 0.01,
		far : 100000.0
	};
	class S : public TG::GLXScene {
	public:
		S(XTG::Display& display,
		  unsigned width,
		  unsigned height,
		  const TG::Scene::Frustum& frustum)
			: TG::GLXScene(display, width, height, 0, frustum){};

	private:
		void HandleEvent(const XEvent& ev) final { Quit(); };
	} window(display, 640, 640, frustum);
	window.RegisterScenery(TG::Scenery::New(
		"sources/.mtests/"
		"kisspng-skybox-texture-mapping-cube-mapping-landscape-watercolor-"
		"sky-5ac85ff2676e48.2605566015230812024237.png"));

	assert(window.IsReady());
	TG::RootWidget root;
	TG::BorderWidget borderWidget(
		(const float[3]){-50.0f, -50.0f, 1.0f},
		(const unsigned[2]){100U, 100U},
		0x55333322);
	TG::Cursor::New("data/cursor.png");
	window.Run();
	return 0;
}
