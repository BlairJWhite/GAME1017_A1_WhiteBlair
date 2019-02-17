#include "FSM.h"
#include "Game.h"
//CTRL M + A/P COLLAPSE/EXPAND ALL, click to other file and back if not working. 
void State::Render()
{
	SDL_RenderPresent(Game::Instance()->GetRenderer());
}


void PauseState::Enter()
{
	cout << "Entering Pause..." << endl;
	Game::Instance()->GetAM()->ToggleMusic();
	m_vButtons.push_back(new Button("Img/button.png", { 312,100,400,100 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 312,300,400,100 }));
}

void PauseState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_R) == 1)
		Game::Instance()->GetFSM()->PopState();
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons.
	if (m_vButtons[btn::play]->Clicked())
		Game::Instance()->GetFSM()->PopState();
	else if (m_vButtons[btn::exit]->Clicked())
	{
	Game::Instance()->m_bLeftMouse = false;
	Game::Instance()->GetFSM()->ChangeState(new TitleState());
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	}
}

void PauseState::Render()
{
	cout << "Rendering Title..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 255);
	//SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting Pause..." << endl;
	Game::Instance()->GetAM()->ToggleMusic();

}


void GameState::Enter()
{ 
	Game::Instance()->currLevel = 0;
	Game::Instance()->hasFinal = false;
	Game::Instance()->hasKey = false;
	cout << "Entering Game..." << endl;
	Game::Instance()->GetAM()->LoadMusic("Aud/Turtles.mp3");
	Game::Instance()->GetAM()->LoadSound("Aud/jump.wav");
	Game::Instance()->GetAM()->LoadSound("Aud/death.wav");
	Game::Instance()->GetAM()->LoadSound("Aud/laser.wav");
	Game::Instance()->GetAM()->PlayMusic();
	
	Game::Instance()->plevels[0] = new Level(2);
	Game::Instance()->plevels[1] = new Level(1);
	Game::Instance()->plevels[2] = new Level(1);
	Game::Instance()->plevels[3] = new Level(2);
	Game::Instance()->plevels[4] = new Level(1);
	
	Game::Instance()->pItem = new Item(0,0);
	
	for (int i = 0; i < 5; i++) // This should be moved.
	{
		
		Game::Instance()->BuildMap(Game::Instance()->plevels[i], Game::Instance()->levelNames[i], Game::Instance()->GetRenderer(), Game::Instance()->tileText); // builds all maps

	}
	Game::Instance()->pPlayer = new Player(COLS / 2, ROWS / 2, Game::Instance()->GetRenderer(), Game::Instance()->playerText);

}

