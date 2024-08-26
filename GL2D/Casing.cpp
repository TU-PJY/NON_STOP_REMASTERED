#include "Casing.h"

Casing::Casing(GLfloat XPos, GLfloat YPos, int DirValue) {
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
		MoveSpeed *= 0.7;
		RotationSpeed *= 0.5;
		PlaySound(CasingHitSound, CasingChannel);
	}

	psUtil.BounceFloor(CasingPositionY, FLOOR_HEIGHT - 0.2, 0.7, 0.5);

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

	if (!psUtil.GetFallingState()) {
		RotationSpeed = 0;
		MoveSpeed = 0;

		timerUtil.Update(FT);
		if(timerUtil.MiliSec() >= 0.3)
			framework.DeleteSelf(this);
	}
}

void Casing::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(CasingPositionX, CasingPositionY);
	Rotate(Rotation);
	Scale(0.05, 0.05);

	// 짧은 시간에 매우 높은 빈도로 생성되는 객체이므로 성능 향상을 위해 하나의 전역 이미지를 돌려쓴다
	// EngineHeade.h에 전역 선언, ResourceList.cpp에 정의 되어있음
	RenderImage(CasingImage);
}