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
	PipedChild::PipedChild(const char* const t[]) :
		pid(fork()),
		writeFd(pipes.p2c[1]),
		readFd(pipes.c2p[0]),
		pipes(Pipe()){
		if(pid < 0){
			syslog(LOG_ERR, "Faild to fork(" __FILE__ ":%d)", __LINE__);
			return;
		}else if(pid){
			//親プロセスなので余計なハンドルを閉じて終了
			close(pipes.p2c[0]);
			close(pipes.c2p[1]);
			return;
		}

		//ハンドルをstdin/outへ複製
		if(dup2(pipes.p2c[0], 0 < 0 || dup2(pipes.c2p[1], 1) < 0)){
			syslog(
				LOG_ERR,
				"failed to duplicate handles of pipe(" __FILE__ ":%d).",
				__LINE__);
			_exit(-1);
		}

		//余計なハンドルを閉じる
		close(pipes.p2c[1]);
		close(pipes.c2p[0]);

		//子プロセスなので指定されたものをexec
		execvp(t[0], const_cast<char* const*>(t));
		syslog(LOG_CRIT, "failed to start process:%s.", t[0]);
		_exit(-1);
	}

	PipedChild::~PipedChild(){
		waitpid(pid, 0, 0);
		close(writeFd);
		close(readFd);
	}

	/** 双方向通信のためにパイプを二つ作って返す
	 */
	PipedChild::Pipes PipedChild::Pipe(){
		Pipes p = { { -1, -1 }, { -1, -1 } };
		if(pipe(p.p2c)){
			syslog(
				LOG_ERR,
				"failed to create send pipe(" __FILE__ ":%d).",
				__LINE__);
			return p;
		}
		if(pipe(p.c2p)){
			syslog(
				LOG_ERR,
				"failed to create send pipe(" __FILE__ ":%d).",
				__LINE__);
			close(p.p2c[0]);
			close(p.p2c[1]);
			return p;
		}

		return p;
	}

}
