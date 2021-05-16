// テストドライバ

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>



int main(int argc, const char* argv[]) {
	for (int n(1); n < argc; ++n) {
		if (!strcmp(argv[0], argv[n])) {
			//自分はスキップ
			continue;
		}

		printf("%s...", argv[n]);

		//単純にargc[n]を実行して結果を待つだけ
		const int pid = fork();
		if (!pid) {
			//ここは子プロセス
			execl(argv[n], argv[n], 0);
			fprintf(stderr, "Failed to execute:%s\n", argv[n]);
			_exit(-1);
		} else if (pid < 0) {
			fprintf(stderr, "Failed to fork. ABORT\n");
			return -1;
		}

		//子プロセスを待つ
		int status;
		wait(&status);
		printf(
			"[%s]\n",
			WIFEXITED(status) && !WEXITSTATUS(status) ? "OK" : "NG");
	}
	return 0;
}
