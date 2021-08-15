#include <unistd.h>
#include <toolbox/sound/alsa.h>



int main() {
	//ホワイトノイズ、ブラウンノイズをそれぞれ1秒間出力
	{
		TB::Sound::While white;
		TB::Sound::ALSATarget target(white);
		sleep(1);
	}
	{
		TB::Sound::Red red;
		TB::Sound::ALSATarget target(red);
		sleep(1);
	}
	return 0;
}
