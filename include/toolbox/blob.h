/************************************************************************ BLOB
 *  Copyright (C) 2021,2022 tarosuke<webmaster@tarosuke.net>
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



/** BLOB
 * ビルド時にobjcopyで取り込んだバイナリを扱う
 * 例えば取り込んだファイルがDEBUG/td/vulkan/test.frag.spvだとすると
 * そのバイナリは_binary_DEBUG_td_vulkan_test_frag_spv_startのシンボルから
 * 始まっていて、同endの前で終わる。
 * DEBUGの部分はmakefileにて_BUILD_TARGET_マクロとして設定されているので
 * ソース上ではビルドターゲットに関わらず指せる。
 *
 * 以下のマクロは_td_vulkan_test_frag_spvの部分を与えて全体を作るものである
 *  BlobDeclare(td_vulkan_test_frag_spv);
 * のようにして宣言し、Blob(td_vulkan_test_frag_spv)で参照する。
 */
#define ___Blob(target, path, suffix) _binary_##target##_##path##suffix
#define __Blob(target, path, suffix) ___Blob(target, path, suffix)

#define BlobStart(path) __Blob(_BUILD_TARGET_, path, _start)
#define BlobEnd(path) __Blob(_BUILD_TARGET_, path, _end)
#define BlobSize(path) __Blob(_BUILD_TARGET_, path, _size)
#define BlobDeclare(path)                                                      \
	extern const char BlobStart(path)[];                                       \
	extern const char BlobEnd(path)[];                                         \
	extern const char BlobSize(path)[];
#define Blob(path)                                                             \
	{ BlobStart(path), BlobEnd(path) }
