#include "EngineHeader.h"
#include "ImageUtil.h"

std::vector<ImageInfo> ImageList
{
	{"player", "res//sprite//player//player.png"},

	{"ground", "res//sprite//map//ground.png"},
	{"guardrail", "res//sprite//map//guardrail.png"},
	{"background", "res//sprite//map//background.png"},

	{"target", "res//sprite//ui//target.png"},

	{"scar_h", "res//sprite//gun//scar_h.png"},
	{"bullet_ar", "res//sprite//prop//bullet_ar.png"},
	{"casing", "res//sprite//prop//casing.png"},
	{"gun_flame", "res//sprite//prop//gun_flame.png"},
	{"gun_flame_blur", "res//sprite//prop//gun_flame_blur.png"},
};

std::vector<FileNameAndOption> SoundList
{
	{"scar_shoot", "res//sound//scar_h.wav"},
	{"scar_shoot_distance", "res//sound//scar_h_distance.wav"},
};

// global image
unsigned int CasingImage;
unsigned int GunFlameImage;
unsigned int BulletARImage;

// load global Image
void LoadGlobalImage() {
	CasingImage = imageUtil.SetGlobalImage(CasingImage, "casing");
	GunFlameImage = imageUtil.SetGlobalImage(GunFlameImage, "gun_flame");
	BulletARImage = imageUtil.SetGlobalImage(BulletARImage, "bullet_ar");
}