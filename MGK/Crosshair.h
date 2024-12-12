#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"
#include "Scene.h"

class Crosshair : public GameObject {
private:
	// 크로스 헤어가 표시되는 위치
	glm::vec2 Position{};

	// 월드 상의 크로스헤어 위치
	glm::vec2 RealPosition{};

	// 랜덤 생성 타격 지점
	glm::vec2 TargetPoint{};

	// 반동 수치
	GLfloat Recoil{};

	// 선 그리기 클래스
	LineBrush line;

	// 격발 여부
	bool ShootState{};

public:
	Crosshair() {
		line.SetColor(1.0, 0.0, 0.0);

		// 카메라의 상태에 영향을 받지 않는다.
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// 크로스헤어에 반동을 준다
	void GiveRecoil(GLfloat Value) {
		Recoil += Value;
	}

	// 총알을 발사한다.
	void ShootGun() {
		// 랜덤한 지점을 생성한다.
		TargetPoint.x = Random::Gen(DIST_REAL, RealPosition.x - 0.03 - Recoil, RealPosition.x + 0.03 + Recoil);
		TargetPoint.y = Random::Gen(DIST_REAL, RealPosition.y - 0.03 - Recoil, RealPosition.y + 0.03 + Recoil);

		// monster 태그를 가진 범위를 얻는다.
		ObjectRange MonsterRange = scene.EqualRange("monster");

		// 범위 내에 있는 모든 오브젝트의 AABB 충돌 검사를 통해 대미지 부여 여부를 결정한다.
		for (auto It = MonsterRange.First; It != MonsterRange.End; ++It) {
			if (It->second->GetAABB().CheckCollisionPoint(TargetPoint.x, TargetPoint.y)) {
				int Damage{};

				// 플레이어의 총에 따라 대미지가 다르게 부여된다.
				if (PlayerGunType == "SCAR_H")
					Damage = 25;

				else if(PlayerGunType == "M16")
					Damage = 18;

				else if(PlayerGunType == "MP44")
					Damage = 35;

				else if (PlayerGunType == "MG42")
					Damage = 10;

				It->second->GiveDamage(Damage);

				// 대미지를 받은 몬스터 리스트에 아이디 및 대미지 추가 
				DamageMonsterList.push_back({ It->second->GetID(), Damage });
			}
		}
	}

	void UpdateFunc(float FT) {
		// 크로스 헤어가 표시되는 위치 지정
		Position.x = mouse.x + cameraCon.Shake.x;
		Position.y = mouse.y + cameraCon.Shake.y - cameraCon.PushValue;

		// 월드 상의 크로스헤어 위치 지정
		RealPosition.x = mouse.x - cameraCon.Position.x;
		RealPosition.y = mouse.y;

		// 반동 수치를 점차 줄인다
		Recoil -= FT * 0.5;

		// Recoil이 0.0 미만으로 감소하지 않도록 한다.
		EX::ClampValue(Recoil, 0.0, CLAMP_LESS);
	}

	void RenderFunc() {
		// left
		line.Draw(Position.x - 0.15 - Recoil, Position.y, Position.x - 0.03 - Recoil, Position.y, 0.015);
		// right
		line.Draw(Position.x + 0.03 + Recoil, Position.y, Position.x + 0.15 + Recoil, Position.y, 0.015);
		// upper
		line.Draw(Position.x, Position.y + 0.03 + Recoil, Position.x, Position.y + 0.15 + Recoil, 0.015);
		// under
		line.Draw(Position.x, Position.y - 0.15 - Recoil, Position.x, Position.y - 0.03 - Recoil, 0.015);
	}
};