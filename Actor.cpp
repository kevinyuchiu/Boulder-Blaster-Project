#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cstdlib>
using namespace std;
Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction direction) :GraphObject(imageID, startX, startY, direction)
{
	setVisible(true);
	ptr1 = ptr;
	m_alive = true;
	m_health = hp;
	hasItem = false;
	itemType = 0;
}
Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction direction) :GraphObject(imageID, startX, startY, direction)
{
	ptr1 = ptr;
	m_alive = true;
}
bool Actor::isAlive()
{
	return m_alive;
}
int Actor::health()
{
	return m_health;
}
void Actor::setDead()
{
	m_alive = false;
}

StudentWorld* Actor::getWorld()
{
	return ptr1;
}
Actor::~Actor()
{

}

Player::Player(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction direction) : Actor(imageID, startX, startY, ptr, hp, direction)
{
	m_ammo = 20;
}
void Player::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_UP:
			if (getWorld()->Walkable(getX(), getY() + 2) && !getWorld()->isAGoodie(getX(), getY() + 2) && !getWorld()->ExitHere(getX(), getY() + 2) && getWorld()->isABoulder(getX(), getY() + 1))
			{
				setDirection(up);
				Direction dir = getDirection();
				getWorld()->pusher(getX(), getY() + 1, dir);
				moveTo(getX(), getY() + 1);
			}
			else if (getWorld()->Walkable(getX(), getY() + 1) && !getWorld()->HoleHere(getX(), getY() + 1))
			{

				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			if (getWorld()->Walkable(getX(), getY() - 2) && !getWorld()->isAGoodie(getX(), getY() - 2) && !getWorld()->ExitHere(getX(), getY() - 2) && getWorld()->isABoulder(getX(), getY() - 1))
			{
				setDirection(down);
				Direction dir = getDirection();
				getWorld()->pusher(getX(), getY() - 1, dir);
				moveTo(getX(), getY() - 1);
			}
			else if (getWorld()->Walkable(getX(), getY() - 1) && !getWorld()->HoleHere(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			setDirection(down);
			break;
		case KEY_PRESS_LEFT:
			if (getWorld()->Walkable(getX() - 2, getY()) && !getWorld()->isAGoodie(getX() - 2, getY()) && !getWorld()->ExitHere(getX() - 2, getY()) && getWorld()->isABoulder(getX() - 1, getY()))
			{
				setDirection(left);
				Direction dir = getDirection();
				getWorld()->pusher(getX() - 1, getY(), dir);
				moveTo(getX() - 1, getY());
			}
			else if (getWorld()->Walkable(getX() - 1, getY()) && !getWorld()->HoleHere(getX() - 1, getY()))
			{

				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (getWorld()->Walkable(getX() + 2, getY()) && !getWorld()->isAGoodie(getX() + 2, getY()) && !getWorld()->ExitHere(getX() + 2, getY()) && getWorld()->isABoulder(getX() + 1, getY()))
			{
				setDirection(right);
				Direction dir = getDirection();
				getWorld()->pusher(getX() + 1, getY(), dir);
				moveTo(getX() + 1, getY());
			}
			else if (getWorld()->Walkable(getX() + 1, getY()) && !getWorld()->HoleHere(getX() + 1, getY()))
			{

				setDirection(right);
				moveTo(getX() + 1, getY());
			}
			setDirection(right);
			break;
		case KEY_PRESS_ESCAPE:
			getWorld()->decLives();
			setDead();
			break;
		case KEY_PRESS_SPACE:
			int x = getX();
			int y = getY();
			if (getDirection() == up)
				y++;
			else if (getDirection() == down)
				y--;
			else if (getDirection() == left)
				x--;
			else if (getDirection() == right)
				x++;
			if (ammoAmt() != 0)
			{
				m_ammo--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				Bullet* b = new Bullet(IID_BULLET, x, y, getWorld(), getDirection());
				getWorld()->storeObject(b);
			}
			break;
		}
	}
}

int Player::ammoAmt()
{
	return m_ammo;
}
void Player::increaseAmmo()
{
	m_ammo += 20;
}




void Wall::doSomething()
{
	return;
}

Boulder::Boulder(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction direction) :Actor(imageID, startX, startY, ptr, hp, direction)
{

}

void Boulder::push(int x, int y, Direction dir)
{
	switch (dir)
	{
	case up:
		if (getWorld()->Walkable(x, y) && !getWorld()->isAGoodie(x, y) && !getWorld()->ExitHere(x, y))//later add that a hole is okay
			moveTo(x, y);
		break;
	case down:
		if (getWorld()->Walkable(x, y) && !getWorld()->isAGoodie(x, y) && !getWorld()->ExitHere(x, y))//later add that a hole is okay
			moveTo(x, y);
		break;
	case left:
		if (getWorld()->Walkable(x, y) && !getWorld()->isAGoodie(x, y) && !getWorld()->ExitHere(x, y))//later add that a hole is okay
			moveTo(x, y);
		break;
	case right:
		if (getWorld()->Walkable(x, y) && !getWorld()->isAGoodie(x, y) && !getWorld()->ExitHere(x, y))//later add that a hole is okay
			moveTo(x, y);
		break;
	}
}



Hole::Hole(int imageID, int startX, int startY, StudentWorld* ptr, Direction Direction) : Actor(imageID, startX, startY, ptr, Direction)
{
	setVisible(true);
}

void Hole::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isABoulder(getX(), getY()))
	{
		setDead();
		getWorld()->DeadBoulder(getX(), getY());
	}
}

