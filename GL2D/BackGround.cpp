#include "BackGround.h"

BackGround::BackGround() {
	SetImage(Image, "background");
}

void BackGround::Update(float FT) {
	if (auto CamController = framework.Find("camera_controller"); CamController) {
		PositionX = CamController->GetPositionX() * 0.7;
		Height = CamController->GetHeight() * 0.7;
	}
}

void BackGround::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(-PositionX, -Height);
	Scale(4.0, 4.0);
	RenderImage(Image, 1.0, 4500, 2000);
}