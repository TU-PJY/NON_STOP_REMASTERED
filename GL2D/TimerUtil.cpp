#include "TimerUtil.h"
#include <cmath>

void TimerUtil::Update(float FT) {
	if(!StopState)
		Time += FT;
}

void TimerUtil::Stop() {
	StopState = true;
}

void TimerUtil::Resume() {
	StopState = false;
}

void TimerUtil::Reset() {
	Time = 0;
}

int TimerUtil::Sec() {
	return (int)Time;
}

GLfloat TimerUtil::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void TimerUtil::Interpolate(GLfloat Value) {
	Time -= (Time - Value); 
}
