#pragma once
#include "GameObject.h"

class BackGround : public GameObject {
public:
	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, 0.0, 0.5);
		Transform::Scale(ScaleMatrix, 8.0, 8.0);
		Render(BackGroundImage);
	}
};