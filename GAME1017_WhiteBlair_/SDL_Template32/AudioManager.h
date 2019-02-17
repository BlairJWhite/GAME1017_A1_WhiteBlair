#pragma once
#include <iostream>
#include <vector>
#include "SDL_mixer.h"
using namespace std;

class AudioManager
{
public:
	void SetMusicVolume(int v);
	void PlayMusic(int idx = 0, int loops = -1);
	void PlaySound(int idx, int channel = -1, int loops = 0);
	void LoadMusic(const char* s);
	void LoadSound(const char* s);
	void ToggleMusic();
	void ClearMusic();
	void ClearSounds();
	// Any other useful methods you want. I'll leave them to you.
private:
	vector<Mix_Music*> m_vMusic;
	vector<Mix_Chunk*> m_vSounds;
};