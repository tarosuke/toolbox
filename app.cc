/************************************************************************* App
 * Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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
 *
 ** アプリケーションのフレームワーク
 */

#include <toolbox/app.h>
#include <toolbox/prefs.h>
#include <toolbox/path.h>

#include <syslog.h>
#include <assert.h>



namespace{
	TB::Prefs<unsigned> logLevel("--verbose", 1, TB::CommonPrefs::nosave);
	void (*callRun)();
	void (*callFinally)();
}

namespace TB{

	App* App::instance(0);

	App::App(){
		assert(!instance);
		instance = this;
		callRun = &CallRun;
		callFinally = &CallFinally;
	}

	void App::CallRun(){
		assert(instance);
		(*instance).Run();
	}

	void App::CallFinally(){
		assert(instance);
		(*instance).Finally();
	}

}

int main(int argc, const char *argv[]){
	TB::Path projectName(TB::Path::Base(argv[0]));

	//syslogを準備する
	static const int logLevels[] = { LOG_CRIT, LOG_INFO, LOG_DEBUG };
	if(2 < logLevel){ logLevel = 2; }
	openlog(0, LOG_CONS, LOG_SYSLOG);
	syslog(LOG_INFO, projectName);

	//設定ファイルのパスを作る
	TB::String prefsPath(".");
	prefsPath += projectName;

	//設定ファイル読み込み／コマンドラインオプション取得
	TB::CommonPrefs::Keeper prefs(prefsPath, argc, argv);

	//コマンドラインオプションに従ってログレベルを設定
	const unsigned logMask(LOG_UPTO(logLevels[logLevel]));
	setlogmask(logMask);

	int rc(0);
	try{
		assert(callRun);
		(*callRun)();
	}
	catch(int returnCode){
		rc = returnCode;
	}
	catch(...){}

	if(callFinally){
		(*callFinally)();
	}

	return rc;
}
