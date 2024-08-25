#include "Scar.h"
#include "MouseUtil.h"
#include "BulletAR.h"
#include "GunFlame.h"
#include "Casing.h"
#include <cmath>

Scar::Scar() {
	SetImage(Image, "scar_h");
}

void Scar::ShootGun() {
	TriggerActivated = true;
}

void Scar::StopShootGun() {
	TriggerActivated = false;
}

bool Scar::FindPtr() {
	if (!TargetPtr || !PlayerPtr) {
		PlayerPtr = framework.Find("player");
		TargetPtr = framework.Find("target");
		CameraPtr = framework.Find("camera_controller");
		return false;
	}

	return true;
}

void Scar::UpdateShoot(float FT) {
	if (Delay > 0)
		Delay -= FT;

	if (TriggerActivated) {
		if (Delay <= 0) {
			framework.AddObject(new BulletAR(Rotation + 90,
				GunX + cos((Rotation + 90) * (3.14 / 180)) * 0.3,
				GunY + sin((Rotation + 90) * (3.14 / 180)) * 0.3,
				10.0, Dir),
				"bullet_ar", Layer::L2);

			framework.AddObject(new GunFlame(Rotation + 90,
				GunX + cos((Rotation + 90) * (3.14 / 180)) * 0.3,
				GunY + sin((Rotation + 90) * (3.14 / 180)) * 0.3),
				"gun_flame", Layer::L2);

			framework.AddObject(new Casing(GunX, GunY, Dir), "casing", Layer::L2);

			RecoilRotation += 10;
			RecoilPositionX += cos((Rotation + 90) * (3.14 / 180)) * 0.1;
			RecoilPositionY += sin((Rotation + 90) * (3.14 / 180)) * 0.1;


			Delay = 0.1;

			if (TargetPtr)
				TargetPtr->GiveRecoil(0.07);

			if (CameraPtr)
				CameraPtr->ShakeCamera(0.5);
		}
	}
}

void Scar::UpdateRecoil(float FT) {
	if (RecoilRotation > 0) {
		RecoilRotation -= FT * 100;
		if (RecoilRotation <= 0)
			RecoilRotation = 0;
	}

	if (RecoilPositionX > 0) {
		RecoilPositionX -= FT;
		if (RecoilPositionX <= 0)
			RecoilPositionX = 0;
	}

	else if (RecoilPositionX < 0) {
		RecoilPositionX += FT;
		if (RecoilPositionX >= 0)
			RecoilPositionX = 0;
	}

	if (RecoilPositionY > 0) {
		RecoilPositionY -= FT;
		if (RecoilPositionY <= 0)
			RecoilPositionY = 0;
	}

	else if (RecoilPositionY < 0) {
		RecoilPositionY += FT;
		if (RecoilPositionY >= 0)
			RecoilPositionY = 0;
	}
}

void Scar::Update(float FT) {
	if (FindPtr()) {
		GunX = PlayerPtr->GetPositionX();
		GunY = PlayerPtr->GetHeight();
		Dir = PlayerPtr->GetLookDir();

		LookAt(ViewportX, ViewportY, TargetPtr->GetPositionX(), TargetPtr->GetHeight(), Rotation, 0, FT);
	}

	UpdateShoot(FT);
	UpdateRecoil(FT);
}

void Scar::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(GunX - RecoilPositionX, GunY - RecoilPositionY);
	Scale(0.3, 0.3);

	if (Dir == 0) {
		Rotate(Rotation - 90 - RecoilRotation);
		FlipImage(Flip::Horizontal);
	}

	else
		Rotate(Rotation + 90 + RecoilRotation);

	RenderImage(Image, 1.0, 200, 100);

	ViewportX = ASP(ViewportPosition().x);
	ViewportY = ViewportPosition().y;
}