void GameState::Update()
{
	
	if(Game::Instance()->m_bDownPressed || Game::Instance()->m_bLeftPressed || Game::Instance()->m_bRightPressed || Game::Instance()->m_bUpPressed)
	{
		if (Game::Instance()->Moving == false) { Game::Instance()->Moving = true; }
	}
	
	if (Game::Instance()->getPlayer()->m_bIsDead == true) { Game::Instance()->GetFSM()->PushState(new Lose());  return; }
	if (Game::Instance()->KeyDown(SDL_SCANCODE_P) == 1)
		Game::Instance()->GetFSM()->PushState(new PauseState());
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_X) == 1)
		Game::Instance()->GetFSM()->ChangeState(new TitleState());

	if (Game::Instance()->KeyDown(SDL_SCANCODE_J) == 1)
		Game::Instance()->GetAM()->PlaySound(sfx::jump);
	if (Game::Instance()->KeyDown(SDL_SCANCODE_K) == 1)
		Game::Instance()->GetAM()->PlaySound(sfx::boom);
	if (Game::Instance()->KeyDown(SDL_SCANCODE_L) == 1)
		Game::Instance()->GetAM()->PlaySound(sfx::laser);
	
	//animation
	Game::Instance()->countAnim++;
	if (Game::Instance()->Moving == true)
	{
		if (Game::Instance()->countAnim == 0) { Game::Instance()->countAnim = 1; }
		if (Game::Instance()->countAnim >= 80) { Game::Instance()->countAnim = 1; }
		Game::Instance()->getPlayer()->m_rSrc.x = (Game::Instance()->countAnim / 20 + 1) * 32;
	}
	if (Game::Instance()->Moving == false) { Game::Instance()->getPlayer()->m_rSrc.x = 0; }
	
	//Collisions and movement. 
	if (Game::Instance()->m_bUpPressed && Game::Instance()->plevels[Game::Instance()->currLevel]->map[Game::Instance()->getPlayerRow() - 1][Game::Instance()->getPlayerCol()].m_bIsObstacle != true)
		Game::Instance()->getPlayer()->m_rDst.y -= 2;
	else
	{
		if (Game::Instance()->m_bUpPressed && (Game::Instance()->getPlayer()->m_rDst.y + 8) % 32 > 2) { Game::Instance()->getPlayer()->m_rDst.y -= 2; }
	}
	
	if (Game::Instance()->m_bDownPressed && Game::Instance()->plevels[Game::Instance()->currLevel]->map[Game::Instance()->getPlayerRow() + 1][Game::Instance()->getPlayerCol()].m_bIsObstacle != true)
		Game::Instance()->getPlayer()->m_rDst.y += 2;
	else
	{
		if (Game::Instance()->m_bDownPressed && (Game::Instance()->getPlayer()->m_rDst.y + 32) % 32 < 30) { Game::Instance()->getPlayer()->m_rDst.y += 2; }
	}


	if (Game::Instance()->m_bLeftPressed &&Game::Instance()->plevels[Game::Instance()->currLevel]->map[Game::Instance()->getPlayerRow()][Game::Instance()->getPlayerCol() - 1].m_bIsObstacle != true)
		Game::Instance()->getPlayer()->m_rDst.x -= 2;
	else
	{
		if (Game::Instance()->m_bLeftPressed && (Game::Instance()->getPlayer()->m_rDst.x + 2) % 32 > 2) { Game::Instance()->getPlayer()->m_rDst.x -= 2; }
	}


	if (Game::Instance()->m_bRightPressed && Game::Instance()->plevels[Game::Instance()->currLevel]->map[Game::Instance()->getPlayerRow()][Game::Instance()->getPlayerCol() + 1].m_bIsObstacle != true)
		Game::Instance()->getPlayer()->m_rDst.x += 2;
	else
	{
		if (Game::Instance()->m_bRightPressed && (Game::Instance()->getPlayer()->m_rDst.x + 30) % 32 < 30) { Game::Instance()->getPlayer()->m_rDst.x += 2; }
	}

	if (Game::Instance()->plevels[Game::Instance()->currLevel]->map[Game::Instance()->getPlayerRow()][Game::Instance()->getPlayerCol()].m_bIsHazard == true) // Player died.
	{
		Game::Instance()->getPlayer()->m_bIsDead = true;
		Game::Instance()->getPlayer()->m_rSrc.x = 32;
		return;
	}
	else // Check doors.
	{
		
		for (int i = 0; i < Game::Instance()->plevels[Game::Instance()->currLevel]->m_iNumDoors; i++)
		{
			if (Game::Instance()->getPlayer()->m_rDst.x+16 > Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rDst.x && // move the player to the new spot on the new map where it needs to be. 
				Game::Instance()->getPlayer()->m_rDst.x+16 < Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rDst.x+32 &&
				Game::Instance()->getPlayer()->m_rDst.y+16 > Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rDst.y &&
				Game::Instance()->getPlayer()->m_rDst.y+16 < Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rDst.y+32) //if players x and y are the same as one of the doors do this.
			{																						
				if (Game::Instance()->currLevel == 0 && Game::Instance()->hasKey==false && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 3)
				{
					Game::Instance()->getPlayer()->m_rDst.y -= 10;
				}
				if (Game::Instance()->currLevel == 0 && Game::Instance()->hasKey == true && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 3)
				{
					Game::Instance()->getPlayer()->m_rDst.x = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestX * 32; //(above)you'll notice c being passed in by int&. so when we say c = we are saying change whats in the address of c to this.
					Game::Instance()->getPlayer()->m_rDst.y = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestY * 32;
					Game::Instance()->currLevel = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel; //c is 'currentLevel' in main. When it runs this it will change the current level to the value found in the door we are looking at. 
					Game::Instance()->GetAM()->PlaySound(0, 0, 0);
					return;
				}
							
				if (Game::Instance()->currLevel == 0 && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 1)
				{
					Game::Instance()->getPlayer()->m_rDst.x = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestX * 32; //(above)you'll notice c being passed in by int&. so when we say c = we are saying change whats in the address of c to this.
					Game::Instance()->getPlayer()->m_rDst.y = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestY * 32;
					Game::Instance()->currLevel = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel;
					Game::Instance()->GetAM()->PlaySound(0, 0, 0);
					return;
				}
				
				if (Game::Instance()->currLevel == 1 && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 0)
				{
					Game::Instance()->getPlayer()->m_rDst.x = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestX * 32; //(above)you'll notice c being passed in by int&. so when we say c = we are saying change whats in the address of c to this.
					Game::Instance()->getPlayer()->m_rDst.y = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestY * 32;
					Game::Instance()->currLevel = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel;
					Game::Instance()->GetAM()->PlaySound(0, 0, 0);
					return;
				}
				
				if (Game::Instance()->currLevel == 3 && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 4)
				{
					Game::Instance()->getPlayer()->m_rDst.x = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestX * 32; //(above)you'll notice c being passed in by int&. so when we say c = we are saying change whats in the address of c to this.
					Game::Instance()->getPlayer()->m_rDst.y = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestY * 32;
					Game::Instance()->currLevel = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel;
					Game::Instance()->GetAM()->PlaySound(0, 0, 0);
					return;
				}
				if (Game::Instance()->currLevel == 4 && Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel == 3)
				{
					Game::Instance()->getPlayer()->m_rDst.x = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestX * 32; //(above)you'll notice c being passed in by int&. so when we say c = we are saying change whats in the address of c to this.
					Game::Instance()->getPlayer()->m_rDst.y = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_DestY * 32;
					Game::Instance()->currLevel = Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_iToLevel;
					Game::Instance()->GetAM()->PlaySound(0, 0, 0);
					return;
				}
			}
		}
	}

	Game::Instance()->pItem->update();
}