void Bullet::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	Direction dir = getDirection();
	int x = getX();
	int y = getY();
	if (getDirection() == up)
		y++;
	else if (getDirection() == down)
		y--;
	else if (getDirection() == left)
		x--;
	else if (getDirection() == right)
		x++;
	if (getWorld()->canDamage(getX(), getY()))
	{
		getWorld()->Damage(getX(), getY());
		setDead();
	}
	else if (getWorld()->canDamage(x, y))
	{
		getWorld()->Damage(x, y);
		setDead();
	}
	else if (!getWorld()->Walkable(getX(), getY()))
		setDead();
	else
	{
		switch (dir)
		{
		case (up) :
			if (getWorld()->Walkable(getX(), getY() + 1))
			{
			moveTo(getX(), getY() + 1);
			}
			else
				setDead();
			break;
		case(down) :
			if (getWorld()->Walkable(getX(), getY() - 1))
			{
			moveTo(getX(), getY() - 1);
			}
			else
				setDead();
			break;
		case(left) :
			if (getWorld()->Walkable(getX() - 1, getY()))
			{
			moveTo(getX() - 1, getY());
			}
			else
				setDead();
			break;
		case(right) :
			if (getWorld()->Walkable(getX() + 1, getY()))
			{
			moveTo(getX() + 1, getY());
			}
			else
				setDead();
			break;
		}
	}
}


void Jewel::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->playerHere(getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
		setDead();
	}
}

void Exit::doSomething()
{
	if (getWorld()->JewelsLeft() != 0)
	{
		return;
	}
	else if (getWorld()->JewelsLeft() == 0)
	{
		if (visible == false)
		{
			setVisible(true);
			getWorld()->playSound(SOUND_REVEAL_EXIT);
			visible = true;
		}
	}
	if (getWorld()->playerHere(getX(), getY()))
	{
		getWorld()->setFinished();
		getWorld()->increaseScore(2000 + getWorld()->BonusAmount());
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
	}
}

