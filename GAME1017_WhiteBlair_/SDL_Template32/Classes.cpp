#include "Classes.h"
#include "Game.h"
void Item::update()
{

	if (Game::Instance()->currLevel == 1)
	{
		if (Game::Instance()->hasKey == false)
		{

			m_rDst.x = 24 * 32; m_rDst.y = 18 * 32;
			m_rSrc.x = 32; m_rSrc.y = 64;
			if (m_rDst.x + 16 > Game::Instance()->getPlayer()->m_rDst.x && m_rDst.x + 16 < Game::Instance()->getPlayer()->m_rDst.x + 32
				&& m_rDst.y + 16 > Game::Instance()->getPlayer()->m_rDst.y && m_rDst.y + 16 < Game::Instance()->getPlayer()->m_rDst.y + 32)
			{
				Game::Instance()->GetAM()->PlaySound(2, 0, 0);
				Game::Instance()->hasKey = true;
			}
		}
		else
		{

		}
	}


	if (Game::Instance()->currLevel == 4)
	{
		if (Game::Instance()->hasFinal == false)
		{

			m_rDst.x = 15 * 32; m_rDst.y = 10 * 32;
			m_rSrc.x = 96; m_rSrc.y = 96;
			if (m_rDst.x + 16 > Game::Instance()->getPlayer()->m_rDst.x && m_rDst.x + 16 < Game::Instance()->getPlayer()->m_rDst.x + 32
				&& m_rDst.y + 16 > Game::Instance()->getPlayer()->m_rDst.y && m_rDst.y + 16 < Game::Instance()->getPlayer()->m_rDst.y + 32)
			{
				Game::Instance()->GetAM()->PlaySound(2, 0, 0);
				Game::Instance()->hasFinal = true;
			}
		}
		else
		{

		}
	}
	
	if (Game::Instance()->currLevel == 3)
	{
		if (Game::Instance()->hasFinal == true)
		{
			m_rDst.x = 15 * 32; m_rDst.y = 8 * 32;
			m_rSrc.x = 160; m_rSrc.y = 64;
			if (m_rDst.x + 16 > Game::Instance()->getPlayer()->m_rDst.x && m_rDst.x + 16 < Game::Instance()->getPlayer()->m_rDst.x + 32
				&& m_rDst.y + 16 > Game::Instance()->getPlayer()->m_rDst.y && m_rDst.y + 16 < Game::Instance()->getPlayer()->m_rDst.y + 32)
			{
				Game::Instance()->GetAM()->PlaySound(1, 0, 0);
				Game::Instance()->GetFSM()->PushState(new Win());
			}
		}
		else
		{

		}
	}

}