#include <toolbox/app.h>



int main(int argc, const char* argv[]) {
	class App : public TB::App {
		int Main() { return 0; };
	} app;
	return TB::App::main(app, argc, argv);
}
