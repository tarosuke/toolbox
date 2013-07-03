/***************************************************** simple unit-test helper
 *
 */

#ifndef _TEST_
#define _TEST_

namespace TOOLBOX{

	class TEST{
	public:
		Assert(bool);
		AssertTolerance(double differ, double tolerance);
		AssertInRange(double value, double lo, double hi);
	};

};

#endif
