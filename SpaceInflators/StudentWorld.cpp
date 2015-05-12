#include "StudentWorld.h"
#include "actor.h"
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

StudentWorld::StudentWorld()
{
	m_torpedoes = 0;
	m_round = 1;
	//m_shipsleft = 3;
	m_nAliens = 0;
	m_nBullets = 0;
	//m_CaptainAlive = true;
	m_nKilledAliens = 0;
	m_myShip = NULL;
}

StudentWorld::~StudentWorld()
{
	for(int i = 0; i < collectionofAllObjects.size();i++)
	{
		delete collectionofAllObjects[i];
		collectionofAllObjects.erase(collectionofAllObjects.begin()+i);
		i--;
	}

}

void StudentWorld::init()
{
	cout << "Game start" << endl;
	PlayerShip* myShip = new PlayerShip(15,1, this);
	//PlayerShip myShip(15,1);	
	//collectionofAllObjects.push_back(myShip);
	myShip->setVisible(true);
	m_myShip = myShip;
}

int StudentWorld::move()
{
	bool testparam = testParamsProvided();
	//decLives();
        //return GWSTATUS_PLAYER_DIED;
	if(testparam ==true)
	{
		if(collectionofAllObjects.size()>1)
		{
			int testIndex = getTestParam(TEST_PARAM_ACTOR_INDEX);
			if(testIndex == TEST_PARAM_NACHLING)
			{
				int startenergy = getRound() *5;
				int xcoor = rand()%VIEW_WIDTH-1;
				Nachling* newegg = new Nachling(IID_NACHLING,xcoor, VIEW_HEIGHT-10, startenergy,this);
				this->addActor(newegg);
			}

			else if(testIndex == TEST_PARAM_SMALLBOT)
			{
				int startenergy = getRound() * 12;
				int xcoor = rand()%VIEW_WIDTH-1;
				SmallBot* newegg = new SmallBot(IID_SMALLBOT, xcoor, VIEW_HEIGHT-1, startenergy, this);
				this->addActor(newegg);
			}

			else if(testIndex == TEST_PARAM_WEALTHY_NACHLING)
			{
				int startenergy = getRound() *8;
				int xcoor = rand()%VIEW_WIDTH-1;
				WealthyNachling* newegg = new WealthyNachling(IID_WEALTHY_NACHLING,xcoor, VIEW_HEIGHT-1,startenergy,this);
				this->addActor(newegg);
			}

			else if (testIndex == TEST_PARAM_GOODIE_ENERGY)
			{
				EnergyGoodie* myGift = new EnergyGoodie(IID_ENERGY_GOODIE, 15, VIEW_HEIGHT-1, this);
				myGift->getMyWorld()->addActor(myGift);
			}

			else if (testIndex == TEST_PARAM_GOODIE_FREE_SHIP)
			{
				FreeShip* myGift = new FreeShip(IID_FREE_SHIP_GOODIE, 15, VIEW_HEIGHT-1, this);
				myGift->getMyWorld()->addActor(myGift);
			}

			else if (testIndex == TEST_PARAM_GOODIE_TORPEDO)
			{
				TorpedoGoodie* myGift = new TorpedoGoodie(IID_TORPEDO_GOODIE, 15, VIEW_HEIGHT-1, this);
				myGift->getMyWorld()->addActor(myGift);
			}
		}// end if
	}// end if
	
	else
	{
		//generate aliens
		int nAliens = getnAliens();
		int allowedAliens = 2+ 0.5*getRound();
		cout << "there are: " << nAliens << "and I allow: " << allowedAliens << endl;

		if(nAliens < allowedAliens)
		{
				int det = rand() % 100;

				//add Nachling
				if(det < 70)
				{
					int adet = rand() %100;
					if(adet < 20)
					{
						int startenergy = getRound() *8;
						int xcoor = rand()%VIEW_WIDTH-1;
						WealthyNachling* newegg = new WealthyNachling(IID_WEALTHY_NACHLING,xcoor, VIEW_HEIGHT-1,startenergy,this);
						this->addActor(newegg);
						cout << "Added Wealthy." << endl;
					}
					else
					{
						int startenergy = getRound() *5;
						int xcoor = rand()%VIEW_WIDTH-1;
						Nachling* newegg = new Nachling(IID_NACHLING,xcoor, VIEW_HEIGHT-10, startenergy,this);
						this->addActor(newegg);
						cout << "Added Nachling: " << endl;
					}//end else
				}// end if
				else
				{
					int startenergy = getRound() * 12;
					int xcoor = rand()%VIEW_WIDTH-1;
					SmallBot* newegg = new SmallBot(IID_SMALLBOT, xcoor, VIEW_HEIGHT-1, startenergy, this);
					this->addActor(newegg);
					cout << "Added SmallBot" << endl;
				}//end else
		}//end if


		//generate random number for stars
		int randomstar = rand() %100;
		if(randomstar < 33)
		{
			int xc = rand()%29;
			Star* thenewStar = new Star(xc, VIEW_HEIGHT-1, this);
			collectionofAllObjects.push_back(thenewStar);
			thenewStar->setVisible(true);
		}

	}//end else

	//update text
	setHeaderText();

	m_myShip->doSomething();
	for(int i = 0; i < collectionofAllObjects.size(); i++)
			collectionofAllObjects[i]->doSomething();

	setStatus();
	bool status = removeTheDead();

	if(status)
	{
		cout << "Game Over." << endl;
		cout << getnAliens() << endl;
		this->decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	int needtoKill = 4 * getRound();
	if(nKilledAliens() >= needtoKill)
	{
		incrementRound();
		resetKilledAliens();
	}

	return GWSTATUS_CONTINUE_GAME;
}

//take in an actor pointer, dynamicall cast it and see who it is
void StudentWorld::addActor(Actor* requestor)
{
	if(requestor != NULL)
	{
		Torpedo* t = dynamic_cast<Torpedo*>(requestor);
		if(t!=NULL)
		{
			cout << "Added torpedo!" << endl;
			collectionofAllObjects.push_back(t);
			incrementBullets(1);
			t->setVisible(true);
			return;
		}
		Bullet* b = dynamic_cast<Bullet*>(requestor);
		if(b!=NULL)
		{
			
			/*if(p!=NULL)
			{*/
				collectionofAllObjects.push_back(b);
				incrementBullets(1);
				b->setVisible(true);
				return;
			//}
		}//end if

		WealthyNachling* w = dynamic_cast<WealthyNachling*>(requestor);
			if(w!=NULL)
			{
				collectionofAllObjects.push_back(w);
				w->setVisible(true);
				incrementAliens(1);
				return;
			}

		Nachling* n = dynamic_cast<Nachling*>(requestor);
		if(n!=NULL)
		{
			
			collectionofAllObjects.push_back(n);
			n->setVisible(true);
			incrementAliens(1);
			return;
		}
		
		SmallBot* sb = dynamic_cast<SmallBot*>(requestor);
		if(sb!=NULL)
		{
			collectionofAllObjects.push_back(sb);
			sb->setVisible(true);
			incrementAliens(1);
			return;
		}

		EnergyGoodie* eg = dynamic_cast<EnergyGoodie*>(requestor);
		if(eg!=NULL)
		{
			collectionofAllObjects.push_back(eg);
			eg->setVisible(true);
			return;
		}

		TorpedoGoodie* tg = dynamic_cast<TorpedoGoodie*>(requestor);
		if(tg!=NULL)
		{
			collectionofAllObjects.push_back(tg);
			tg->setVisible(true);
			return;
		}

		FreeShip* fg = dynamic_cast<FreeShip*>(requestor);
		if(fg!=NULL)
		{
			collectionofAllObjects.push_back(fg);
			fg->setVisible(true);
			return;
		}

	}//end if
}

PlayerShip* StudentWorld::getMyShip() const
{
	return m_myShip;
}

void StudentWorld::setHeaderText()
{
	int currscore = getScore();
	int currships = this->getLives();
	int currtor = getTorpedoes();
	int currround = getRound();
	double currenergy = getEnergyPer();

	string outputHeader = formatHeader(currscore, currround, currenergy, currships, currtor);

	setGameStatText(outputHeader);

}

string StudentWorld::formatHeader(int score, int round, double energy, int ships, int torpedoes)
{
	string Sscore;
	ostringstream convert;
	convert << setfill('0') << setw(7) << score;
	Sscore = convert.str();

	string Sround;
	ostringstream convert1;
	convert1 << setfill('0') << setw(2) << round;
	Sround = convert1.str();

	string Senergy;
	ostringstream convert2;
	convert2 << energy;
	Senergy = convert2.str();

	string Sships;
	ostringstream convert3;
	convert3 << setfill('0') << setw(2) <<ships;
	Sships = convert3.str();

	string Storpedoes;
	ostringstream convert4;
	convert4 << setfill('0') << setw(3) << torpedoes;
	Storpedoes = convert4.str();

	string result = "Score: " + Sscore + "  " + "Round: " + Sround + "  " + "Energy: " + Senergy + "%  " + "Torpedoes: " + Storpedoes + "  " + "Ships: " + Sships;

	return result;
}

void StudentWorld::incrementAliens(int count)
{
	m_nAliens+=count;
}

void StudentWorld::incrementBullets(int count)
{
	m_nBullets += count;
}

void StudentWorld::incrementTorpedoes(int count)
{
	m_torpedoes += count;
}

void StudentWorld::decrementBullets(int count)
{
	m_nBullets -= count;
}

void StudentWorld::decrementTorpedoes()
{
	m_torpedoes--;
}

void StudentWorld::decrementAliens(int count)
{
	if(m_nAliens>0)
		m_nAliens -=count;
}

void StudentWorld::incrementKilledAliens()
{
	m_nKilledAliens++;
}

void StudentWorld::incrementRound()
{
	m_round++;
}

void StudentWorld::resetKilledAliens()
{
	m_nKilledAliens = 0;
}

//loop through and check status (dead/alive, fallen off, shot) and set, also sets the invisibility
void StudentWorld::setStatus()
{
	for(int i = 0; i < collectionofAllObjects.size(); i++)
	{
		if(collectionofAllObjects[i]->getY()<0 || collectionofAllObjects[i]->getY() > 40 
			|| collectionofAllObjects[i]->getHPorEnergy() <= 0)
		{
			collectionofAllObjects[i]->setstatus(false);
			collectionofAllObjects[i]->setVisible(false);
		}
		//add a check for the lifetime for goodie
		Goodie* gd = dynamic_cast<Goodie*>(collectionofAllObjects[i]);
		if(gd!=NULL)
		{
			if(gd->getTick() >= gd->getLifeTime())
			{
				gd->setstatus(false);
				gd->setVisible(false);
			}
		}
	}//end for

	if(m_myShip->getHPorEnergy() <=0)
	{
		m_myShip->setstatus(false);
		m_myShip->setVisible(false);
		m_CaptainAlive = false;
	}
}

bool StudentWorld::removeTheDead()
{
	bool isCaptainDead = false;
	for(int i = 0; i<collectionofAllObjects.size();i++)
	{
		if(!collectionofAllObjects[i]->amIAlive())
		{
			Bullet* bp = dynamic_cast<Bullet*>(collectionofAllObjects[i]);
			if(bp!= NULL)
			{
				cout << "decremented bullet" << endl;
				decrementBullets(1);
			}
			Alien* np = dynamic_cast<Alien*>(collectionofAllObjects[i]);
			if(np!=NULL)
			{
				decrementAliens(1);
			}
			delete collectionofAllObjects[i];
			collectionofAllObjects.erase(collectionofAllObjects.begin()+i);
			i--;
		}
	}//end for

	if(!m_myShip->amIAlive())
	{
		isCaptainDead = true;
		//delete m_myShip;
	}

	return isCaptainDead;
}

void StudentWorld::cleanUp()
{
	for(int i = 0; i<collectionofAllObjects.size();i++)
	{
		Alien* ap = dynamic_cast<Alien*>(collectionofAllObjects[i]);
		if(ap!=NULL)
			decrementAliens(1);
		Bullet* bp = dynamic_cast<Bullet*>(collectionofAllObjects[i]);
		if(bp!=NULL)
			decrementBullets(1);
		collectionofAllObjects[i]->setVisible(false);
		delete collectionofAllObjects[i];
		collectionofAllObjects.erase(collectionofAllObjects.begin()+i);
		i--;
	}
	delete m_myShip;
}

//////////////////////////////////////////////////////////////////
// getters implementation                                       //
//////////////////////////////////////////////////////////////////

bool StudentWorld::isCaptainAlive() const
{
	return m_CaptainAlive;
}

int StudentWorld::getRound() const
{
	return m_round;
}

int StudentWorld::getTorpedoes() const
{
	return m_torpedoes;
}

int StudentWorld::shipsLeft() const
{
	return m_shipsleft;
}

double StudentWorld::getEnergyPer()
{
	//cout << "Before evaluating: " << getMyShip()->getHPorEnergy()/50.000;
	m_energyper  = getMyShip()->getHPorEnergy()/50.000;
	//cout << "From EnergyPer: " << m_energyper;
	//cout << "Percent: " << m_energyper*100;
	return m_energyper*100;
}

int StudentWorld::getnBullets() const
{
	return m_nBullets;
}

int StudentWorld::getnAliens() const
{
	return m_nAliens;
}

int StudentWorld::nKilledAliens() const
{
	return m_nKilledAliens;
}

int StudentWorld::getSizeofVector() const
{
	return collectionofAllObjects.size();
}

Actor* StudentWorld::getanActor(int index) const
{
	return collectionofAllObjects[index];
}

int StudentWorld::getnEnemyBullets()
{
	int count = 0;
	for(int i = 0; i < collectionofAllObjects.size(); i++)
	{
		Bullet* bp = dynamic_cast<Bullet*>(collectionofAllObjects[i]);
		//we found a missile!
		if(bp!=NULL)
		{
			int a = bp->getMyOwner();
			//Alien* bpp = dynamic_cast<Alien*>(a);
			if(a==ALIEN_OWNER) //we have found an alien missile!!
			count++;
		}
		
	}

	return count;
}


// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp