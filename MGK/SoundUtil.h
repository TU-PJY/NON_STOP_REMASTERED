#pragma once
#include "EngineHeader.h"
#include <map>
#include <string>

class SoundUtil {
private:
	FMOD::System* SoundSystem{};
	FMOD::DSP* BeatDetector{};
	FMOD::DSP* LowPass{};

	FMOD_VECTOR ListenerPosition{};
	FMOD_VECTOR SoundPosition{};

	FMOD_RESULT Result{};
	void* ExtDvData{};

public:
	void Init();
	void Import(Sound& Sound, const char* FileName, FMOD_MODE Option);
	void Release(Sound& Sound);
	void Update();
	void PlaySound(FMOD::Sound* Sound, FMOD::Channel*& ChannelVar, unsigned int Ms=0);
	void PauseSound(FMOD::Channel*& ChannelVar, bool Flag);
	void StopSound(FMOD::Channel*& ChannelVar);
	unsigned int GetLength(FMOD::Sound* Sound);
	unsigned int GetPlayTime(FMOD::Channel* ChannelVar);
	void SetPlaySpeed(FMOD::Channel*& ChannelVar, float Speed);
	void ResetPlaySpeed(FMOD::Channel*& ChannelVar);
	void SetFreqCutOff(FMOD::Channel*& ChannelVar, float Frequency);
	void SetBeatDetect(FMOD::Channel*& ChannelVar);
	float DetectBeat(float Threshold, float SamplingRate);
	bool IsBeat(float Threshold, float SamplingRate);
	void UnSetBeatDetect(FMOD::Channel*& ChannelVar);
	void UnSetFreqCutOff(FMOD::Channel*& ChannelVar);
	void SetDistance(FMOD::Channel*& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(FMOD::Channel*& ChannelVar, float X, float Y, float Diff);
};
extern SoundUtil soundUtil;
