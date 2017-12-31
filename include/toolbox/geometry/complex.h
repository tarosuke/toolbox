/** Complexes
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
 *
 * dim: dimensions
 *     2: a + ib
 *     4: a + ib + jc + kd
 */
#pragma once



template<typename T, unsigned dim = 2> class Complex{
public:
	Complex(){ Normalize(); };

	void Normalize(){
		value[0] = 1;
		for(unsigned n(1); n < dim; ++n){
			value[n] = 0;
		}
	}

private:
	T value[dim];
};
