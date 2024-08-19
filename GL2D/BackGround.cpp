#include "BackGround.h"
#include "Framework.h"

BackGround::BackGround() {
	SetImage(Image, "background");
}

void BackGround::Update(float FT) {
	if (auto CamController = framework.Find("camera_controller"); CamController) {
		PositionX = CamController->GetPositionX() * 0.3;
		Height = CamController->GetHeight() * 0.3;
	}
}

void BackGround::Render() {
	BeginProcess(ImageRenderMode::Static);
	SetPosition(PositionX, Height);
	Scale(3.0, 3.0);
	RenderImage(Image, 1.0, 4500, 2000);
}