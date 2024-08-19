#include "PhysicsUtil.h"
#include <cmath>

void PhysicsUtil::SetFallingState() {
	FallingState = true;
}

void PhysicsUtil::AddGravityAcc(GLfloat AccValue) {
	GravityAcc += AccValue;
	FallingState = true;
}

void PhysicsUtil::Fall(GLfloat& Position, GLfloat Gravity, float FT) {
	if (FallingState) {
		GravityAcc -= Gravity * FT;
		Position += GravityAcc * FT;
	}
}

bool PhysicsUtil::IsOnFloor(GLfloat Position, GLfloat FloorHeight) {
	if (Position == FloorHeight)
		return true;

	return false;
}

void PhysicsUtil::LandFloor(GLfloat& Position, GLfloat FloorHeight) {
	if (GravityAcc < 0 && Position < FloorHeight) {
		Position = FloorHeight;
		GravityAcc = 0;
		FallingState = false;
	}
}

void PhysicsUtil::BounceFloor(GLfloat& Position, GLfloat FloorHeight, GLfloat RebounceValue, GLfloat Threshold) {
	if (GravityAcc < 0 && Position < FloorHeight && FallingState) {
		Position = FloorHeight;

		if (GravityAcc < 0 && fabs(GravityAcc) <= Threshold) {
			GravityAcc = 0;
			FallingState = false;
		}
		else if(GravityAcc < 0 && fabs(GravityAcc) > Threshold)
			GravityAcc *=  -RebounceValue;
	}
}

void PhysicsUtil::BounceWall(int& MoveDirection, GLfloat& Speed, GLfloat RebounceValue) {
	MoveDirection *= -1;
	Speed *= RebounceValue;
}

void PhysicsUtil::LerpAcc(GLfloat& Speed, int MoveDirection, GLfloat Dest, GLfloat AccValue, float FT) {
	Speed = std::lerp(Speed, Dest * MoveDirection, FT * AccValue);
}

void PhysicsUtil::LerpDcc(GLfloat& Speed, GLfloat Friction, float FT) {
	Speed = std::lerp(Speed, 0.0, FT * Friction);
}

void PhysicsUtil::LinearAcc(GLfloat& Speed, int MoveDirection, GLfloat Dest, GLfloat AccValue, float FT) {
	Speed += MoveDirection * AccValue * FT;

	if (MoveDirection > 0) {
		if (Speed >= Dest)
			Speed = Dest;
	}

	else if (MoveDirection < 0) {
		if (Speed <= Dest * MoveDirection)
			Speed = Dest * MoveDirection;
	}
}

void PhysicsUtil::LinearDcc(GLfloat& Speed, GLfloat Friction, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		if (Speed <= 0)
			Speed = 0;
	}

	else if (Speed < 0) {
		Speed += Friction * FT;
		if (Speed >= 0)
			Speed = 0;
	}
}

void PhysicsUtil::Mediate2DirSpeed(GLfloat& SpeedX, GLfloat& SpeedY) {
	GLfloat Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}