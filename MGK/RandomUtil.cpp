#include "RandomUtil.h"

std::random_device RD;

GLfloat Random::Gen(int DistType, GLfloat Min, GLfloat Max) {
	if (DistType == DIST_REAL) {
		std::uniform_real_distribution URD{ Min, Max };
		return URD(RD);
	}

	else if (DistType == DIST_INT) {
		std::uniform_int_distribution UID{ (int)Min, (int)Max };
		return UID(RD);
	}
}