#include "StudentWorld.h"
#include <string>
#include "GraphObject.h"
#include "GameWorld.h"
#include "Level.h"
#include <iostream> //for testing purposes
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::~StudentWorld()
{
	while (!m_actors.empty())
		delete m_actors.back();
	delete m_player;
}
int StudentWorld::loadLevel(int number)
{
	int hp = 10;
	int hp1 = 20;
	int hp2 = 1000;
	int normal = 1;
	int angry = 2;
	char num = '0' + number;
	string levelData = "level0";
	levelData += num;
	levelData += ".dat";
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(levelData);
	if (result == Level::load_fail_file_not_found)
		return Level::load_fail_file_not_found;
	else if (result == Level::load_fail_bad_format)
		return Level::load_fail_bad_format;
	for (int k = 0; k < VIEW_WIDTH; k++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Level::MazeEntry item = lev.getContentsOf(k, j);
			if (item == Level::player)
				m_player = new Player(IID_PLAYER, k, j, this, hp1);
			if (item == Level::wall)
				m_actors.push_back(new Wall(IID_WALL, k, j, this));
			if (item == Level::boulder)
				m_actors.push_back(new Boulder(IID_BOULDER, k, j, this, hp));
			if (item == Level::hole)
				m_actors.push_back(new Hole(IID_HOLE, k, j, this));
			if (item == Level::jewel)
				m_actors.push_back(new Jewel(IID_JEWEL, k, j, this));
			if (item == Level::exit)
				m_actors.push_back(new Exit(IID_EXIT, k, j, this));
			if (item == Level::extra_life)
				m_actors.push_back(new ExtraLife(IID_EXTRA_LIFE, k, j, this));
			if (item == Level::ammo)
				m_actors.push_back(new Ammo(IID_AMMO, k, j, this));
			if (item == Level::restore_health)
				m_actors.push_back(new RestoreHealth(IID_RESTORE_HEALTH, k, j, this));
			if (item == Level::horiz_snarlbot)
				m_actors.push_back(new Snarlbot(IID_SNARLBOT, k, j, this, hp, GraphObject::right));
			if (item == Level::vert_snarlbot)
				m_actors.push_back(new Snarlbot(IID_SNARLBOT, k, j, this, hp, GraphObject::down));
			if (item == Level::kleptobot_factory)
				m_actors.push_back(new Factory(IID_ROBOT_FACTORY, k, j, normal, this, GraphObject::none));
			if (item == Level::angry_kleptobot_factory)
				m_actors.push_back(new Factory(IID_ROBOT_FACTORY, k, j, angry, this, GraphObject::none));
		}
	}

	return Level::load_success;
}

void StudentWorld::updateDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int health = m_player->health() * 5;
	int ammo = m_player->ammoAmt();
	unsigned int bonus = BonusAmount();
	int lives = getLives();
	string s = setText(score, level, lives, health, ammo, bonus);
	// Finally, update the display text at the top of the screen with your // newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}
string StudentWorld::setText(int score, int level, int livesLeft, int health, int ammo, unsigned int bonus){
	string message = "Score: ";
	int places1 = numDigits(score);
	for (int k = 0; k < 7 - places1; k++)
	{
		message += '0';
	}
	string temp1 = std::to_string(score);
	message += temp1;
	message += "  Level: ";
	int places2 = numDigits(level);
	for (int k = 0; k < 2 - places2; k++)
	{
		message += '0';
	}
	string temp2 = std::to_string(level);
	message += temp2;
	message += "  Lives: ";
	int places3 = numDigits(livesLeft);
	for (int k = 0; k < 2 - places3; k++)
	{
		message += ' ';
	}
	string temp3 = std::to_string(livesLeft);
	message += temp3;
	message += "  Health: ";
	int places4 = numDigits(health);
	for (int k = 0; k < 3 - places4; k++)
	{
		message += ' ';
	}
	string temp4 = std::to_string(health);
	message += temp4;
	message += "%  Ammo: ";
	int places5 = numDigits(ammo);
	for (int k = 0; k < 3 - places5; k++)
	{
		message += ' ';
	}
	string temp5 = std::to_string(ammo);
	message += temp5;
	message += "  Bonus: ";
	int places6 = numDigits(bonus);
	for (int k = 0; k < 4 - places6; k++)
	{
		message += ' ';
	}
	string temp6 = std::to_string(bonus);
	message += temp6;
	return message;
}
int StudentWorld::numDigits(int number)
{
	int digits = 0;
	if (number >= 0 && number <= 9)
		return 1;
	while (number != 0) {
		number /= 10;
		digits++;
	}
	return digits;
}
int StudentWorld::level()
{
	int t = getLevel();
	return t;
}
unsigned int StudentWorld::BonusAmount() const
{
	return bonus;
}
void StudentWorld::setFinished()
{
	finished = true;
}
bool StudentWorld::Finished()
{
	return finished;
}

