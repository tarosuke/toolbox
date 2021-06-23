#include <toolbox/tg/openvr.h>
#include <toolbox/tg/scenery.h>
#include <toolbox/tg/widget/root.h>
#include <toolbox/tg/widget/border.h>
#include <toolbox/tg/widget/cursor.h>

#include <assert.h>
#include <unistd.h>
#include <syslog.h>



int main() {
	try {
		class VR : public TG::OpenVR {
		public:
			bool Finish() final {
				static unsigned n(0);
				switch (++n) {
				case 1:
					(*pw).MoveTo(TB::Vector<3, float>({50, 50, 3}));
					break;
				case 200:
					(*pw).MoveTo(TB::Vector<3, float>({-50, -50, 1}));
					break;
				case 300:
					return false;
				}
				return true;
			};
			TG::PositionWidget* pw;
		} scene;
		TG::RootWidget root;
		TG::BorderWidget borderWidget(
			(const float[3]){-50.0f, -50.0f, 1.0f},
			(const unsigned[2]){100U, 100U},
			0x55333322);
		TG::Cursor::New("data/cursor.png");
		scene.RegisterScenery(TG::Scenery::New(
			"sources/.mtests/"
			"kisspng-skybox-texture-mapping-cube-mapping-landscape-watercolor-"
			"sky-5ac85ff2676e48.2605566015230812024237.png"));
		scene.pw = &borderWidget;
		scene.Run();
	} catch (const char* m) {
		syslog(LOG_CRIT, "Fatal error: %s.", m);
		return -1;
	} /* catch (TB::Exception& e) {
		 syslog(
			 LOG_CRIT,
			 "%s(%s)",
			 e.message,
			 vr::VR_GetVRInitErrorAsEnglishDescription(e.code));
		 return -1;
	 }*/
	catch (...) {
		syslog(LOG_CRIT, "Unknown errer(uncaught exception)");
		return -1;
	}
	return 0;
}
