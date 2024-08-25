#include "Casing.h"

Casing::Casing(GLfloat XPos, GLfloat YPos, int DirValue) {
	SetImage(Image, "casing");
	CasingPositionX = XPos;
	CasingPositionY = YPos;

	MoveSpeed = randomUtil.Gen(Dist::Real, 0.6, 1.5);
	FallingAcc = randomUtil.Gen(Dist::Real, 1.2, 1.5);

	psUtil.AddGravityAcc(FallingAcc);
	Dir = DirValue;
}

void Casing::Update(float FT) {
	psUtil.Fall(CasingPositionY, 10.0, FT);

	if (psUtil.IsHitFloor(CasingPositionY, FLOOR_HEIGHT - 0.2)) {
		MoveSpeed *= 0.5;
		RotationSpeed *= 0.5;
	}

	psUtil.BounceFloor(CasingPositionY, FLOOR_HEIGHT - 0.2, 0.7, 0.1);

	if (MoveActivated) {
		if (Dir == 0) {
			CasingPositionX += MoveSpeed * FT;
			Rotation -= FT * RotationSpeed;
		}

		else {
			CasingPositionX -= MoveSpeed * FT;
			Rotation += FT * RotationSpeed;
		}
	}

	if(!psUtil.GetFallingState()) {
		MoveActivated = false;

		timerUtil.Update(FT);
		if (timerUtil.MiliSec() >= 0.5)
			framework.DeleteSelf(this);
	}
}

void Casing::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(CasingPositionX, CasingPositionY);
	Rotate(Rotation);
	Scale(0.05, 0.05);
	RenderImage(Image);
}