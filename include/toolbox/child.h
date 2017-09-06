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
#pragma once


namespace TB{

	class Child{
		Child();
		Child(const Child&);
		void operator=(const Child&);
	public:
		const long pid;

		Child(const char* const args[]);
		~Child();
	};

	class PipedChild{
		PipedChild();
		PipedChild(const PipedChild&);
		void operator=(const PipedChild&);
	public:
		PipedChild(const char* const args[]);
		~PipedChild();

		int ReadFd(){ return pipes.read; };
		int WriteFd(){ return pipes.write; };
	private:
		struct Pipes{
			int pid;
			int read;
			int write;
		};
		const Pipes pipes;
		static Pipes Pipe(const char[], const char* const*);
	};

}
