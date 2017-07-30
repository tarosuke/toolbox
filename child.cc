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

	Child::Child(const char* t[]) : pid(fork()){
		if(pid < 0){
			syslog(LOG_ERR, "Faild to fork(%s:%d)", __FILE__, __LINE__);
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
}
