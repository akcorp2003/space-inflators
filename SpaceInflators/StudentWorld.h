#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "actor.h"
#include <vector>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp

class Actor;
class PlayerShip;

class StudentWorld : public GameWorld
{
public:
	StudentWorld();
	virtual ~StudentWorld();

	virtual void init();
	//PERSONAL NOTE: move() is the ticker
	virtual int move();
	virtual void cleanUp();

	//important functions
	void setStatus();
	bool removeTheDead();
	void addActor(Actor* requestor);
	void setHeaderText();
	std::string formatHeader(int score, int round, double energy, int ships, int torpedoes);

	void incrementAliens(int count);
	void incrementBullets(int count);
	void decrementBullets(int count);
	void incrementTorpedoes(int count);
	void decrementTorpedoes();
	void decrementAliens(int count);
	void incrementKilledAliens();
	void resetKilledAliens();
	void resetEnergy();
	void incrementRound();
	
	
	//properties
	PlayerShip* getMyShip() const;
	Actor* getanActor(int index) const;
	int getSizeofVector() const;
	int getTorpedoes() const;
	int getRound() const;
	double getEnergyPer();
	int shipsLeft() const;
	bool isCaptainAlive() const;

	int getnAliens() const;
	int getnBullets() const;
	int getnEnemyBullets();
	int nKilledAliens() const;
private:
	std::vector<Actor*> collectionofAllObjects;
	//ship variables
	PlayerShip* m_myShip;
	int m_torpedoes;
	int m_round;
	int m_shipsleft;
	double m_energyper;
	//alien variables
	int m_nAliens;
	int m_nBullets;
	int m_nKilledAliens;
	bool m_CaptainAlive;
	

};

#endif // _GAMEWORLD_H_
