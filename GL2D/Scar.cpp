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

		if (auto Target = framework.Find("target"); Target) {
			if (Player->GetLookDir() == 1)
				Rotation = atan2(Target->GetHeight() - ViewportY, Target->GetPositionX() - ViewportX) * (180 / 3.14);

			else if (Player->GetLookDir() == 0)
				Rotation = atan2(ViewportY - Target->GetHeight(), ViewportX - Target->GetPositionX()) * (180 / 3.14);
		}
	}
}

void Scar::Render() {
	if (auto Player = framework.Find("player"); Player) {
		BeginProcess(ImageRenderMode::Default);
		SetPosition(GunX, GunY);
		Scale(0.3, 0.3);
		Rotate(Rotation);

		if (Player->GetLookDir() == 0)
			FlipImage(Flip::Horizontal);

		RenderImage(Image, 1.0, 200, 100);

		ViewportX = ASP(ViewportPosition().x);
		ViewportY = ViewportPosition().y;
	}
}