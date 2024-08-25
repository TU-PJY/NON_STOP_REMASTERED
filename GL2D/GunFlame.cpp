#include "GunFlame.h"
#include <cmath>

GunFlame::GunFlame(GLfloat RotationValue, GLfloat XPos, GLfloat YPos) {
	SetImage(Image, "gun_flame");
	Rotation = RotationValue;
	FlamePositionX = XPos;
	FlamePositionY = YPos;
}

void GunFlame::Update(float FT) {
	timerUtil.Update(FT);
	if(timerUtil.MiliSec(2) >= 0.05)
		framework.DeleteSelf(this);
}

void GunFlame::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(FlamePositionX, FlamePositionY);
	Rotate(Rotation);
	Scale(0.3, 0.3);
	RenderImage(Image);
}