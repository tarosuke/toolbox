/****************************************************************** 子プロセス
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <unistd.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <toolbox/child.h>


namespace TB{

	/** 子プロセス
	 */
	Child::Child(const char* const t[]) : pid(fork()){
		if(pid < 0){
			syslog(LOG_ERR, "Faild to fork(" __FILE__ ":%d)", __LINE__);
			return;
		}else if(pid){
			//親プロセスなので終了
			return;
		}
		//子プロセスなので指定されたものをexec
		execvp(t[0], const_cast<char* const*>(t));
		syslog(LOG_CRIT, "failed to start process:%s.", t[0]);
		_exit(-1);
	}

	Child::~Child(){
		waitpid(pid, 0, 0);
	}

	/** パイプ付き子プロセス
	 */
	PipedChild::PipedChild(const char* const t[]) : pipes(Pipe(*t, t)){}
	PipedChild::~PipedChild(){
		close(pipes.read);
		close(pipes.write);
		waitpid(pipes.pid, 0, 0);
	}

	/** 双方向通信のためにパイプを二つ作って返す
	 */
	PipedChild::Pipes PipedChild::Pipe(
		const char t[],
		const char* const* params){
		//戻り値(の入れ物)
		Pipes r = { -1, -1, -1 };

		/** 親→子のパイプ生成
		 * p2cHandles[0] ： 親では使わない／子で0番に複製
		 * p2cHandles[1] ： 親で書き込む／子では使わない
		 */
		int p2cHandles[2];
		if(pipe(p2cHandles)){
			syslog(LOG_ERR, "failed to create send pipe.");
			return r;
		}
		/** 子→親のパイプ生成
		 * c2pHandles[0] ： 親では読みだす／子では使わない
		 * c2pHandles[1] ： 親では使わない／子で1番に複製
		 */
		int c2pHandles[2];
		if(pipe(c2pHandles)){
			syslog(LOG_ERR, "failed to create receive pipe");
			close(p2cHandles[0]);
			close(p2cHandles[1]);
			return r;
		}

		//子プロセス起動
		const pid_t pid(fork());
		if(!pid){
			//子プロセスなので相手方パイプをハンドル番号0へ
			if(dup2(p2cHandles[0], 0) < 0 || dup2(c2pHandles[1], 1) < 0){
				syslog(LOG_CRIT, "failed to duplicate handles of pipe.");
				_exit(-1);
			}
			//不要ハンドルを閉じる
			close(p2cHandles[1]);
			close(c2pHandles[0]);

			//対象起動
			execvp(t, const_cast<char* const*>(params));

			//起動失敗
			syslog(LOG_CRIT, "failed to start process:%s.", t);
			_exit(-1);
		}else if(pid < 0){
			syslog(LOG_CRIT, "failed to fork.");
			return r;
		}

		//不要ハンドルを閉じる
		close(p2cHandles[0]);
		close(c2pHandles[1]);

		r.pid = pid;
		r.read = c2pHandles[0];
		r.write = p2cHandles[1];
		return r;
	}

}
