#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
// Students:  Add code to this file, actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int ALIEN_OWNER = 1;
const int SHIP_OWNER = 2;

class StudentWorld;
class Actor;

class Actor : public GraphObject
{
	public: 
		Actor(int ID, int xc, int yc, int hp, StudentWorld* myWorld);
		virtual ~Actor();

		// properties
		int getTick() const;
		int getHPorEnergy() const;
		bool amIAlive() const;
		virtual StudentWorld* getMyWorld() const;
		Actor* didICollide(Actor* caller, int myx, int myy, int dam, bool& isCollision);

		//actions
		void setstatus(bool Alive);
		void incrementTick(int value);
		void setTick(int value);
		virtual void doSomething()=0;
		void damage(int loss);
		void resetEnergy(int gain); //should only be applied to ship
	private:
		int m_tick;
		int m_hp;
		bool m_alive;
		StudentWorld* m_myWorld;
};

//////////////////////////////////////////////////////////////
// The Allied Forces                                        //
/////////////////////////////////////////////////////////////

class PlayerShip : public Actor
{
	public:
		PlayerShip(int xcoor, int ycoor, StudentWorld* myWorld);
		PlayerShip(const PlayerShip& src);
		virtual ~PlayerShip();

		//void damage(int loss);
		//void resetEnergy(int gain);

		//properties
		virtual void doSomething();
		virtual StudentWorld* getMyWorld() const;
		bool getCaptainStatus() const;
		//int getEnergy() const;
	private:
		int m_xcoor;
		int m_ycoor;
		StudentWorld* m_shipsworld;
		int m_energy;
		int m_hp;
		bool m_alive;
};

/////////////////////////////////////////////////////////////
// The Neutral                                             //
/////////////////////////////////////////////////////////////

class Star : public Actor
{
	public:
		Star(int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~Star();

		//properties
		virtual void doSomething();
	private:
		int m_xcoor;
		int m_ycoor;
};

class Bullet : public Actor
{
	public:
		Bullet(int ID, int xcoor, int ycoor, int myOwner, StudentWorld* myWorld);
		virtual ~Bullet();
		int getMyOwner() const;

		//properties
		virtual void doSomething();
	private:
		int m_xcoor;
		int m_ycoor;
		int m_myOwner;

};

class Torpedo : public Bullet
{
	public: 
		Torpedo(int ID, int xcoor, int ycoor, int myOwner, StudentWorld* myWorld);
		virtual ~Torpedo();

		virtual void doSomething();

	private:
};

//////////////////////////////////////////////////////////////
// The Provisions                                           //
//////////////////////////////////////////////////////////////

class Goodie : public Actor
{
	public:
		Goodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~Goodie();

		virtual void doSomething();
		void doSomethingGShared();
		//void setLife();
		void resetLocalTick();
		void incrementLocalTick();

		//properties
		int getLifeTime() const;
		int getLocalTick() const;
	private:
		int m_life;
		int m_localTick;
};

class EnergyGoodie : public Goodie
{
	public:
		EnergyGoodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~EnergyGoodie();

		virtual void doSomething();
	private:
};

class TorpedoGoodie : public Goodie
{
	public:
		TorpedoGoodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~TorpedoGoodie();

		virtual void doSomething();
	private:
};

class FreeShip : public Goodie
{
	public:
		FreeShip(int ID, int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~FreeShip();

		virtual void doSomething();
	private:
};

//////////////////////////////////////////////////////////////
// The Covenant                                             //
//////////////////////////////////////////////////////////////

class Alien : public Actor
{
	public:
		Alien(int ID, int hp, int xcoor, int ycoor, StudentWorld* myWorld);
		virtual ~Alien();

		//properties
		//int getHP() const;
		//int damage(int loss);
	private:
		int m_xcoor;
		int m_ycoor;
		int m_hp;
};

class SmallBot : public Alien
{
	public:
		SmallBot(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld);
		virtual ~SmallBot();

		void setIsHit(bool hit);
		//properties
		virtual void doSomething();
		bool isJustHit() const;
	private:
		int m_xcoor;
		int m_ycoor;
		bool m_justHit;
};

class Nachling : public Alien
{
	public:
		Nachling(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld);
		virtual ~Nachling();

		void changeState(int value);
		int computeMDB();
		void setAllWeirdVariables(int HMR, int HMDr, int HMD, int MDB);
		void setHMDr(int value);
		void setHMR(int value);
		void doSomethingShared();

		//properties
		int getState() const;
		virtual void doSomething();
		int getHMR() const;
		int getHMDr() const;
		int getHMD() const;
		int getMDB() const;
	private:
		int m_xcoor;
		int m_ycoor;
		int m_state;
		int m_HMD;
		int m_HMDr;
		int m_HMR;
		int m_MDB;

};

class WealthyNachling : public Nachling
{
	public:
		WealthyNachling(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld);
		virtual ~WealthyNachling();

		virtual void doSomething();
		void setMalfunct();
		void repair();

		//properties
		bool isMalfunct() const;
	private:
		int m_xcoor;
		int m_ycoor;
		bool m_malfunction;
};

#endif // _ACTOR_H_