#include "RandomUtil.h"

GLfloat RandomUtil::Gen(Dist DistType, GLfloat Min, GLfloat Max) {
	if (DistType == Dist::Real) {
		std::uniform_real_distribution URD{ Min, Max };
		return URD(RD);
	}

	else if (DistType == Dist::Int) {
		std::uniform_int_distribution UID{ (int)Min, (int)Max };
		return UID(RD);
	}
}