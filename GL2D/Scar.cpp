#include "Scar.h"
#include <cmath>
#include "MouseUtil.h"
#include <iostream>

Scar::Scar() {
	SetImage(Image, "scar_h");
}

void Scar::Update(float FT) {
	if (auto Player = framework.Find("player"); Player) {
		GunX = Player->GetPositionX();
		GunY = Player->GetHeight();
	}

	if (auto Target = framework.Find("target"); Target)
		LookAt(ViewportX, ViewportY, Target->GetPositionX(), Target->GetHeight(), Rotation, 20, FT);
}

void Scar::Render() {
	if (auto Player = framework.Find("player"); Player) {
		BeginProcess(ImageRenderMode::Default);
		SetPosition(GunX, GunY);
		Scale(0.3, 0.3);

		if (Player->GetLookDir() == 0) {
			Rotate(Rotation - 90);
			FlipImage(Flip::Horizontal);
		}

		else
			Rotate(Rotation + 90);

		RenderImage(Image, 1.0, 200, 100);

		ViewportX = ASP(ViewportPosition().x);
		ViewportY = ViewportPosition().y;
	}
}