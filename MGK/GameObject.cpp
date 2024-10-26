#include "GameObject.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include <cmath>

void GameObject::InitMatrix(int RenderType) {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	ImageAspectMatrix = glm::mat4(1.0f);
	FlipMatrix = glm::mat4(1.0f);

	TransparencyValue = 1.0f;
	BlurValue = 0.0;

	camera.SetCamera(RenderType);
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColor(glm::vec3 Color) {
	ObjectColor.r = Color.r;
	ObjectColor.g = Color.g;
	ObjectColor.b = Color.b;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

GLfloat GameObject::ASP(GLfloat Value) {
	return Value * ASPECT;
}

void GameObject::UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect) {
	if (ApplyAspect)
		ValueX = ASP(ViewportPosition().x);
	else
		ValueX = ViewportPosition().x;
	ValueY = ViewportPosition().y;
}

void GameObject::UpdateViewportPosition(glm::vec2& Position, bool ApplyAspect) {
	if (ApplyAspect) {
		Position.x = ASP(ViewportPosition().x);
		Position.y = ViewportPosition().y;
	}
	else
		Position = ViewportPosition();
}

void GameObject::UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY) {
	ValueX = LocalPosition().x;
	ValueY = LocalPosition().y;
}

void GameObject::UpdateLocalPosition(glm::vec2& Position) {
	Position = LocalPosition();
}

void GameObject::Flip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_H:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		break;

	case FLIP_V:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;

	case FLIP_HV:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;
	}
}

void GameObject::Blur(int Strength) {
	BlurValue = (GLfloat)Strength;
}

void GameObject::Render(Image& Image, GLfloat Transparency, bool DisableAdjustAspect) {
	TransparencyValue = Transparency;

	if (!DisableAdjustAspect) {
		if (Image.Width > Image.Height) {
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3(1.0, (GLfloat)Image.Height / (GLfloat)Image.Width, 1.0));
			ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix * ImageAspectMatrix;
		}
		else if (Image.Width < Image.Height) {
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3((GLfloat)Image.Width / (GLfloat)Image.Height, 1.0, 1.0));
			ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix * ImageAspectMatrix;
		}
		else
			ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
	}

	else
		ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;


	if (FlipMatrix != glm::mat4(1.0f))
		ResultMatrix *= FlipMatrix;

	PrepareRender();
	imageUtil.Render(Image);
}

void GameObject::PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime) {
	soundUtil.PlaySound(Sound, ChannelVar, StartTime);
}

void GameObject::PauseSound(SoundChannel& ChannelVar, bool Flag) {
	soundUtil.PauseSound(ChannelVar, Flag);
}

void GameObject::StopSound(SoundChannel& ChannelVar) {
	soundUtil.StopSound(ChannelVar);
}

void GameObject::SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed) {
	soundUtil.SetPlaySpeed(ChannelVar, PlaySpeed);
}

void GameObject::ResetPlaySpeed(SoundChannel& ChannelVar) {
	soundUtil.ResetPlaySpeed(ChannelVar);
}

void GameObject::EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency) {
	soundUtil.SetFreqCutOff(ChannelVar, Frequency);
}

void GameObject::EnableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.SetBeatDetect(ChannelVar);
}

void GameObject::DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate) {
	Value =  soundUtil.DetectBeat(ThresHold, SamplingRate);
}

bool GameObject::IsBeat(float ThresHold, float SamplingRate) {
	return soundUtil.IsBeat(ThresHold, SamplingRate);
}

void GameObject::DisableFreqCutoff(SoundChannel& ChannelVar) {
	soundUtil.UnSetFreqCutOff(ChannelVar);
}

void GameObject::DisableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.UnSetBeatDetect(ChannelVar);
}

void GameObject::SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	soundUtil.SetDistance(ChannelVar, MinDist, MaxDist);
}

void GameObject::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void GameObject::SetListnerPosition(glm::vec2 Position) {
	soundUtil.SetListnerPosition(Position.x, Position.y);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, X, Y, Diff);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, Position.x, Position.y, Diff);
}

////////////////////////// private
void GameObject::PrepareRender() {
	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(ImageTransparencyLocation, TransparencyValue);
	glUniform3f(ImageColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (BlurValue > 0.0) {
		glUniform1i(BoolBlurLocation, 1);
		glUniform1f(BlurStrengthLocation, BlurValue);
		glUniform2f(TexelSizeLocation, ASP(1.0) / (GLfloat)WIDTH, 1.0 / (GLfloat)HEIGHT);
	}
	else  
		glUniform1i(BoolBlurLocation, 0);

	glUniformMatrix4fv(ImageModelLocation, 1, GL_FALSE, value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	return camera.Projection * camera.ViewMatrix * ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}