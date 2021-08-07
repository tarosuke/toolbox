/********************************************************************* version
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

#include <toolbox/string.h>



namespace TB {

	template <unsigned REVs = 3> struct Version {
		Version() = delete;
		Version(const Version&) = default;
		Version(const unsigned (&t)[REVs]) {
			for (unsigned n(0); n < REVs; ++n) {
				rev[n] = t[n];
			}
		};
		Version(const String& o) { *this = o; };
		Version(const char* o) : Version(String(o)){};
		Version& operator=(const Version&) = default;
		void operator=(const String& o) {
			auto v(o.Split("."));
			for (unsigned n(0); n < v.Length() && n < REVs; ++n) {
				rev[n] = v[n].ToU();
			}
		};
		void operator=(const char* o) { *this = String(o); };

		bool operator==(const Version& t) const { return Compare(t, EQ); };
		bool operator!=(const Version& t) const { return !(*this == t); };
		bool operator<(const Version& t) const { return Compare(t, LT); };
		bool operator<=(const Version& t) const { return Compare(t, LT); };
		bool operator>(const Version& t) const { return !((*this) <= t); };
		bool operator>=(const Version& t) const { return !((*this) < t); };

		operator String() const {
			String r;
			for (unsigned n(0); n < REVs; ++n) {
				r << rev[n];
				if (n < REVs - 1) {
					r += '.';
				}
			}
			return r;
		};

	private:
		unsigned rev[REVs];
		static bool EQ(unsigned a, unsigned b) { return a == b; };
		static bool LT(unsigned a, unsigned b) { return a < b; };
		static bool ELT(unsigned a, unsigned b) { return a <= b; };
		bool Compare(const Version& t, bool (*op)(unsigned, unsigned)) const {
			for (unsigned n(0); n < REVs; ++n) {
				if (op(rev[n], t.rev[n])) {
					return false;
				}
			}
			return true;
		};
	};
}
