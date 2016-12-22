#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include "Level.h"
#include <vector>
#include <iostream>

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		finished = false;
		bonus = 1000;
	}
	~StudentWorld();
	virtual int init()
	{
		finished = false;
		bonus = 1000;
		int lvl = getLevel();
		int result = loadLevel(lvl);
		if (result == Level::load_fail_file_not_found)
			return GWSTATUS_PLAYER_WON;
		else if (result == Level::load_fail_bad_format)
			return GWSTATUS_LEVEL_ERROR;
		else
			return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		updateDisplayText();//player's move
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (m_actors[i]->isAlive())
				m_actors[i]->doSomething();
			if (!m_player->isAlive())
				return GWSTATUS_PLAYER_DIED;
		}
		if (m_player->isAlive())
			m_player->doSomething();
		if (!m_player->isAlive())
			return GWSTATUS_PLAYER_DIED;
		removeDeadObjects();
		if (Finished())
		{

			return GWSTATUS_FINISHED_LEVEL;
		}
		if (bonus != 0)
			decreaseBonus();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		while (!m_actors.empty())
		{
			delete m_actors.back();
			m_actors.pop_back();
		}
		delete m_player;
	}
	int loadLevel(int x);
	void updateDisplayText();
	std::string setText(int score, int level, int lives, int health, int ammo, unsigned int bonus);
	unsigned int BonusAmount() const;
	int numDigits(int number);
	bool HoleHere(int x, int y);
	bool isABoulder(int x, int y);
	void pusher(int x, int y, GraphObject::Direction dir);
	bool canDamage(int x, int y);
	void Damage(int x, int y);
	bool Walkable(int x, int y);
	void storeObject(Actor* temp);
	void removeDeadObjects();
	void DeadBoulder(int x, int y);
	bool playerHere(int x, int y);
	int JewelsLeft();
	void setFinished();
	void decreaseBonus();
	bool isAGoodie(int x, int y);
	bool Finished();
	int level();
	bool canShoot(int x, int y, GraphObject::Direction dir);
	void Stats(int ammo, int health);
	int Count(int x, int y);
	bool kleptobotHere(int x, int y);
	int stealAGoodie(int x, int y);
	bool ExitHere(int x, int y);
private:
	Player* m_player;
	std::vector <Actor*> m_actors;
	unsigned int bonus;
	bool finished;
};

#endif // STUDENTWORLD_H_