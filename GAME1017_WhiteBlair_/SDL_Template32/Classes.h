#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#define OTILES 8
#define HTILES 1
#define ROWS 24
#define COLS 32
using namespace std;

/*							Txt char to Tile Legend:
================================================================================
|  * = Empty Tile.	    , = Light green grass      ; = Dark green grass (weeds)|
|																			   |
|  M = Rocks (big)      m = Rocks (small)		   . = Dirt Road               |
|																			   |
|  - = (dash) ????		~ = Water				   X = Lava					   |
|																			   |
|  # = Leafy Foliage    O = Tree Stump			   = = (equals) ?????		   |
|																			   |
|  H = ?????			B = Stone Wall			   _ = (underscore)	Stone Floor|
|																			   |
|  Q = Door				% = Bones				                               |
================================================================================*/

constexpr char g_cOTiles[OTILES] = { 'M', 'm', '#', 'O', '=', 'H', 'B', '%' };
constexpr char g_cHTiles[HTILES] = { 'X' };

class Sprite
{
public:
	char m_cOutput;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst;
	// Add more comments like this
	// Don't store texture and renderer as pointers on EVERY sprite
	SDL_Texture* m_pTexture;
	SDL_Renderer* m_pRenderer;
	Sprite() {}
	~Sprite() {}
	void SetX(int x) { m_rDst.x = x * 32; }
	void SetY(int y) { m_rDst.y = y * 32; }
	void SetImage(SDL_Renderer* r, SDL_Texture* t)
	{
		m_pRenderer = r;
		m_pTexture = t;
	}
};

class LOTile : public Sprite
{
public:
	LOTile() {}
	LOTile(int x, int y)
	{
		SetX(x);
		SetY(y);
	}
};
class Item : public Sprite
{
public:
	Item(int x, int y)
	{
		m_rDst.x = x;
		m_rDst.y = y;
		m_rSrc.w = 32; m_rDst.w = 32;
		m_rSrc.h = 32; m_rDst.h = 32;
		m_rSrc.x = 32; m_rSrc.y = 64;
	}

	int image = 0;
	void update();
	
};
class BGTile : public Sprite
{
public:
	bool m_bIsObstacle;
	bool m_bIsHazard;

	void SetRect()
	{
		if (m_cOutput == ',')
			m_rSrc = { 0, 0, 32, 32 };
		else if (m_cOutput == ';')
			m_rSrc = { 32, 0, 32, 32 };
		else if (m_cOutput == 'M')
			m_rSrc = { 64, 0, 32, 32 };
		else if (m_cOutput == 'm')
			m_rSrc = { 96, 0, 32, 32 };
		else if (m_cOutput == '.')
			m_rSrc = { 128, 0, 32, 32 };
		else if (m_cOutput == '-')
			m_rSrc = { 160, 0, 32, 32 };
		else if (m_cOutput == '~')
			m_rSrc = { 192, 0, 32, 32 };
		else if (m_cOutput == 'X')
			m_rSrc = { 224, 0, 32, 32 };
		else if (m_cOutput == '#')
			m_rSrc = { 0, 32, 32, 32 };
		else if (m_cOutput == 'O')
			m_rSrc = { 32, 32, 32, 32 };
		else if (m_cOutput == '=')
			m_rSrc = { 64, 32, 32, 32 };
		else if (m_cOutput == 'H')
			m_rSrc = { 96, 32, 32, 32 };
		else if (m_cOutput == 'B')
			m_rSrc = { 128, 32, 32, 32 };
		else if (m_cOutput == '_')
			m_rSrc = { 160, 32, 32, 32 };
		else if (m_cOutput == 'D') // BG Door tile. NOt Q.
			m_rSrc = { 192, 32, 32, 32 };
		else if (m_cOutput == '%')
			m_rSrc = { 224, 32, 32, 32 };
	}
	void SetBGTile(const char c)
	{
		m_cOutput = c;
		SetRect();
		for (int i = 0; i < OTILES; i++)
		{
			if (g_cOTiles[i] == c)
				m_bIsObstacle = true;
		}
		for (int i = 0; i < HTILES; i++)
		{
			if (g_cHTiles[i] == c)
				m_bIsHazard = true;
		}
	}
};

class Player : public LOTile
{
public:
	bool m_bIsDead;
	int moveSpeed = 7;
	Player(int x, int y, SDL_Renderer* r, SDL_Texture* t) : LOTile(x, y)
	{

		int playerTileX = 0;
		m_bIsDead = false;
		m_cOutput = '@';
		SetImage(r, t);
		m_rSrc = { 0, 0, 32, 32 };
		
		m_rDst.w = m_rDst.h = 32;
	}
	int getMoveSpeed() { return moveSpeed; }

};

class Door : public LOTile
{
public:
	int m_iToLevel;
	int m_DestX;
	int m_DestY;
	Door() { m_cOutput = 'D'; }
	void SetDoor(const int x, const int y, const int toLevel, const int dx, const int dy, SDL_Renderer* r, SDL_Texture* t)
	{
		m_iToLevel = toLevel;
		m_DestX = dx;
		m_DestY = dy;
		SetX(x);
		SetY(y);
		SetImage(r, t);
		m_rSrc = { 192, 32, 32, 32 };
		m_rDst.w = m_rDst.h = 32;
	}
};

class Level
{
public:
	BGTile map[ROWS][COLS];
	Door* doors;
	int m_iNumDoors;
	int m_iNextDoor;
	/*Level()
	{
	m_iNextDoor = 0;
	m_iNumDoors = 3;
	doors = new Door[m_iNumDoors];
	}*/
	Level(const int n)
	{
		m_iNextDoor = 0;
		m_iNumDoors = n;
		doors = new Door[m_iNumDoors];
		cout << endl << "to level"<< m_iNextDoor;
	}
	~Level()
	{
		if (doors != nullptr) // Just because.
		{
			delete[] doors;
			doors = nullptr;
		}
	}
	void AddDoor(const int x, const int y, const int toLevel, const int dx, const int dy, SDL_Renderer* r, SDL_Texture* t)
	{
		if (m_iNextDoor < m_iNumDoors)
		{
			if (r == nullptr || t == nullptr)
				cout << "Some pointer is null" << endl;
			doors[m_iNextDoor++].SetDoor(x, y, toLevel, dx, dy, r, t);
		}
		else
			cout << "Cannot add door. Limit reached." << endl;
	}
};