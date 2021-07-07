#include <toolbox/tg/vulkan/image.h>

#include <assert.h>
#include <unistd.h>
#include <syslog.h>



int main() {
	try {
		TB::VK::FrameBuffer fv(640, 480);

#if false
		class VR : public TG::OpenVR {
			bool Finish(unsigned tick) final { return tick < 1000; };
		} scene;

		TG::RootWidget root;

		for (unsigned n(0); n < 10; ++n) {
			new TG::BorderWidget(
				(const float[3]){-50.0f + 100 * n, -50.0f, 1.0f},
				(const unsigned[2]){100U, 100U},
				0x55333322);
		}
		TG::Cursor::New("data/cursor.png");
		scene.RegisterScenery(TG::Scenery::New(
			"sources/.mtests/"
			"kisspng-skybox-texture-mapping-cube-mapping-landscape-watercolor-"
			"sky-5ac85ff2676e48.2605566015230812024237.png"));
		scene.Run();
#endif
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
