#include "GuardRail.h"
#include "Framework.h"

GuardRail::GuardRail() {
	SetImage(Image, "guardrail");
}

void GuardRail::Update(float FT) {
	if (auto CamController = framework.Find("camera_controller"); CamController)
		PositionX = CamController->GetPosition() * 0.5;
}

void GuardRail::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, FLOOR_HEIGHT - 0.3);
	Scale(3.0, 3.0);
	RenderImage(Image, 1.0, 4500, 800);
}