#include <toolbox/tg/openvr.h>
#include <toolbox/tg/scenery.h>

#include <assert.h>
#include <unistd.h>
#include <syslog.h>



int main() {
	try {
		class VR : public TG::OpenVR {
			bool Finish() final {
				static unsigned n(0);
				return ++n < 1000;
			};
		} scene;
		scene.RegisterScenery(
			TG::Scenery::New("/home/tarosuke/Pictures/sceneries/Cubic30.png"));
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
