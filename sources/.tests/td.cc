#include <toolbox/test.h>
#include <toolbox/td/vulkan/td.h>

#include <stdio.h>
#include <unistd.h>



int main() {
	// ManualTest;

	try {
		// 初期設定
		struct TD : public TB::VK::XTD {
			TD(unsigned w, unsigned h, const TB::TD::M44& m) : XTD(w, h, m){};
			void Tick(const Timestamp& tick) {
				if (TB::sec(5) < tick.uptime) {
					Quit();
				}
			};

		private:
		} td(200, 200, TB::TD::Frustum(0.01, 0.01, 0.01, 100000));

		// ここいらでオブジェクトをtdへ登録
		// TB::VK::TD::Object object;
		// td.AddScenery(object);

		// 周回処理
		td.Cyclic();

	} catch (const char* message) { puts(message); }


	return 0;
}
