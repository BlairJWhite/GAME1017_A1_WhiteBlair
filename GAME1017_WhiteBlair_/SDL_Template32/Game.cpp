#include <chrono>
#include <iostream>
#include <string>
#include "Game.h"
#include "Classes.h"
#define ROWS 24
#define COLS 32
using namespace std;
using namespace chrono;


int Game::getPlayerRow()
{
	int playerY = Game::Instance()->getPlayer()->m_rDst.y + 16;
	int playerTileY;

	for (playerTileY = 0; playerTileY < COLS; playerTileY++)
	{
		if (playerTileY > playerY / 32) { return playerTileY - 1; }
	}
}

int Game::getPlayerCol()
{
	int playerX = Game::Instance()->getPlayer()->m_rDst.x + 16;
	int playerTileX;

	for (playerTileX = 0; playerTileX < COLS; playerTileX++)
	{
		if (playerTileX > playerX / 32) { return playerTileX - 1; }
	}
}

void Game::checkCollision()
//This can be run only when pos % 32 > 32 - movespeed. ie. not in the middle of a tile. 
{



}






