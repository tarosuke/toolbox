/********************************************************************** sounds
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
#include <toolbox/sound/sound.h>

#include <stdio.h>



namespace TB {
	namespace Sound {

	}
}


#if 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_Z 16

double z[MAX_Z];
double k[MAX_Z];

double pinkfilter(double in) {
	extern double z[MAX_Z];
	extern double k[MAX_Z];
	static double t = 0.0;
	double q;
	int i;

	q = in;
	for (i = 0; i < MAX_Z; i++) {
		z[i] = (q * k[i] + z[i] * (1.0 - k[i]));
		q = (q + z[i]) * 0.5;
	}
	return (t = 0.75 * q + 0.25 * t); /* add 1st order LPF */
}

void init_pink() {
	extern double z[MAX_Z];
	extern double k[MAX_Z];
	int i;

	for (i = 0; i < MAX_Z; i++)
		z[i] = 0;
	k[MAX_Z - 1] = 0.5;
	for (i = MAX_Z - 1; i > 0; i--)
		k[i - 1] = k[i] * 0.25;
}

int main() {
	int i, j;
	double n;

	init_pink();
	srandomdev();
	for (i = 0; i < 65536; i++) {
		printf("%.8g\n", pinkfilter(random() & 1 ? 1.0 : -1.0));
	}
}
#endif