void StudentWorld::decreaseBonus()
{
	bonus--;
}
bool StudentWorld::Walkable(int xx, int yy)
{
	if (m_player->getX() == xx && m_player->getY() == yy)
		return false;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
		if (typeid(*m_actors[i]) == typeid(Wall))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
		if (typeid(*m_actors[i]) == typeid(Snarlbot))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
		if (typeid(*m_actors[i]) == typeid(Factory))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
		if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
		if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return false;
	}
	return true;
}


bool StudentWorld::HoleHere(int xx, int yy)
{
	for (int i = 0; i< m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Hole))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				return true;
	}
	return false;
}

bool StudentWorld::ExitHere(int x, int y)
{
	for (int i = 0; i< m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Exit))
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
				return true;
	}
	return false;
}
bool StudentWorld::isABoulder(int xx, int yy)
{
	bool Here = false;
	int k = m_actors.size();
	for (int i = 0; i< k; i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				Here = true;
	}
	return Here;
}

bool StudentWorld::playerHere(int xx, int yy)
{
	if (m_player->getX() == xx && m_player->getY() == yy)
		return true;
	return false;
}


void StudentWorld::DeadBoulder(int xx, int yy)
{
	for (int i = 0; i< m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
			if (m_actors[i]->getX() == xx && m_actors[i]->getY() == yy)
				m_actors[i]->setDead();
	}
}



void StudentWorld::pusher(int x, int y, GraphObject::Direction Dir)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				switch (Dir)
				{
				case (GraphObject::up) :
					m_actors[i]->push(x, y + 1, GraphObject::up);
					break;
				case (GraphObject::down) :
					m_actors[i]->push(x, y - 1, GraphObject::down);
					break;
				case (GraphObject::left) :
					m_actors[i]->push(x - 1, y, GraphObject::left);
					break;
				case (GraphObject::right) :
					m_actors[i]->push(x + 1, y, GraphObject::right);
					break;
				}
			}
		}
	}

}
bool StudentWorld::canDamage(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(Snarlbot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
	}

	if (m_player->getX() == x && m_player->getY() == y)
	{
		return true;
	}
	return false;
}

void StudentWorld::Damage(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Boulder))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				m_actors[i]->dealDamage();
			}
		}
		if (typeid(*m_actors[i]) == typeid(Snarlbot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				playSound(SOUND_ROBOT_IMPACT);
				m_actors[i]->dealDamage();
				if (!m_actors[i]->isAlive())
				{
					playSound(SOUND_ROBOT_DIE);
					increaseScore(100);
				}
			}
		}
		if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				playSound(SOUND_ROBOT_IMPACT);
				m_actors[i]->dealDamage();
				playSound(SOUND_ROBOT_DIE);
				increaseScore(10);
				if (!m_actors[i]->isAlive())
				{
					if (m_actors[i]->holding() == true)
					{
						if (m_actors[i]->returnItem() == 1)
						{
							Ammo *a = new Ammo(IID_AMMO, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(a);
							break;
						}
						if (m_actors[i]->returnItem() == 2)
						{
							RestoreHealth *R = new RestoreHealth(IID_RESTORE_HEALTH, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(R);
							break;
						}
						if (m_actors[i]->returnItem() == 3)
						{
							ExtraLife *E = new ExtraLife(IID_EXTRA_LIFE, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(E);
							break;
						}

					}
				}
			}
		}
		if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				playSound(SOUND_ROBOT_IMPACT);
				m_actors[i]->dealDamage();
				playSound(SOUND_ROBOT_DIE);
				increaseScore(20);
				if (!m_actors[i]->isAlive())
				{
					if (m_actors[i]->holding() == true)
					{
						if (m_actors[i]->returnItem() == 1)
						{
							Ammo *a = new Ammo(IID_AMMO, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(a);
							break;
						}
						if (m_actors[i]->returnItem() == 2)
						{
							RestoreHealth *R = new RestoreHealth(IID_RESTORE_HEALTH, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(R);
							break;
						}
						if (m_actors[i]->returnItem() == 3)
						{
							ExtraLife *E = new ExtraLife(IID_EXTRA_LIFE, m_actors[i]->getX(), m_actors[i]->getY(), this);
							storeObject(E);
							break;
						}

					}
				}

			}

		}
	}
	if (m_player->getX() == x && m_player->getY() == y)
	{
		playSound(SOUND_PLAYER_IMPACT);
		m_player->dealDamage();
		if (!m_player->isAlive())
		{
			playSound(SOUND_PLAYER_DIE);
			decLives();
		}

	}
}

void StudentWorld::storeObject(Actor* temp)
{
	m_actors.push_back(temp); //push the Actor pointer into the vector
}

void StudentWorld::removeDeadObjects()
{
	if (!m_player->isAlive())
		delete m_player;

	for (int i = 0; i< m_actors.size(); i++)
	{
		if (!m_actors[i]->isAlive()){
			delete m_actors[i];
			m_actors.erase(m_actors.begin() + i);
		}
	}
}

