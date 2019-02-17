#pragma once
#include <chrono>
#include <iostream>
#include <fstream>
#include "AudioManager.h"
#include "FSM.h" // Needed for the StateMachine class.
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Classes.h"
#define ROWS 24
#define COLS 32
using namespace std;
using namespace chrono;

class Game
{
private:
	bool m_bRunning;
	bool m_bGotTick;
	//bool m_bLeftMouse = false; // Just a single bool to keep track of left mouse button state.
	const Uint8* m_iKeystates; // Pointer to an 'array' of key states with values of 1 or 0.
							   // The const just prevents us from changing the array.
	int m_iMouseX, m_iMouseY;  // Variables to hold mouse positions.	
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;
	StateMachine* m_pFSM; // Pointer to the StateMachine object created dynamically.
	AudioManager* m_pAM;

	Game() { cout << "Constructing game..." << endl; }
public:
	~Game() { delete m_pFSM; } // Destructor.
	static Game* Instance() // This static method creates the static instance that can be accessed 'globally'
	{
		static Game* instance = new Game(); // C++11 will prevent this line from running more than once. Magic statics.
		return instance;
	}
	// Now a trio of getters.
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	SDL_Texture* GetTexture() { return m_pText; }
	StateMachine* GetFSM() { return m_pFSM; }
	AudioManager* GetAM() { return m_pAM; }
	int GetMouseX() { return m_iMouseX; }
	int GetMouseY() { return m_iMouseY; }
	int GetLeftMouse() { return m_bLeftMouse; }
	int tl, px, py;
	void DoQuit() { m_bRunning = false; }
	Level* plevels[5];
	const char* levelNames[5] = { "Level1.txt", "Level2.txt", "Level3.txt", "Level4.txt", "Level5.txt" };//An Array text files with these names. 
	SDL_Surface* tileSurf;
	SDL_Texture* tileText;
	SDL_Surface* playerSurf;
	SDL_Texture* playerText;
	int currLevel = 0;
	bool m_bUpPressed;
	bool m_bDownPressed;
	bool m_bLeftPressed;
	bool m_bRightPressed;
	Player* pPlayer;
    Player* getPlayer() { return pPlayer; }
	Item* pItem;
	const SDL_Rect keyRect = { 32,64,32,32 };
	const SDL_Rect finalRect = { 96, 96, 32,32 };
	const SDL_Rect item1 = { 32,32,32,32 };
	const SDL_Rect item2 = { 64,32,32,32 };
	const SDL_Rect prizeRect = {160,64,32,32};
	bool hasKey = false;
	bool hasFinal = false;
	int leftRight; //player facing; 0 = left 1 = right
	bool Moving; // false = not moving
	int countAnim = 0;
	bool m_bLeftMouse = false;
	int getPlayerCol();int getPlayerRow();
	void checkCollision();
	bool KeyDown(SDL_Scancode c) // This tests the scancode parameter against the keystates. Scancode is SDL2.
	{
		if (m_iKeystates != nullptr)
		{
			if (m_iKeystates[c] == 1)
				return true;
			else
				return false;
		}
		return false;
	}
	bool Init(const char* title, int xpos, int ypos, int width,	int height, int flags)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // SDL init success.
		{
			m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
			if (m_pWindow != 0) // Window init success.
			{
				m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
				if (m_pRenderer != nullptr) // Renderer init success.
				{
					if (IMG_Init(IMG_INIT_PNG) != 0)
					{
						
						 tileSurf = IMG_Load("Tiles.png");
						 tileText = SDL_CreateTextureFromSurface(Game::GetRenderer(), tileSurf);
						 playerSurf = IMG_Load("Player.png");
						 playerText = SDL_CreateTextureFromSurface(Game::GetRenderer(), playerSurf);

						m_pSurf = IMG_Load(" ");
						m_pText = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurf);
						SDL_FreeSurface(m_pSurf);
						if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
						{
							Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 8192);
						}
						else return false; // Mixer init fail.
					}
					else return false; // Image init fail.
				}
				else return false; // Renderer init fail.
			}
			else return false; // Window init fail.
		}
		else return false; // SDL init fail.
		m_pAM = new AudioManager(); // Creates the audio manager object.
		m_pAM->SetMusicVolume(10); // Set low volume so we can hear sfx.
		m_pFSM = new StateMachine(); // Creates the state machine object/instance.
		m_pFSM->ChangeState(new TitleState()); // Invoking the ChangeState method to set the initial state, Title.
		m_iKeystates = SDL_GetKeyboardState(NULL); // We only need to put this here. Why?
		m_bRunning = true; // Everything is okay, start the engine.
		return true;
	}
	bool Running() { return m_bRunning; };
	bool BuildMap(Level* l, const char* n, SDL_Renderer* r, SDL_Texture* t) //This gives value to Class 'Level' member 'map' @ [row][col]
	{					//n = which level       
		ifstream inFile(n);// open the file with the name contained in 'n' 
		char temp; //temporary holder for a character in the file
		if (inFile.is_open())
		{
			for (int row = 0; row < ROWS; row++)
			{
				for (int col = 0; col < COLS; col++) // infile will read the first char and move to the right, then at an endl; will go down and repeat.
				{
					inFile >> temp;
					l->map[row][col].SetBGTile((temp == '*' ? ' ' : temp)); //if the character in the text file, temp is a * make the bg tile ' ' if its not a star make the tile what temp is already set to.
					l->map[row][col].SetImage(r, t);
					l->map[row][col].m_rDst = { col * 32, row * 32, 32, 32 };//the destination to draw.
					if (temp == 'Q')// if you find a Q(in the text file)
					{
						
						inFile >> tl >> px >> py; // get the 3 characters info after it and make a door with those values. 
						l->AddDoor(col, row, tl, Game::Instance()->px, Game::Instance()->py, r, t);//the values can be seen in the door object. they include where the player needs to go, and which map it leads to etc.
					}
				}
			}
			inFile.close();
			return 0;
		}
		return 1;
	}
	bool Tick()
	{
		auto duration = steady_clock::now().time_since_epoch();
		auto count = duration_cast<microseconds>(duration).count();
		int tick = 1000000 / 60;

		if (count % tick < 100) // Drops potential duplicate frames.
		{
			if (m_bGotTick == false)
			{
				m_bGotTick = true;
				return true;
			}
			return false;
		}
		else m_bGotTick = false;
		return false;
	}
	void Update()
	{
		GetFSM()->Update(); // Invokes the update of the state machine.
	}
	void HandleEvents() // Significantly shorter handle events. 
	{
		SDL_Event event;

		while (SDL_PollEvent(&event)) // SDL_PollEvent invokes SDL_PumpEvents() which we need to refresh the keyboard states.
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_bRunning = false;
				break;
				
			case SDL_KEYDOWN:
				
				switch (event.key.keysym.sym)
				{
				case 'w':
				case 'W':
					if (!m_bUpPressed)
						m_bUpPressed = true;
					Moving = true;
					break;
				case 's':
				case 'S':
					if (!m_bDownPressed)
						m_bDownPressed = true;
					break;
				case 'a':
				case 'A':
					if (!m_bLeftPressed)
						m_bLeftPressed = true;
					leftRight = 0;
					Moving = true;
					break;
				case 'd':
				case 'D':
					if (!m_bRightPressed)
						m_bRightPressed = true;
					leftRight = 1;
					Moving = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case 'w':
				case 'W':
					Moving = false;
					m_bUpPressed = false;
				
					break;
				case 's':
				case 'S':	
					Moving = false;
					m_bDownPressed = false;
				
					break;
				case 'a':
				case 'A':
					Moving = false;
					m_bLeftPressed = false;
				
					break;
				case 'd':
				case 'D':
					Moving = false;
					m_bRightPressed = false;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					m_bLeftMouse = true;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					m_bLeftMouse = false;
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&m_iMouseX, &m_iMouseY);
				break;
			}
		}
	}
	void Render()
	{
		GetFSM()->Render(); // Invokes the render of the state machine.
	}
	void Clean()
	{
		GetFSM()->Clean();
		SDL_DestroyTexture(m_pText);
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		IMG_Quit();
		SDL_Quit();
	}
	
};