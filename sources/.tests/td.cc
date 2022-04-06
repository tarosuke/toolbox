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
		static unsigned char texture[16][16][4];
		for (unsigned v(0); v < 16; ++v) {
			for (unsigned u(0); u < 16; ++u) {
				texture[v][u][0] = v * 16 + u;
				texture[v][u][1] = u * 16 + v;
				texture[v][u][2] = 127;
				texture[v][u][3] = 0;
			}
		}
		auto& layer(td.FindLayer("scenery"));
		TB::VK::TD::Layer::Object object(
			{{{0.0f, -0.5f, 1.0f}, {1.0f, 0.0f}},
			 {{0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}},
			 {{-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}}},
			{0, 1, 2},
			{16, 16, texture});
		layer.Add(object);



		// 周回処理
		td.Cyclic();

	} catch (const char* message) { puts(message); }


	return 0;
}
