#pragma once
#include "Framework.h"

class Scar : public GameObject {
private:
	Image Image{};
	GLfloat Rotation{}, RecoilRotation{};


	GLfloat GunX{}, GunY{};
	GLfloat ViewportX{}, ViewportY{};
	GLfloat RecoilPositionX{}, RecoilPositionY{};

	bool TriggerActivated{}, ShootActivated{};
	GLfloat Delay{};

	int Dir{};

	GameObject* TargetPtr{};
	GameObject* PlayerPtr{};
	GameObject* CameraPtr{};

public:
	Scar();
	void ShootGun();
	void StopShootGun();
	void UpdateShoot(float FT);
	void UpdateRecoil(float FT);
	bool FindPtr();
	void Update(float FT);
	void Render();
};

