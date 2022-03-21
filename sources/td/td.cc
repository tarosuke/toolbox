/********************************************************** 3D -> ThreeD -> TD
 *  Copyright (C) 2021 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/td.h>
#include <toolbox/exception.h>



namespace TB {

	TD::M44 TD::Frustum(float width, float height, float near, float far) {
		return M44((const float[4][4]){
			{2 * near / width, 0, 0, 0},
			{0, 2 * near / height, 0, 0},
			{0, 0, far / far - near, 1},
			{0, 0, near * far / (near - far), 0}});
	}


}
