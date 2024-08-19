#pragma once
#include "EngineHeader.h"

class TimerUtil {
private:
	GLfloat Time{};
	bool StopState{};
	
public:
	void Update(float FT);
	void Stop();
	void Resume();
	void Reset();
	int Sec();
	GLfloat MiliSec(int DemicalPlace=1);
	void Interpolate(GLfloat Value);
};