void ExtraLife::doSomething()
{
	if (!isAlive())
		return;
	else if (getWorld()->playerHere(getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->incLives();
	}
}

void Ammo::doSomething()
{
	if (!isAlive())
		return;
	else if (getWorld()->playerHere(getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->Stats(20, 0);
		setDead();
	}
}

void RestoreHealth::doSomething()
{
	if (!isAlive())
		return;
	else if (getWorld()->playerHere(getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(500);
		getWorld()->Stats(0, 100);
		setDead();
	}
}
Robot::Robot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir) :Actor(imageID, startX, startY, ptr, hp, dir)
{
	int  t = getWorld()->getLevel();
	m_ticks = (28 - t) / 4;
	if (m_ticks < 3)
		m_ticks = 3;
	int counter = 1;

}
bool Robot::shoot()
{
	if (getDirection() == right && getWorld()->canShoot(getX(), getY(), getDirection()))
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		Bullet* b = new Bullet(IID_BULLET, getX() + 1, getY(), getWorld(), getDirection());
		getWorld()->storeObject(b);
		setCounter();
		return true;
	}
	if (getDirection() == left && getWorld()->canShoot(getX(), getY(), getDirection()))
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		Bullet* b = new Bullet(IID_BULLET, getX() - 1, getY(), getWorld(), getDirection());
		getWorld()->storeObject(b);
		setCounter();
		return true;
	}
	if (getDirection() == down && getWorld()->canShoot(getX(), getY(), getDirection()))
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		Bullet* b = new Bullet(IID_BULLET, getX(), getY() - 1, getWorld(), getDirection());
		getWorld()->storeObject(b);
		setCounter();
		return true;
	}
	if (getDirection() == up && getWorld()->canShoot(getX(), getY(), getDirection()))
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		Bullet* b = new Bullet(IID_BULLET, getX(), getY() + 1, getWorld(), getDirection());
		getWorld()->storeObject(b);
		setCounter();
		return true;
	}
	return false;
}
void Snarlbot::doSomething()
{
	if (!isAlive())
		return;
	if (getCounter() != getTicks())
	{
		CounterIncrementer();
		return;
	}
	else
	{
		if (shoot())
			return;
		if (getDirection() == right && getWorld()->Walkable(getX() + 1, getY()) && !getWorld()->HoleHere(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			setCounter();
			return;
		}
		else if (getDirection() == right && (!getWorld()->Walkable(getX() + 1, getY()) || getWorld()->HoleHere(getX() + 1, getY())))
		{
			setDirection(left);
			setCounter();
			return;

		}
		if (getDirection() == left && getWorld()->Walkable(getX() - 1, getY()) && !getWorld()->HoleHere(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			setCounter();
			return;
		}
		else if (getDirection() == left && (!getWorld()->Walkable(getX() - 1, getY()) || getWorld()->HoleHere(getX() - 1, getY())))
		{
			setDirection(right);
			setCounter();
			return;
		}
		if (getDirection() == down && getWorld()->Walkable(getX(), getY() - 1) && !getWorld()->HoleHere(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			setCounter();
			return;
		}
		else if (getDirection() == down && (!getWorld()->Walkable(getX(), getY() - 1) || getWorld()->HoleHere(getX() + 1, getY() - 1)))
		{
			setDirection(up);
			setCounter();
			return;
		}
		if (getDirection() == up && getWorld()->Walkable(getX(), getY() + 1) && !getWorld()->HoleHere(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			setCounter();
			return;
		}
		else if (getDirection() == up && (!getWorld()->Walkable(getX(), getY() + 1) || getWorld()->HoleHere(getX(), getY() + 1)))
		{
			setDirection(down);
			setCounter();
			return;
		}
	}

}
int Kleptobot::distanceBeforeTurning()
{
	return distance;
}
void Kleptobot::decrementDistance()
{
	distance--;
}
void Kleptobot::stealGoodie()
{
	int w = 4;
	if (rand() % 10 == w && !holding())
	{
		setItem(getWorld()->stealAGoodie(getX(), getY()));
		setHolding();
	}
}
void Kleptobot::move()
{
	switch (getDirection())
	{
	case right:
	{
		if (getWorld()->Walkable(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			decrementDistance();
		}
		break;
	}
	case left:
	{
		if (getWorld()->Walkable(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			decrementDistance();
		}
		break;
	}
	case up:
	{
		if (getWorld()->Walkable(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			decrementDistance();
		}
		break;
	}
	case down:
	{
		if (getWorld()->Walkable(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			decrementDistance();
		}
		break;
	}
	}
}
void Kleptobot::moveRand()
{
	setDistance();
	int i = 0;
	int temp = 0;
	int d = rand() % 4 + 1;
	int considered[4] = { 0, 0, 0, 0 };

	while (considered[0] != d && considered[1] != d && considered[2] != d && considered[3] != d)
	{
		if (d == 1 && getWorld()->Walkable(getX(), getY() + 1) && !getWorld()->HoleHere(getX(), getY() + 1))//NORTH
		{
			setDirection(up);
			moveTo(getX(), getY() + 1);
			break;
		}
		else if (d == 1 && (!getWorld()->Walkable(getX(), getY() + 1) || getWorld()->HoleHere(getX(), getY() + 1)))//NORTH
		{
			setDirection(up);
		}
		if (d == 2 && getWorld()->Walkable(getX(), getY() - 1) && !getWorld()->HoleHere(getX(), getY() - 1))//SOUTH
		{
			setDirection(down);
			moveTo(getX(), getY() - 1);
			break;
		}
		else if (d == 2 && (!getWorld()->Walkable(getX(), getY() - 1) || getWorld()->HoleHere(getX(), getY() - 1)))//SOUTH
		{
			setDirection(down);
		}
		if (d == 3 && getWorld()->Walkable(getX() + 1, getY()) && !getWorld()->HoleHere(getX() + 1, getY()))//RIGHT
		{
			setDirection(right);
			moveTo(getX() + 1, getY());
			break;
		}
		else if (d == 3 && (!getWorld()->Walkable(getX() + 1, getY()) || getWorld()->HoleHere(getX() + 1, getY())))//RIGHT
		{
			setDirection(right);
		}
		if (d == 4 && getWorld()->Walkable(getX() - 1, getY()) && !getWorld()->HoleHere(getX() - 1, getY()))//LEFT
		{
			setDirection(left);
			moveTo(getX() - 1, getY());
			break;
		}
		else if (d == 4 && (!getWorld()->Walkable(getX(), getY() - 1) || getWorld()->HoleHere(getX(), getY() - 1)))//LEFT
		{
			setDirection(left);
		}

		temp = d;
		considered[i] = temp;
		while (d == temp)
		{
			d = rand() % 4 + 1;
		}
		i++;
		if (i == 4)
			break;
	}
}

void Kleptobot::setDistance()
{
	distance = rand() % 6 + 1;
}
void NormalKleptobot::doSomething()
{
	if (!isAlive())
		return;
	if (getTicks() != getCounter())
	{
		CounterIncrementer();
		return;
	}
	int zz = getX();
	int pp = getY();
	if (getDirection() == up)
		pp++;
	else if (getDirection() == down)
		pp--;
	else if (getDirection() == left)
		zz--;
	else if (getDirection() == right)
		zz++;
	if (getTicks() == getCounter())
	{
		if (getWorld()->isAGoodie(getX(), getY()))
		{
			stealGoodie();
		}
		else if (distanceBeforeTurning() != 0 && getWorld()->Walkable(zz, pp) && !getWorld()->HoleHere(zz, pp))
		{
			move();
		}
		else
		{
			moveRand();
		}

	}

	setCounter();
}
void AngryKleptobot::doSomething()
{
	if (!isAlive())
		return;
	if (getTicks() != getCounter())
	{
		CounterIncrementer();
		return;
	}
	int zz = getX();
	int pp = getY();
	if (getDirection() == up)
		pp++;
	else if (getDirection() == down)
		pp--;
	else if (getDirection() == left)
		zz--;
	else if (getDirection() == right)
		zz++;
	if (getTicks() == getCounter())
	{
		if (shoot())
			return;
		if (getWorld()->isAGoodie(getX(), getY()))
		{
			stealGoodie();
		}
		else if (distanceBeforeTurning() != 0 && getWorld()->Walkable(zz, pp) && !getWorld()->HoleHere(zz, pp))
		{
			move();
		}
		else
		{
			moveRand();

		}
	}

	setCounter();
}

void Factory::doSomething()
{
	int chance = 20;
	if (getWorld()->Count(getX(), getY()) < 3 && !getWorld()->kleptobotHere(getX(), getY()))
	{
		if (FactoryProductType == 1 && rand() % 50 == chance)//production of normal kleptos
		{
			getWorld()->playSound(SOUND_ROBOT_BORN);
			NormalKleptobot* k = new NormalKleptobot(IID_KLEPTOBOT, getX(), getY(), getWorld(), 6, right);
			getWorld()->storeObject(k);
		}
		if (FactoryProductType == 2 && rand() % 50 == chance)//production of normal kleptos
		{
			getWorld()->playSound(SOUND_ROBOT_BORN);
			AngryKleptobot* ak = new AngryKleptobot(IID_KLEPTOBOT, getX(), getY(), getWorld(), 8, right);
			getWorld()->storeObject(ak);
		}

	}
}