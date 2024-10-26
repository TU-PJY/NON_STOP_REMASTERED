#pragma once

enum ClampType { 
	CLAMP_GREATER, 
	CLAMP_LESS
};

namespace EX {
	void SwitchBool(bool& Value);
	void SwitchValue(float& TargetValue, float Value1, float Value2);
	void ClampValue(float& Value, float Dest, int Type);
 }