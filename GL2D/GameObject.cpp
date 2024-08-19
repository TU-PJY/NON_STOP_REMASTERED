#include "GameObject.h"
#include "ImageUtil.h"
#include "RenderModeUtil.h"
#include "CameraUtil.h"

void GameObject::SetPosition(GLfloat X, GLfloat Y) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void GameObject::Rotate(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
}

void GameObject::RotateHorziontal(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(1.0, 0.0, 0.0));
}

void GameObject::RotateVertical(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 1.0, 0.0));
}

void GameObject::Scale(GLfloat X, GLfloat Y) {
	ScaleMatrix = scale(ScaleMatrix, glm::vec3(X, Y, 0.0));
}

void GameObject::RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY) {
	RotateMatrix = translate(RotateMatrix, glm::vec3(AxisX, AxisY, 0.0));
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
	RotateMatrix = translate(RotateMatrix, glm::vec3(-AxisX, -AxisY, 0.0));
}

void GameObject::MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT) {
	Position += Speed * MoveDirection * FT;
}

void GameObject::MoveStraight(GLfloat& Position, GLfloat Speed, float FT) {
	Position += Speed * FT;
}

void GameObject::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * MoveDirection * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * MoveDirection * FT;
	}
}

void GameObject::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * FT;
	}
}

void GameObject::LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT) {
	GLfloat targetAngle{}, shortestAngle{};
	targetAngle = atan2(ToY - FromY, ToX - FromX) * (180 / 3.14) - 90;
	targetAngle = NormalizeDegree(targetAngle);

	if (RotationSpeed > 0)
		shortestAngle = std::lerp(shortestAngle, CalculateShortestRotation(RotationVar, targetAngle), FT * RotationSpeed);
	else
		shortestAngle = CalculateShortestRotation(RotationVar, targetAngle);

	RotationVar = NormalizeDegree(RotationVar + shortestAngle);
}

void GameObject::LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT) {
	GLfloat targetAngle{}, shortestAngle{};
	targetAngle = NormalizeDegree(Rotation);

	if (RotationSpeed > 0)
		shortestAngle = std::lerp(shortestAngle, CalculateShortestRotation(RotationVar, targetAngle), FT * RotationSpeed);
	else
		shortestAngle = CalculateShortestRotation(RotationVar, targetAngle);

	RotationVar = NormalizeDegree(RotationVar + shortestAngle);
}

GLfloat GameObject::ASP(GLfloat Value) {
	return Value * ASPECT;
}

glm::vec4 GameObject::ViewportPosition() {
	glm::mat4 Result = TranslateMatrix * RotateMatrix * ScaleMatrix;
	glm::vec4 Position = camera.Projection * camera.ViewMatrix * Result * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return Position;
}

void GameObject::BeginProcess(ImageRenderMode Mode) {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = 1.0f;

	if (Mode == ImageRenderMode::Static)
		renderMode.SetStaticImageMode();
	else
		renderMode.SetImageMode();
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}


void GameObject::SetImage(Image& Image, std::string ImageName) {
	imageUtil.SetImage(Image, ImageName);
}

void GameObject::FlipImage(Flip FlipOption) {
	if (FlipOption != static_cast<Flip>(-1)) {
		switch (FlipOption) {
		case Flip::Horizontal:
			RotateMatrix = rotate(RotateMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			break;

		case Flip::Vertical:
			RotateMatrix = rotate(RotateMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
			break;
		}
	}
}

void GameObject::RenderImage(Image Image, GLfloat Transparency, GLfloat ImageWidth, GLfloat ImageHeight) {
	if (ImageWidth != 0.0 && ImageHeight != 0.0)
		ScaleMatrix = scale(ScaleMatrix, glm::vec3(ImageWidth / ImageHeight, 1.0, 0.0));

	TransparencyValue = Transparency;

	ProcessTransform();
	imageUtil.Render(Image);
}


void GameObject::SetSound(Sound& Sound, std::string SoundName) {
	Sound = soundUtil.GetSound(SoundName);
}

void GameObject::PlaySound(Sound Sound, Channel& Channel, unsigned int MS) {
	soundUtil.PlaySound(Sound, Channel, MS);
}

void GameObject::PauseSound(Channel& Channel, bool Flag) {
	soundUtil.PauseSound(Channel, Flag);
}

void GameObject::StopSound(Channel& Channel) {
	soundUtil.StopSound(Channel);
}

void GameObject::SetPlaySpeed(Channel& Channel, float PlaySpeed) {
	soundUtil.SetPlaySpeed(Channel, PlaySpeed);
}

void GameObject::ResetPlaySpeed(Channel& Channel) {
	soundUtil.ResetPlaySpeed(Channel);
}

void GameObject::SetFreqCutOff(Channel& Channel, float Frequency) {
	soundUtil.SetFreqCutOff(Channel, Frequency);
}

void GameObject::SetBeatDetect(Channel& Channel) {
	soundUtil.SetBeatDetect(Channel);
}

void GameObject::DetectBeat(float Threshold, int SamplingRate) {
	soundUtil.DetectBeat(Threshold, SamplingRate);
}

void GameObject::UnsetFreqCutOff(Channel& Channel) {
	soundUtil.UnSetFreqCutOff(Channel);
}

void GameObject::UnsetBeatDetect(Channel& Channel) {
	soundUtil.UnSetBeatDetect(Channel);
}

void GameObject::SetChannelDistance(Channel& Channel, float MinDist, float MaxDist) {
	soundUtil.SetDistance(Channel, MinDist, MaxDist);
}

void GameObject::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void GameObject::SetSoundPosition(Channel& Channel, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(Channel, X, Y, Diff);
}


void GameObject::FirstColorClipping() {
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void GameObject::SecondColorClipping() {
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void GameObject::EndColorClipping() {
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_STENCIL_TEST);
}

void GameObject::FirstAlphaClipping() {
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void GameObject::SecondAlphaClipping() {
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void GameObject::EndAlphaClipping() {
	glDisable(GL_STENCIL_TEST);
}

////////////////////////// private

void GameObject::ProcessTransform() {
	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * RotateMatrix * ScaleMatrix));
}

GLfloat GameObject::NormalizeDegree(GLfloat Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

GLfloat GameObject::CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest) {
	float Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}