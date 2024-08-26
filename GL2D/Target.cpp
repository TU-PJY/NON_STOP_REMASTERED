#include "Target.h"
#include "MouseUtil.h"
#include "Framework.h"
#include <cmath>

GLfloat Target::GetPositionX() {
	return PositionX;
}

GLfloat Target::GetHeight() {
	return PositionY;
}

Target::Target() {
	SetImage(TargetImage, "target");
}

void Target::GiveRecoil(GLfloat Value) {
	PositionY += Value;
	PositionX += randomUtil.Gen(Dist::Real, -Value, Value);
}

void Target::Update(float FT) {
	// 마우스가 움직인 만큼 조준점을 움직인다
	PositionX += mouse.x;
	PositionY += mouse.y;

	// 조준점이 화면 밖으로 벗어나지 않도록 한다
	if (PositionX > ASP(1.0))
		PositionX = ASP(1.0);

	else if (PositionX < ASP(-1.0))
		PositionX = ASP(-1.0);

	if (PositionY > 1.0)
		PositionY = 1.0;

	else if (PositionY < -1.0)
		PositionY = -1.0;
}

void Target::Render() {
	// 조준점
	BeginProcess(ImageRenderMode::Static);
	Scale(0.3, 0.3);
	SetPosition(PositionX, PositionY);
	RenderImage(TargetImage);
;}