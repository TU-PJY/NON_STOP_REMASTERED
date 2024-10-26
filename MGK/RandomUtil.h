#pragma once
#include "EngineHeader.h"
#include <random>

enum Dist {
	DIST_REAL, 
	DIST_INT
};

namespace Random {
	GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
}