/************************************************************************* App
 * Copyright (C) 2021,2023 tarosuke<webmaster@tarosuke.net>
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

#include <tb/app.h>

#include <assert.h>
#include <stdexcept>
#include <syslog.h>



tb::App* tb::App::instance(0);
tb::Prefs<unsigned> tb::App::logLevel(
	"--logLevel", 1, 0, tb::CommonPrefs::nosave);

int main(int argc, char** argv) {
	int rv(0);
	tb::CommonPrefs::Keeper k(
		argc, (const char**)argv, tb::App::instance->Name());
	rv = tb::App::instance->Main((uint)argc - k, (const char**)argv + k);
	return rv;
}
