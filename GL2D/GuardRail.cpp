#include "GuardRail.h"

GuardRail::GuardRail() {
	SetImage(Image, "guardrail");
}

void GuardRail::Update(float FT) {
	// 카메라의 X좌표 위치와 높이를 가져와 가드레일이 카메라 가까이 위치한 것처럼 연출한다
	if (auto CamController = framework.Find("camera_controller"); CamController) {
		PositionX = CamController->GetPositionX() * 0.5;
		Height = CamController->GetHeight() * 0.2;
	}
}

void GuardRail::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height - 0.9);
	Scale(6.0, 3.0);
	RenderImage(Image, 1.0, 4500, 800);
}