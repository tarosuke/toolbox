/**************************************************************** toolbox test
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
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <toolbox/string.h>


#define ManualTest                                                             \
	{                                                                          \
		if (getenv("AUTO_TEST")) {                                             \
			exit(0);                                                           \
		}                                                                      \
	}
#define assert(c)                                                              \
	if (!(c)) {                                                                \
		fprintf(stderr, "%s:%u:: info: %s.\n", __FILE__, __LINE__, #c);        \
	}
#define assertEQ(A, B)                                                         \
	{                                                                          \
		if ((A) != (B)) {                                                      \
			fprintf(                                                           \
				stderr,                                                        \
				"%s:%u:: info: %s != %s.\n",                                   \
				__FILE__,                                                      \
				__LINE__,                                                      \
				(const char*)TB::String(A),                                    \
				(const char*)TB::String(B));                                   \
		}                                                                      \
	}
#define assertNE(A, B, H)                                                      \
	{                                                                          \
		if ((A) == (B)) {                                                      \
			fprintf(                                                           \
				stderr,                                                        \
				"%s:%u:: info: %s == %s\n",                                    \
				__FILE__,                                                      \
				__LINE__,                                                      \
				(const char*)TB::String(A),                                    \
				(const char*)TB::String(B));                                   \
		}                                                                      \
	}
