#include "BulletAR.h"

BulletAR::BulletAR(GLfloat RotationValue, GLfloat XPos, GLfloat YPos, GLfloat Speed, int DirValue) {
	Rotation = RotationValue;
	BulletX = XPos;
	BulletY = YPos;
	MoveSpeed = Speed;
	Dir = DirValue;
}

void BulletAR::Update(float FT) {
	MoveForward(BulletX, BulletY, MoveSpeed, Rotation, FT);
	if (BulletX < -9.0 || BulletX > 9.0 || BulletY < -9.0 || BulletY > 9.0)
		framework.DeleteSelf(this);
}

void BulletAR::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(BulletX, BulletY);
	Rotate(Rotation);
	Scale(0.2, 0.2);
	RenderImage(BulletARImage);
}
