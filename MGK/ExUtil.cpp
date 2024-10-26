#include "ExUtil.h"

void EX::SwitchBool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}

void EX::SwitchValue(float& TargetValue, float Value1, float Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX::ClampValue(float& Value, float Dest, int Type) {
	if ((Type == CLAMP_GREATER && Value > Dest) || (Type == CLAMP_LESS && Value < Dest))
		Value = Dest;
}