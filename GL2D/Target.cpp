#include "Target.h"
#include "MouseUtil.h"

GLfloat Target::GetPositionX() {
	return PositionX;
}

GLfloat Target::GetHeight() {
	return PositionY;
}

Target::Target() {
	SetImage(Image, "target");
}

void Target::Update(float FT) {
	PositionX += mouse.x;
	PositionY += mouse.y;

	if (PositionX > ASP(1.0))
		PositionX = ASP(1.0);

	if (PositionX < ASP(-1.0))
		PositionX = ASP(-1.0);

	if (PositionY > 1.0)
		PositionY = 1.0;

	if (PositionY < -1.0)
		PositionY = -1.0;
}

void Target::Render() {
	BeginProcess(ImageRenderMode::Static);
	Scale(0.3, 0.3);
	SetPosition(PositionX, PositionY);
	RenderImage(Image);
}