int StudentWorld::JewelsLeft()
{
	int counter = 0;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Jewel))
			counter++;
	}
	return counter;
}

void StudentWorld::Stats(int ammo, int health)
{
	if (ammo == 20)
		m_player->increaseAmmo();
	else if (health == 100)
		m_player->increaseHealth();
}
bool StudentWorld::isAGoodie(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Ammo))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(RestoreHealth))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(ExtraLife))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::canShoot(int x, int y, GraphObject::Direction dir)
{
	if (m_player->getY() != y && dir == GraphObject::right)
		return false;
	if (m_player->getX() < x  && dir == GraphObject::right)
		return false;
	if (m_player->getY() != y && dir == GraphObject::left)
		return false;
	if (m_player->getX() > x  && dir == GraphObject::left)
		return false;
	if (m_player->getX() != x && dir == GraphObject::up)
		return false;
	if (m_player->getY() < y  && dir == GraphObject::up)
		return false;
	if (m_player->getX() != x && dir == GraphObject::down)
		return false;
	if (m_player->getY() > y  && dir == GraphObject::down)
		return false;
	if (m_player->getX() < x  && dir == GraphObject::left)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (typeid(*m_actors[i]) == typeid(Boulder))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Wall))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(Snarlbot))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(Factory))
			{
				if (m_actors[i]->getX() > m_player->getX() && m_actors[i]->getX() < x && m_actors[i]->getY() == y)
					return false;
			}
		}
	}
	if (m_player->getX() > x  && dir == GraphObject::right)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (typeid(*m_actors[i]) == typeid(Boulder))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Wall))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(Snarlbot))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
					return false;
			}
			if (typeid(*m_actors[i]) == typeid(Factory))
			{
				if (m_actors[i]->getX() < m_player->getX() && m_actors[i]->getX() > x && m_actors[i]->getY() == y)
					return false;
			}
		}
	}
	if (m_player->getY() < y  && dir == GraphObject::down)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (typeid(*m_actors[i]) == typeid(Boulder))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Wall))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Snarlbot))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Kleptobot))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Factory))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
			{
				if (m_actors[i]->getY() > m_player->getY() && m_actors[i]->getY() < y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
		}
	}
	if (m_player->getY() > y  && dir == GraphObject::up)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (typeid(*m_actors[i]) == typeid(Boulder))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Wall))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Snarlbot))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Kleptobot))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
			if (typeid(*m_actors[i]) == typeid(Factory))
			{
				if (m_actors[i]->getY() < m_player->getY() && m_actors[i]->getY() > y && m_actors[i]->getX() == x)
				{
					return false;
				}
			}
		}
	}
	return true;
}

int StudentWorld::Count(int x, int y)
{
	int counter = 0;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
		{
			for (int k = 0; k <= 3; k++)
			{
				for (int j = 0; j <= 3; j++)
				{
					if (m_actors[i]->getX() == x + k && m_actors[i]->getY() == y + j && x + k < 14 && y + j < 14)
						counter++;
					if (m_actors[i]->getX() == x - k && m_actors[i]->getY() == y - j && x - k > 0 && y - j > 0)
						counter++;
					if (m_actors[i]->getX() == x + k && m_actors[i]->getY() == y - j && y - j > 0 && x + k < 14)
						counter++;
					if (m_actors[i]->getX() == x - k && m_actors[i]->getY() == y + j && y + j  < 14 && x - k >0)
						counter++;
				}

			}

		}
		if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
		{

			for (int k = 0; k <= 3; k++)
			{
				for (int j = 0; j <= 3; j++)
				{
					if (m_actors[i]->getX() == x + k && m_actors[i]->getY() == y + j && x + k < 14 && y + j < 14)
						counter++;
					if (m_actors[i]->getX() == x - k && m_actors[i]->getY() == y - j && x - k > 0 && y - j > 0)
						counter++;
					if (m_actors[i]->getX() == x + k && m_actors[i]->getY() == y - j && y - j > 0 && x + k < 14)
						counter++;
					if (m_actors[i]->getX() == x - k && m_actors[i]->getY() == y + j && y + j < 14 && x - k >0)
						counter++;
				}

			}


		}
	}
	return counter;
}


bool StudentWorld::kleptobotHere(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(NormalKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
		if (typeid(*m_actors[i]) == typeid(AngryKleptobot))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
	}
	return false;
}

int StudentWorld::stealAGoodie(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (typeid(*m_actors[i]) == typeid(Ammo))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				m_actors[i]->setDead();
				playSound(SOUND_ROBOT_MUNCH);
				return 1;
			}
		}
		if (typeid(*m_actors[i]) == typeid(RestoreHealth))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				m_actors[i]->setDead();
				playSound(SOUND_ROBOT_MUNCH);
				return 2;
			}
		}
		if (typeid(*m_actors[i]) == typeid(ExtraLife))
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				m_actors[i]->setDead();
				playSound(SOUND_ROBOT_MUNCH);
				return 3;
			}
		}
	}
}