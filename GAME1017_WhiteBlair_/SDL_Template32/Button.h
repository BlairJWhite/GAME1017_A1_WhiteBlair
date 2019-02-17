#pragma once
#include "SDL.h"

class Button
{
public:
	Button(const char* s, SDL_Rect d);
	~Button();
	void Update();
	void Render();
	bool Clicked() { return m_iFrame == 2; }

private:
	enum state { MOUSE_UP, MOUSE_OVER, MOUSE_DOWN };
	bool m_bReleased;
	int m_iFrame;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst;
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;
};