void GameState::Render()
{
	//cout << "Rendering Game..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	
	SDL_RenderClear(Game::Instance()->GetRenderer()); // Clear the screen to the draw color.
	for (int row = 0; row < ROWS; row++) // Drawing the tiles.
	{
		for (int col = 0; col < COLS; col++)
		{
			if (Game::Instance()->plevels[Game::Instance()->currLevel]->map[row][col].m_cOutput != ' ')//if the tile at (map[][]) of the class  'Levels'(var 1 input) has a variable m_cOutput that isn't ' '...
				SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->plevels[Game::Instance()->currLevel]->map[row][col].m_pTexture, &Game::Instance()->plevels[Game::Instance()->currLevel]->map[row][col].m_rSrc, &Game::Instance()->plevels[Game::Instance()->currLevel]->map[row][col].m_rDst); //draw the tile from the tiles variables.
		}
	}
	for (int i = 0; i < Game::Instance()->plevels[Game::Instance()->currLevel]->m_iNumDoors; i++) // Drawing the level doors.
	SDL_RenderCopy(Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_pRenderer, Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_pTexture, &Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rSrc, &Game::Instance()->plevels[Game::Instance()->currLevel]->doors[i].m_rDst);
	
	if (Game::Instance()->Moving == false) 
	{
		
		if (Game::Instance()->leftRight == 0) 
		{SDL_RenderCopyEx(Game::Instance()->getPlayer()->m_pRenderer, Game::Instance()->getPlayer()->m_pTexture, &Game::Instance()->getPlayer()->m_rSrc, &Game::Instance()->getPlayer()->m_rDst,0,NULL, SDL_FLIP_HORIZONTAL);} // Drawing the player.
		if (Game::Instance()->leftRight == 1)
		{SDL_RenderCopyEx(Game::Instance()->getPlayer()->m_pRenderer, Game::Instance()->getPlayer()->m_pTexture, &Game::Instance()->getPlayer()->m_rSrc, &Game::Instance()->getPlayer()->m_rDst, 0, NULL, SDL_FLIP_NONE);}
	}
	
	if (Game::Instance()->Moving == true) 
	{
		if (Game::Instance()->leftRight == 0)
		{
			SDL_RenderCopyEx(Game::Instance()->getPlayer()->m_pRenderer, Game::Instance()->getPlayer()->m_pTexture, &Game::Instance()->getPlayer()->m_rSrc, &Game::Instance()->getPlayer()->m_rDst, 0, NULL, SDL_FLIP_HORIZONTAL);
		} // Drawing the player.
		if (Game::Instance()->leftRight == 1)
		{
			SDL_RenderCopyEx(Game::Instance()->getPlayer()->m_pRenderer, Game::Instance()->getPlayer()->m_pTexture, &Game::Instance()->getPlayer()->m_rSrc, &Game::Instance()->getPlayer()->m_rDst, 0, NULL, SDL_FLIP_NONE);
		}
	}
	if (Game::Instance()->currLevel == 0)
	{
		SDL_SetTextureAlphaMod(Game::Instance()->tileText, 0);
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->pItem->m_rSrc, &Game::Instance()->pItem->m_rDst);
		SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
	}
	if (Game::Instance()->currLevel == 1)
	{
		if (Game::Instance()->hasKey == true)
		{
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 0);
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->pItem->m_rSrc, &Game::Instance()->pItem->m_rDst);
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
		}
		else
		{
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->pItem->m_rSrc, &Game::Instance()->pItem->m_rDst);
		}
	}
	if (Game::Instance()->currLevel == 4)
	{
		if (Game::Instance()->hasFinal == true)
		{
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 0);
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->finalRect, &Game::Instance()->pItem->m_rDst);
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
		}
		else
		{
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->finalRect, &Game::Instance()->pItem->m_rDst);
		}
	}

	if (Game::Instance()->currLevel == 3)
	{
		if (Game::Instance()->hasFinal == true)
		{
			SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->prizeRect, &Game::Instance()->pItem->m_rDst);
		}
	}

	

	if (Game::Instance()->hasKey == true)
	{
		SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->keyRect, &Game::Instance()->item1);
	}
	if (Game::Instance()->hasFinal == true)
	{
		SDL_SetTextureAlphaMod(Game::Instance()->tileText, 255);
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->tileText, &Game::Instance()->finalRect, &Game::Instance()->item2);
	}

	SDL_RenderPresent(Game::Instance()->GetRenderer()); // Draw new.
	//State::Render();
}

