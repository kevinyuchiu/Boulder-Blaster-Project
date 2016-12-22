#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <iostream>
using namespace std;
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction startDirection);
	Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction startDirection);
	virtual void doSomething() = 0;
	virtual void push(int x, int y, Direction dir)
	{
		return;
	}
	StudentWorld* getWorld();
	virtual ~Actor();
	bool isAlive();
	void setDead();
	int health();
	int returnItem()
	{
		return itemType;
	}
	void setItem(int x)
	{
		itemType = x;
	}
	void dealDamage()
	{
		m_health -= 2;
		if (m_health == 0)
		{
			setDead();
		}
	}
	void increaseHealth()
	{
		m_health = 20;
	}
	void setHolding()
	{
		hasItem = true;
	}
	bool holding()
	{
		return hasItem;
	}
private:
	StudentWorld* ptr1;
	bool m_alive;
	int m_health;
	bool hasItem;
	int itemType;
};

class Player : public Actor
{
public:
	Player(int imageID, int startX, int startY, StudentWorld* ptr, int hp = 20, Direction startDirection = right);
	int ammoAmt();
	virtual void doSomething();
	virtual ~Player()
	{

	}
	void increaseAmmo();
private:
	int m_ammo;
};

class Wall : public Actor
{
public:
	Wall(int imageID, int startX, int startY, StudentWorld* ptr, Direction startDirection = none) : Actor(imageID, startX, startY, ptr, startDirection)
	{
		setVisible(true);
	}
	virtual void doSomething();
	virtual ~Wall()
	{

	}
};

class Boulder : public Actor
{
public:
	Boulder(int imageID, int startX, int startY, StudentWorld* ptr, int hp = 10, Direction startDirection = none);
	virtual void doSomething()
	{
		return;
	}
	virtual ~Boulder()
	{
	}
	virtual void push(int x, int y, Direction dir);
};

class Hole :public Actor
{
public:
	Hole(int imageID, int startX, int startY, StudentWorld* ptr, Direction startDirection = none);
	virtual void doSomething();
	virtual ~Hole()
	{

	}
};

class Bullet :public Actor
{
public:
	Bullet::Bullet(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir) :Actor(imageID, startX, startY, ptr, dir)
	{
		setVisible(true);
	}
	virtual void doSomething();
	virtual ~Bullet()
	{
	}
};

class Jewel :public Actor
{
public:
	Jewel::Jewel(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none) :Actor(imageID, startX, startY, ptr, dir)
	{
		setVisible(true);
	}
	virtual void doSomething();
	virtual ~Jewel()
	{

	}
};

class Exit :public Actor
{
public:
	Exit::Exit(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none) :Actor(imageID, startX, startY, ptr, dir)
	{
		visible = false;
	}
	virtual void doSomething();
	virtual ~Exit()
	{

	}
private:
	bool visible;


};

class Goodie : public Actor
{
public:
	Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir) :Actor(imageID, startX, startY, ptr, dir)
	{
		setVisible(true);
	}
	virtual void doSomething() = 0;
	virtual ~Goodie()
	{

	}

};

class ExtraLife :public Goodie
{
public:
	ExtraLife::ExtraLife(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none) :Goodie(imageID, startX, startY, ptr, dir)
	{
	}
	virtual void doSomething();
	virtual ~ExtraLife()
	{

	}

};

class Ammo :public Goodie
{
public:
	Ammo::Ammo(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none) :Goodie(imageID, startX, startY, ptr, dir)
	{
	}
	virtual void doSomething();
	virtual ~Ammo()
	{

	}

};
class RestoreHealth :public Goodie
{
public:
	RestoreHealth::RestoreHealth(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none) :Goodie(imageID, startX, startY, ptr, dir)
	{
	}
	virtual void doSomething();
	virtual ~RestoreHealth()
	{

	}

};
class Robot :public Actor
{
public:
	Robot::Robot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir);
	virtual void doSomething() = 0;
	int getCounter()
	{
		return counter;
	}
	void CounterIncrementer()
	{
		counter++;
	}
	void setCounter()
	{
		counter = 1;
	}
	int getTicks()
	{
		return m_ticks;
	}
	virtual ~Robot()
	{

	}
	bool shoot();
private:
	int counter;
	int m_ticks;
};


class Snarlbot :public Robot
{
public:
	Snarlbot::Snarlbot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir) :Robot(imageID, startX, startY, ptr, hp, dir)
	{

	}
	virtual void doSomething();
	virtual ~Snarlbot()
	{

	}
};

class Kleptobot :public Robot
{
public:
	Kleptobot::Kleptobot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir) :Robot(imageID, startX, startY, ptr, hp, dir)
	{
		distance = rand() % 6 + 1;
		direction = 0;
	}
	virtual void doSomething() = 0;
	virtual ~Kleptobot()
	{

	}
	int distanceBeforeTurning();
	void decrementDistance();
	void setDistance();
	void moveRand();
	void stealGoodie();
	void move();
private:
	int distance;
	int direction;
	bool hasItem;
};

class NormalKleptobot :public Kleptobot
{
public:
	NormalKleptobot::NormalKleptobot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir) :Kleptobot(imageID, startX, startY, ptr, hp, dir)
	{

	}
	virtual void doSomething();
	virtual ~NormalKleptobot()
	{

	}
};

class AngryKleptobot :public Kleptobot
{
public:
	AngryKleptobot::AngryKleptobot(int imageID, int startX, int startY, StudentWorld* ptr, int hp, Direction dir) :Kleptobot(imageID, startX, startY, ptr, hp, dir)
	{

	}
	virtual void doSomething();
	virtual ~AngryKleptobot()
	{

	}
};
class Factory :public Actor
{
public:
	Factory::Factory(int imageID, int startX, int startY, int type, StudentWorld* ptr, Direction dir) :Actor(imageID, startX, startY, ptr, dir)
	{
		setVisible(true);
		FactoryProductType = type;
	}
	virtual void doSomething();
	virtual ~Factory()
	{

	}
private:
	int FactoryProductType;

};


#endif // ACTOR_H_
