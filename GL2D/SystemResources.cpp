#include "EngineHeader.h"

std::vector<ImageInfo> SystemImageList
{
	{"BOUNDBOX", "mata_ENGINE_res//Image//Collision//Square.png"},
	{"BOUNDBOXCOLLISION", "mata_ENGINE_res//Image//Collision//Square Inside.png"},
	{"CIRCLE", "mata_ENGINE_res//Image////Collision//Circle.png"},
	{"CIRCLECOLLISION", "mata_ENGINE_res//Image////Collision//Circle Inside.png"},
	{"FMOD_LOGO", "mata_ENGINE_res//Image//Common//FMOD Logo.png"},
	{"mata_ENGINE_LOGO", "mata_ENGINE_res//Image//Common//mata_ENGINE Logo.png"},
};

std::vector<FileNameAndOption> SystemSoundList
{
	{"mata_ENGINE_LOGO_SOUND", "mata_ENGINE_res//Sound//mata_ENGINE Logo Sound.wav", FMOD_DEFAULT},
	{"mata_ENGINE_ERROR_SOUND", "mata_ENGINE_res//Sound//mata_ENGINE crash.wav", FMOD_DEFAULT}
};