void GameState::Exit()
{ 
	Game::Instance()->currLevel = 0;
	cout << "Exiting Game..." << endl;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	for (int i = 0; i < 5; i++)
	{
		delete Game::Instance()->plevels[i];
		Game::Instance()->plevels[i] = nullptr;
	}
}


void Lose::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons.
	if (m_vButtons[btn::play]->Clicked())
	{
		Game::Instance()->m_bLeftMouse = false;
		Game::Instance()->GetFSM()->ChangeState(new TitleState());
	}
	else if (m_vButtons[btn::exit]->Clicked())
		Game::Instance()->DoQuit();
	
	if (endloop == 420) 
	{
		Game::Instance()->GetAM()->ToggleMusic();
	}
	endloop++;
}

void Lose::Render()
{
	cout << "Rendering LOSE..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 255);
	//SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void Lose::Enter()
{
	endloop = 0;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	cout << "Entering LOSE..." << endl;
	m_vButtons.push_back(new Button("Img/button.png", { 312,100,400,100 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 312,300,400,100 }));
	Game::Instance()->GetAM()->LoadMusic("Aud/Thundercats.mp3");
	Game::Instance()->GetAM()->LoadSound("Aud/power.wav");
	Game::Instance()->GetAM()->PlayMusic();
}

void Lose::Exit()
{
	cout << "Exiting Title..." << endl;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}


void Win::Update()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons.
	if (m_vButtons[btn::play]->Clicked())
	{
		Game::Instance()->m_bLeftMouse = false;
		Game::Instance()->GetFSM()->ChangeState(new TitleState());
	}
	else if (m_vButtons[btn::exit]->Clicked())
		Game::Instance()->DoQuit();

	
}

void Win::Render()
{
	cout << "Rendering WIN..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 255);
	//SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void Win::Enter()
{
	
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	cout << "Entering LOSE..." << endl;
	m_vButtons.push_back(new Button("Img/button.png", { 312,100,400,100 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 312,300,400,100 }));
	Game::Instance()->GetAM()->LoadMusic("Aud/Thundercats.mp3");
	Game::Instance()->GetAM()->LoadSound("Aud/power.wav");
	Game::Instance()->GetAM()->PlayMusic();
}

void Win::Exit()
{
	cout << "Exiting Title..." << endl;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}




void TitleState::m_Test()
{
	cout << "Blah..." << endl;
}

void TitleState::Enter()
{ 
	cout << "Entering Title..." << endl;
	m_vButtons.push_back(new Button("Img/button.png", { 312,100,400,100 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 312,300,400,100 }));
	Game::Instance()->GetAM()->LoadMusic("Aud/Thundercats.mp3");
	Game::Instance()->GetAM()->LoadSound("Aud/power.wav");
	Game::Instance()->GetAM()->PlayMusic();
}

void TitleState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_B) == 1)
		Game::Instance()->GetAM()->PlaySound(sfx::boop);
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons.
	if (m_vButtons[btn::play]->Clicked())
		Game::Instance()->GetFSM()->ChangeState(new GameState());
	else if (m_vButtons[btn::exit]->Clicked())
		Game::Instance()->DoQuit();
}

void TitleState::Render()
{
	cout << "Rendering Title..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void TitleState::Exit()
{ 
	cout << "Exiting Title..." << endl;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();

}


void StateMachine::Update()
{
	if (!m_vStates.empty()) // empty() and back() are methods of the vector type.
		m_vStates.back()->Update();
}

void StateMachine::Render()
{
	if (!m_vStates.empty())
		m_vStates.back()->Render();
}

void StateMachine::PushState(State* pState)
{
	m_vStates.push_back(pState); // push_back() is a method of the vector type.
	m_vStates.back()->Enter();
}

void StateMachine::ChangeState(State* pState)
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back(); // De-allocating the state in the heap.
		m_vStates.back() = nullptr; // Nullifying pointer to the de-allocated state.
		m_vStates.pop_back(); // Removes the now-null pointer from the vector.
	}
	pState->Enter();
	m_vStates.push_back(pState);
}

void StateMachine::PopState()
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
	m_vStates.back()->Resume();
}

void StateMachine::Clean()
{
	while (!m_vStates.empty()) // Because we can exit the game in the pause state with the window's 'X'.
	{						   // Ensures that ALL states left in the vector are cleaned up.
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}


