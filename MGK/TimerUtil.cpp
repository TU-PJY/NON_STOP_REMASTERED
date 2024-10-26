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
	Time = 0.0;
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
	GLfloat OverTime = Time - Value;
	Time = OverTime;
}




void Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (GLdouble)BPM;
}

void Metronome::Update(float FT) {
	if (!StopState) {
		Time += FT;

		if (Time >= BeatInterval) {
			GLdouble OverTime = Time - BeatInterval;
			Time = OverTime;
			BeatState = true;
		}

		else
			BeatState = false;
	}
}

void Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void Metronome::Resume() {
	StopState = false;
}

void Metronome::Reset() {
	Time = 0.0;
}

bool Metronome::IsBeat() {
	return BeatState;
}