#include "actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

// Students:  Add code to this file (if you wish), actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////
// Collision Function Implementation                     //
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Actor Implementation	                                 //
///////////////////////////////////////////////////////////

Actor::Actor(int ID, int xc, int yc, int hp, StudentWorld* myWorld)
	:GraphObject(ID, xc, yc)
{
	m_tick = 0;
	m_myWorld = myWorld;
	m_hp = hp;
}

Actor::~Actor()
{
}

//condition: must pass in isCollision as False, handles the collision
//and invisibility factor
Actor* Actor::didICollide(Actor* caller, int x, int y, int dam, bool& isCollision)
{
	//loop through entire collection vector
	int size = getMyWorld()->getSizeofVector();

	//cout << "Killed Aliens" << getMyWorld()->nKilledAliens() << endl;

	//Alien handles if it has collided with bullet
	Alien* tempcaller = dynamic_cast<Alien*>(caller);
	if(tempcaller!=NULL) //we know it is an alien so apply loop and actions on the alien
	{
		for(int i = 0; i<size; i++)
		{
			Actor* candidate = getMyWorld()->getanActor(i);
			Bullet* tempbullet = dynamic_cast<Bullet*>(candidate);
			if(tempbullet!=NULL)
			{
				if((tempcaller->getX() == tempbullet->getX()) && (tempcaller->getY() == tempbullet->getY()))
				{
					int tempbowner = tempbullet->getMyOwner();
					//PlayerShip* pTS = dynamic_cast<PlayerShip*>(tempbowner);
					if(tempbowner == SHIP_OWNER)//the alien got hit!!
					{
						cout << "Alien got hit!" << endl;
						Torpedo* temptor = dynamic_cast<Torpedo*>(tempbullet);
						if(temptor!=NULL)
						{
							tempcaller->damage(6);
							temptor->setstatus(false);
						}
						tempcaller->damage(2);
						tempbullet->getMyWorld()->playSound(SOUND_ENEMY_HIT);
						tempbullet->setstatus(false);
						//check status of alien
						if(tempcaller->getHPorEnergy() <=0)
						{
							WealthyNachling* pWA = dynamic_cast<WealthyNachling*>(tempcaller);
							if(pWA!=NULL)
							{
								tempcaller->getMyWorld()->increaseScore(200); //because the next statement will be called
								
								int decision = rand()%2;
								if(decision == 1) //drop a goodie
								{
									int secdec = rand()%2;
									if(secdec == 0) //drop energy
									{
										EnergyGoodie* myGift = new EnergyGoodie(IID_ENERGY_GOODIE, this->getX(), this->getY()-1, this->getMyWorld());
										getMyWorld()->addActor(myGift);
									}
									else //drop torpedo
									{
										TorpedoGoodie* myGift = new TorpedoGoodie(IID_TORPEDO_GOODIE, this->getX(), this->getY()-1, this->getMyWorld());
										getMyWorld()->addActor(myGift);
									}
								}//end if
							}
							SmallBot* pSB = dynamic_cast<SmallBot*>(tempcaller);
							if(pSB!=NULL)
							{
								tempcaller->getMyWorld()->increaseScore(500);
								
								FreeShip* myGift = new FreeShip(IID_FREE_SHIP_GOODIE, this->getX(), this->getY()-1, this->getMyWorld());
								getMyWorld()->addActor(myGift);
							}
							tempcaller->getMyWorld()->increaseScore(1000);
							tempcaller->getMyWorld()->incrementKilledAliens();
							//cout << "Killed Aliens: " << tempcaller->getMyWorld()->nKilledAliens() << endl;
						}

						SmallBot* pSB = dynamic_cast<SmallBot*>(tempcaller);
						if(pSB!=NULL)
						{
							pSB->setIsHit(true);
						}
					}
				}
				//no check for collision on player because player checks that
			}//end if
		}//end for
	}//end Alien if

	PlayerShip* ptempcaller = dynamic_cast<PlayerShip*>(caller);
	if(ptempcaller!=NULL)
	{
		//in here, handle the goodies
		for(int i = 0; i<size; i++)
		{
			Actor* candidate = getMyWorld()->getanActor(i);
			Alien* tempalien = dynamic_cast<Alien*>(candidate);
			if(tempalien!=NULL) //we have found an alien!
			{
				if((ptempcaller->getX() == tempalien->getX()) && (ptempcaller->getY() == tempalien->getY())) //we have found our target!
				{
					cout << "I collided with Alien!" << endl;
					int aHP = tempalien->getHPorEnergy();
					ptempcaller->damage(15); //check and make sure that I have handled collision with Player properly
					//cout << "Alien Energy:" << tempalien->getHPorEnergy();
					tempalien->damage(aHP);
					//cout << " after energy: " << tempalien->getHPorEnergy();
					//ap->setVisible(false); //collision will set the invisibility
					//ap->getMyWorld()->decrementAliens(1);
					ptempcaller->getMyWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
					return NULL;
				}
			}//end if
			Bullet* tempbullet = dynamic_cast<Bullet*>(candidate);
			if(tempbullet!=NULL)
			{
				//cout << "Found a bullet" << endl;
				//cout << tempbullet->getX() << endl;
				if((ptempcaller->getX() == tempbullet->getX()) && (ptempcaller->getY() == tempbullet->getY()))
				{
					//cout << "Entered if" << endl;
					int tempbowner = tempbullet->getMyOwner();
					//Alien* pTA = dynamic_cast<Alien*>(tempbowner);
					if(tempbowner == ALIEN_OWNER)//the alien got hit!!
					{
						//cout << "I got hit!" << endl;
						ptempcaller->damage(2);
						tempbullet->getMyWorld()->playSound(SOUND_PLAYER_HIT);
						tempbullet->setstatus(false);
					}
				}
			}
		}//end for
	}//end player if

	Goodie* tempgood = dynamic_cast<Goodie*>(caller);
	if(tempgood!=NULL)
	{
		if((tempgood->getX() == tempgood->getMyWorld()->getMyShip()->getX()) && (tempgood->getY() ==
			tempgood->getMyWorld()->getMyShip()->getY()))
		{
			isCollision = true;
			tempgood->getMyWorld()->playSound(SOUND_GOT_GOODIE);
			tempgood->setstatus(false);
		}
	}

	return NULL;
}


	//------------------------------------------------------------------//
	//here is where the original code starts
	/*for(int i = 0; i<size; i++)
	{
		Actor* candidate = getMyWorld()->getanActor(i);

		//cast all of the actors into the various types, Alien can be the same
		Alien* ap = dynamic_cast<Alien*>(candidate);
		if(ap!=NULL && this != ap)
		{
			if((ap->getX() == this->getX() && ap->getY() == this->getY()))
			{
				isCollision = true;
				PlayerShip* psp = dynamic_cast<PlayerShip*>(this);
				if(psp!=NULL)
				{
					cout << "I collided with Player!" << endl;
					int aHP = ap->getHPorEnergy();
					ap->damage(aHP); //check and make sure that I have handled collision with Player properly
					cout << "Player Energy:" << psp->getHPorEnergy();
					psp->damage(15);
					cout << "Player after energy: " << psp->getHPorEnergy();
					//ap->setVisible(false); //collision will set the invisibility
					//ap->getMyWorld()->decrementAliens(1);
					ap->setstatus(false);
					psp->getMyWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
				}
				/*Bullet* bp = dynamic_cast<Bullet*>(this);
				if(bp!=NULL)
				{
					Actor* app = bp->getMyOwner();
					PlayerShip* pP = dynamic_cast<PlayerShip*>(app);
					if(pP!=NULL)
					{
						ap->damage(2);
						if(ap->getHPorEnergy() <= 0)
						{
							ap->setVisible(false);
							ap->getMyWorld()->decrementAliens(1);
						}
					}
				}//end if*/
			//}//end if
		//}//end if
		/*PlayerShip* pp = dynamic_cast<PlayerShip*>(candidate);
		if(pp!=NULL && this != pp)
		{
			if((pp->getX() == this->getX() && pp->getY() == this->getY()))
				isCollision = true;

			//letting the aliens decide if collision with player

				Bullet* bp = dynamic_cast<Bullet*>(this);
				if(bp!=NULL)
				{
					Actor* app = bp->getMyOwner();
					Alien* aP = dynamic_cast<Alien*>(app);
					if(aP!=NULL)
					{
						pp->damage(2);
					}
				}//end if
			
		}
		
		/*Bullet* bp = dynamic_cast<Bullet*>(candidate);
		if(bp!=NULL && this!=bp)
		{
			//check if the bullet is not hitting with its owner or like
			Actor*ob = bp->getMyOwner();
			Alien* ab = dynamic_cast<Alien*>(ob);
			if(ab!=NULL) //means that the owner of this bullet is an alien
			{
				PlayerShip* pb = dynamic_cast<PlayerShip*>(this);
				if(pb!=NULL) //means that the player has collided into a bullet that belongs to an alien!!
				{
					this->damage(2); 
				}

				//if the caller is an alien, then we do nothing
			}
			PlayerShip* pb = dynamic_cast<PlayerShip*>(ob);
			if(pb!=NULL) //means that the owner of this bullet is the player
			{
				Alien* ab = dynamic_cast<Alien*>(this); //check if the caller of this function is an alien
				if(ab!=NULL)
				{
					this->damage(2); 
					if(ab->getHPorEnergy() <=0)
					{
						ap->setVisible(false);
						ap->getMyWorld()->decrementAliens(1);
					}
					
				}//end if

				//if the caller is the playership, we do nothing
			}//end if

		}//end if*/

	/*}//end for
	return NULL;
}*/

bool Actor::amIAlive() const
{
	return m_alive;
}

void Actor::setstatus(bool Alive)
{
	if(!Alive)
	{
		m_alive = false;
	}
}

int Actor::getTick() const
{
	return m_tick;
}

StudentWorld* Actor::getMyWorld() const
{
	return m_myWorld;
}

/*void Actor::doSomething()
{
}*/

void Actor::incrementTick(int value)
{
	m_tick += value;
}

void Actor::setTick(int value)
{
	m_tick = value;
}

//this function does not set the Actor as dead, the setStatus function does that
//so do not assume!
void Actor::damage(int loss)
{
	m_hp -= loss;
}

void Actor::resetEnergy(int gain)
{
	m_hp = gain;
}

int Actor::getHPorEnergy() const
{
	return m_hp;
}


/////////////////////////////////////////////////////////
// PlayerShip Implementation                           //
/////////////////////////////////////////////////////////

PlayerShip::PlayerShip(int xcoor, int ycoor, StudentWorld* myWorld)
	:Actor(IID_PLAYER_SHIP, xcoor, ycoor, 50, myWorld), m_shipsworld(myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	//m_energy = 50;
}

/*PlayerShip::PlayerShip(const PlayerShip& src)
	:Actor(IID_PLAYER_SHIP, src.m_xcoor, src.m_ycoor, src.m_myWorld)
{
	m_xcoor = src.m_xcoor;
	m_ycoor = src.m_ycoor;
}*/

PlayerShip::~PlayerShip()
{
}

StudentWorld* PlayerShip::getMyWorld() const
{
	return m_shipsworld;
}

void PlayerShip::doSomething()
{
	//cout << "entering player something" << endl;
	
	if(this->getHPorEnergy() <= 0)
		return;
	bool a = false;
	this->didICollide(this, getX(),getY(),6000,a);

	int keyHit = 999; //just initialized plainly
	bool isKeyHit = m_shipsworld->getKey(keyHit);

	if(isKeyHit)
	{
		if(keyHit == KEY_PRESS_LEFT && (this->getX() >0))
		{
			this->moveTo(getX()-1, getY());
		}
		else if(keyHit == KEY_PRESS_UP && (this->getY() < 39))
		{
			this->moveTo(getX(), getY()+1);
		}
		else if(keyHit == KEY_PRESS_RIGHT && (this->getX() < 29))
		{
			this->moveTo(getX()+1, getY());
		}
		else if(keyHit == KEY_PRESS_DOWN && (this->getY() > 0))
		{
			this->moveTo(getX(), getY()-1);
		}
		else if(keyHit == KEY_PRESS_SPACE && getTick()!=1)
		{
			StudentWorld* tempSWorld = getMyWorld();
			Bullet* myBullet = new Bullet(IID_BULLET,getX(),getY()+1, SHIP_OWNER, tempSWorld);
			tempSWorld->addActor(myBullet);
			tempSWorld->playSound(SOUND_PLAYER_FIRE);
			
			setTick(1);  //in order for the ship to only fire every other time
		}
		else if(keyHit == KEY_PRESS_TAB && getTick()!=1 && getMyWorld()->getTorpedoes() != 0)
		{
			StudentWorld* tempSWorld = getMyWorld();
			Torpedo * myTor = new Torpedo(IID_TORPEDO,getX(), getY()+1, SHIP_OWNER, tempSWorld);
			tempSWorld->addActor(myTor);
			tempSWorld->playSound(SOUND_PLAYER_FIRE);
			tempSWorld->decrementTorpedoes();
			setTick(1);
		}

	}// end if
	this->didICollide(this, getX(),getY(),6000,a);
	setTick(0);
}

////////////////////////////////////////////////////////////////
// Star Implementation                                        //
////////////////////////////////////////////////////////////////

Star::Star(int xcoor, int ycoor, StudentWorld* myWorld)
	:Actor(IID_STAR, xcoor, ycoor, 100, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
}

Star::~Star()
{
}

void Star::doSomething()
{
	//cout << "Entering star something" << endl;
	int curxcoor = getX();
	int curycoor = getY();

	int newycoor = curycoor -1;

	moveTo(curxcoor,newycoor);
}

////////////////////////////////////////////////////////////////////////////////
// Bullet Implementation (includes the septic bullet)                         //
////////////////////////////////////////////////////////////////////////////////

Bullet::Bullet(int ID, int xcoor, int ycoor, int myOwner, StudentWorld* myWorld)
	:Actor(ID, xcoor, ycoor, 100, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	m_myOwner = myOwner;
}

Bullet::~Bullet()
{
}

int Bullet::getMyOwner() const
{
	return m_myOwner;
}

void Bullet::doSomething()
{
	//cout << "Entering bullet something" << endl;
	//if(amIAlive())
	bool b = false;
	//cout << "Before bullet colliding" << endl;
	//this->didICollide(this,this->getX(),this->getY(),2,b);
	//cout << "Stage1." << endl;
	//cout << getMyWorld()->getnBullets() << endl;
	int a = getMyOwner();
	//cout << "Got owner" << endl;
	//cout << getMyWorld()->getMyShip()->getHPorEnergy() << endl;
	//PlayerShip* poop = dynamic_cast<PlayerShip*>(a); 
	//PlayerShip* pb = dynamic_cast<PlayerShip*>(a);//here's the trouble statement, it occasionally works, occasionally doesn't
	if(a == SHIP_OWNER)
	{
		moveTo(getX(), getY()+1);
		//bool c = false;
		//this->didICollide(this,this->getX(),this->getY(),2,c);
		//cout << "Stage1.1" << endl;
		return;
	}
	//Alien* nb = dynamic_cast<Alien*>(a);
	//cout << "cast attempt2" << endl;
	if(a == ALIEN_OWNER)
	{
		moveTo(getX(), getY()-1);
		//bool c = false;
		//this->didICollide(this,this->getX(),this->getY(),2,c);
		//cout << "Stage 1.2" << endl;
		return;
	}
}

/////////////////////////////////////////////////////////////////////
//Torpedo Implementation                                           //
/////////////////////////////////////////////////////////////////////

Torpedo::Torpedo(int ID, int xcoor, int ycoor, int myOwner, StudentWorld* myWorld)
	:Bullet(IID_TORPEDO, xcoor, ycoor, myOwner, myWorld)
{
}

Torpedo::~Torpedo()
{
}

void Torpedo::doSomething()
{
	bool b = false;
	
	int a = getMyOwner();
	
	//PlayerShip* poop = dynamic_cast<PlayerShip*>(a); 
	//PlayerShip* pb = dynamic_cast<PlayerShip*>(a);
	
	//cout << "Fire torpedo!" << endl;
	if(a==SHIP_OWNER)
	{
		moveTo(getX(), getY()+1);
		return;
	}
	//Alien* nb = dynamic_cast<Alien*>(a);
	if(a==ALIEN_OWNER)
	{
		moveTo(getX(), getY()-1);
		return;
	}
}

/////////////////////////////////////////////////////////////////////
// Alien Implementation                                            //
////////////////////////////////////////////////////////////////////

Alien::Alien(int ID, int hp, int xcoor, int ycoor, StudentWorld* myWorld)
	:Actor(ID, xcoor, ycoor, hp, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	//m_hp = hp;
	
}

Alien::~Alien()
{
}

//decrements and returns the new hp
/*int Alien::damage(int loss) 
{
	m_hp -= loss;
	return m_hp;
}

/*int Alien::getHP() const
{
	return m_hp;
}*/

/////////////////////////////////////////////////////////////////////
// Nachling Implementation                                         //
////////////////////////////////////////////////////////////////////

Nachling::Nachling(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld)
	:Alien(ID, hp, xcoor, ycoor, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	m_state = 0;
	//m_HMDr = 1; //debugging
}

Nachling::~Nachling()
{
}

int Nachling::getState() const
{
	return m_state;
}

int Nachling::getHMR() const
{
	return m_HMR;
}

int Nachling::getHMD() const
{
	return m_HMD;
}

int Nachling::getHMDr() const
{
	return m_HMDr;
}

int Nachling::getMDB() const
{
	return m_MDB;
}

void Nachling::changeState(int value)
{
	m_state = value;
}

int Nachling::computeMDB()
{
	if(this->getX() <= 14)
		return this->getX();
	else
	{
		//cout << "The X: " << getX() << endl;
		//cout << "Return: " << (29-this->getX()) << endl;
		return ((29 - this->getX()));
	}
}

void Nachling::setAllWeirdVariables(int HMR, int HMDr, int HMD, int MDB)
{
	m_HMR = HMR;
	m_HMDr = HMDr;
	m_HMD = HMD;
	m_MDB = MDB;
}

//1 is left and 2 is right
void Nachling::setHMDr(int value)
{
	m_HMDr = value;
}

void Nachling::setHMR(int value)
{
	m_HMR = value;
}

void Nachling::doSomething()
{
	//cout << "entering nach something" << endl;

	if(getTick() != 1)
	{

		/*Bullet* NmyBullet = new Bullet(this->getX(), this->getY()-1, this, getMyWorld());
					getMyWorld()->addActor(NmyBullet);
					getMyWorld()->playSound(SOUND_ENEMY_FIRE);*/

					//setTick(1);
		bool a = false;
		Actor* isShip = this->didICollide(this,this->getX(),this->getY(),10000,a);
		PlayerShip* is = dynamic_cast<PlayerShip*>(isShip);
		if(is!=NULL)
			return;
	srand(time(NULL));
		int HMD;
		int HMDr;
		int HMR;
		int MDB;

		cout << "My State: " << getState() << endl;
		if(getState() == 0)
		{
			if(this->getX() == getMyWorld()->getMyShip()->getX() && (this->getX() != 0 || getX() != VIEW_WIDTH-1))
			{
				changeState(1);
				cout << "change state!" << endl;
				MDB = computeMDB();
				//cout << "MDB: " << MDB << endl;
				if(MDB > 3)
					HMD = rand() % 3 +1; //generate a number between 1 and 3, HMD = horizontal moving distance
				else
					HMD = MDB;
				HMDr = rand() % 2 +1;  //generate a number between 0 and 1, HMDr = horizontal moving direction
				                    //where 1 is left and 2 is right
				//cout << "HMDr: " << HMDr << endl;
				HMR = HMD;
				setAllWeirdVariables(HMR, HMDr, HMD, MDB);
				/*cout << "HMR: " << getHMR() << endl;
				cout << "HMDr: " << getHMDr() << endl;
				cout << "HMD: " << getHMD() << endl;
				cout << "MDB: " << getMDB() << endl;*/
				
			}
			else
			{
				int action = rand() % 3 +1;
				if(action == 1)
				{
					if(this->getX() < getMyWorld()->getMyShip()->getX())
					{
						this->moveTo(getX()+1, getY()-1);
						setTick(1);
						return;
					}
					else if(this->getX() > getMyWorld()->getMyShip()->getX())
					{
						this->moveTo(getX()-1, getY()-1);
						setTick(1);
						return;
					}
				}//end if
			}//end else
			//move normally
			this->moveTo(getX(), getY()-1);
			setTick(1);
			return;
		}//end if

		cout << "My State: " << getState() << endl;
		if(getState() == 1)
		{
			if(getMyWorld()->getMyShip()->getY() > this->getY())
			{
				changeState(2);
				setTick(1);
				return;
			}
			//cout << "My HMR: " << getHMR() << endl;
			if(this->getHMR() == 0)
			{
				int tHMDr = getHMDr();
				//cout << "Before: " << tHMDr;
				if(tHMDr == 1)
					setHMDr(2);
				else
					setHMDr(1);
				int tempHMD = 2*getHMD();
				setHMR(tempHMD);
				//cout << "After: " << getHMDr() << endl;
				cout << "HMD: " << getHMD() << "HMR: " << getHMR() << endl;
			}
			else
			{
				int tHMR = getHMR()-1;
				setHMR(tHMR);
			}

			if(this->getHMDr() == 1) //left
			{
				this->moveTo(getX()-1, getY());
				//setHMDr(2);
			}
			else if(this->getHMDr() == 2)
			{
				this->moveTo(getX()+1, getY());
				//setHMDr(1);
			}

			//compute chance for firing
			int chancesofFiring = (10/(getMyWorld()->getRound())) +1;
			int number = rand()% (chancesofFiring-1);
			//int number = 0;
			cout << "The Number: " << number << endl;
		//force setting
			//number = 0; 
			if(number == 0)
			{
				int numberofEnemyMissiles = getMyWorld()->getnEnemyBullets();
				cout << "entering attack: " << numberofEnemyMissiles << endl;
				if(numberofEnemyMissiles < 2*getMyWorld()->getRound())
				{
					Bullet* NmyBullet = new Bullet(IID_BULLET,this->getX(), this->getY()-1, ALIEN_OWNER, getMyWorld());
					getMyWorld()->addActor(NmyBullet);
					getMyWorld()->playSound(SOUND_ENEMY_FIRE);

					setTick(1);
				}
			}

			int transitionchance = rand() % 19;
			if(transitionchance == 1)
			{
				changeState(2);
			}
			return;
		}//end elseif
		
		//cout << "My State: " << getState() << endl;
		else if(getState() == 2)
		{
			cout << "My State: " << getState() << endl;
			if(getY() == VIEW_HEIGHT-1)
			{
				changeState(0);
				setTick(1);
				return;
			}
			if(getX() == 0)
			{
				setHMDr(2);
			}
			else if (getX() == VIEW_WIDTH-1)
			{
				setHMDr(1);
			}
			else
			{
				int dir = rand()%2+1;
				if(dir == 1)
					setHMDr(1);
				else
					setHMDr(2);
			}

			int dir = getHMDr();
			if(dir == 1)
				moveTo(getX()-1, getY()+1);
			else if(dir ==2)
				moveTo(getX()+1, getY()+1);
			
			setTick(1);
			return;
			
		}//end elseif
	}//end if
	else
	{
		setTick(0);
	}
	return;
}

/*void Nachling::doSomethingShared()
{
	//cout << "entering shared" << endl;

	//cout << "Nachling tick: " << getTick() << endl;
	if(getTick() != 1)
	{

		/*Bullet* NmBullet = new Bullet(this->getX(), this->getY()-1, this, getMyWorld());
					getMyWorld()->addActor(NmyBullet);
					getMyWorld()->playSound(SOUND_ENEMY_FIRE);
		//cout << "Before Tick: " << getTick() << endl;
		setTick(1);
		//cout << "After tick: " << getTick() << endl;
		bool a = false;
		Actor* isShip = this->didICollide(this,this->getX(),this->getY(),10000,a);
		PlayerShip* is = dynamic_cast<PlayerShip*>(isShip);
		if(is!=NULL)
			return;
	//srand(time(NULL));
		int HMD;
		int HMDr;
		int HMR;
		int MDB;

		//cout << "My State: " << getState() << endl;
		if(getState() == 0)
		{
			if(this->getX() == getMyWorld()->getMyShip()->getX() && (this->getX() != 0 || getX() != VIEW_WIDTH-1))
			{
				changeState(1);
				//cout << "change state!" << endl;
				MDB = computeMDB();
				//cout << "MDB: " << MDB << endl;
				if(MDB > 3)
					HMD = rand() % 3 +1; //generate a number between 1 and 3, HMD = horizontal moving distance
				else
					HMD = MDB;
				HMDr = rand() % 2 +1;  //generate a number between 0 and 1, HMDr = horizontal moving direction
				                    //where 1 is left and 2 is right
				//cout << "HMDr: " << HMDr << endl;
				HMR = HMD;
				setAllWeirdVariables(HMR, HMDr, HMD, MDB);
				/*cout << "HMR: " << getHMR() << endl;
				cout << "HMDr: " << getHMDr() << endl;
				cout << "HMD: " << getHMD() << endl;
				cout << "MDB: " << getMDB() << endl;
				
			}
			else
			{
				int action = rand() % 3 +1;
				if(action == 1)
				{
					if(this->getX() < getMyWorld()->getMyShip()->getX())
					{
						this->moveTo(getX()+1, getY()-1);
						setTick(1);
						return;
					}
					else if(this->getX() > getMyWorld()->getMyShip()->getX())
					{
						this->moveTo(getX()-1, getY()-1);
						setTick(1);
						return;
					}
				}//end if
			}//end else
			//move normally
			this->moveTo(getX(), getY()-1);
			setTick(1);
			return;
		}//end if

		//cout << "My State: " << getState() << endl;
		if(getState() == 1)
		{
			if(getMyWorld()->getMyShip()->getY() > this->getY())
			{
				changeState(2);				
				setTick(1);
				return;
			}
			//cout << "My HMR: " << getHMR() << endl;
			if(this->getHMR() == 0)
			{
				int tHMDr = getHMDr();
				//cout << "Before: " << tHMDr;
				if(tHMDr == 1)
					setHMDr(2);
				else
					setHMDr(1);
				int tempHMD = 2*getHMD();
				setHMR(tempHMD);
				//cout << "After: " << getHMDr() << endl;
				//cout << "HMD: " << getHMD() << "HMR: " << getHMR() << endl;
			}
			else
			{
				int tHMR = getHMR()-1;
				setHMR(tHMR);
			}

			if(this->getHMDr() == 1) //left
			{
				this->moveTo(getX()-1, getY());
				//setHMDr(2);
			}
			else if(this->getHMDr() == 2)
			{
				this->moveTo(getX()+1, getY());
				//setHMDr(1);
			}

			//compute chance for firing
			int chancesofFiring = (10/(getMyWorld()->getRound())) +1;
			int number = rand()% (chancesofFiring-1);
			//int number = 0;
			cout << "The Number: " << number << endl;
		//force setting
			//number = 0; 
			if(number == 0)
			{
				int numberofEnemyMissiles = getMyWorld()->getnEnemyBullets();
				cout << "entering attack: " << numberofEnemyMissiles << endl;
				if(numberofEnemyMissiles < 2*getMyWorld()->getRound())
				{
					Bullet* NmyBullet = new Bullet(IID_BULLET,this->getX(), this->getY()-1, ALIEN_OWNER, getMyWorld());
					getMyWorld()->addActor(NmyBullet);
					getMyWorld()->playSound(SOUND_ENEMY_FIRE);

					setTick(1);
				}
			}

			int transitionchance = rand() % 19;
			if(transitionchance == 1)
			{
				changeState(2);
			}
			return;
		}//end elseif
		
		//cout << "My State: " << getState() << endl;
		else if(getState() == 2)
		{
			//cout << "My State: " << getState() << endl;
			if(getY() == VIEW_HEIGHT-1)
			{
				changeState(0);
				setTick(1);
				return;
			}
			if(getX() == 0)
			{
				setHMDr(2);
			}
			else if (getX() == VIEW_WIDTH-1)
			{
				setHMDr(1);
			}
			else
			{
				int dir = rand()%2+1;
				if(dir == 1)
					setHMDr(1);
				else
					setHMDr(2);
			}

			int dir = getHMDr();
			if(dir == 1)
			{
				bool a = false;
				moveTo(getX()-1, getY()+1);
				this->didICollide(this,this->getX(),this->getY(),6000,a);
			}
			else if(dir ==2)
			{
				bool a = false;
				moveTo(getX()+1, getY()+1);
				this->didICollide(this,this->getX(),this->getY(),6000,a);
			}
			setTick(1);
			return;
			
		}//end elseif
	}//end if

	else
	{
		//cout << "SHUCKS! NO MOVE!" << endl;
		setTick(0);
	}
	return;
}*/

////////////////////////////////////////////////////////////////
// WealthyNachling Implementation                             //
////////////////////////////////////////////////////////////////

WealthyNachling::WealthyNachling(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld)
	:Nachling(IID_WEALTHY_NACHLING,xcoor, ycoor, hp, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	m_malfunction = false;
}

WealthyNachling::~WealthyNachling()
{
}

void WealthyNachling::doSomething()
{
	//cout << "Entering wealth something" << endl;
	if(getTick()==30 && isMalfunct())
	{
		//reset the status
		setTick(0);
		//cout << "current tick: " << getTick() << endl;
		repair();
		//cout << "I have repaired." << endl;
	}
		
	if(isMalfunct())
	{
		incrementTick(1);
		//cout << "current tick: " << getTick() << endl;
		//cout << "I'm malfunct." << endl;
		bool a = false;
		this->didICollide(this, getX(), getY(), 6000, a);
		return;
	}

	int a = rand() %200;
	//cout << "The status number: " << a << endl;
	if(a == 1)
	{
		setMalfunct();
		//if(isMalfunct())
			//cout << "I'm now malfunct." << endl;
		incrementTick(1);
		//cout << "current tick: " << getTick() << endl;
		return;
	}
	Nachling::doSomething(); //operate like a Nachling
	return;
}

void WealthyNachling::setMalfunct()
{
	m_malfunction = true;
}

void WealthyNachling::repair()
{
	m_malfunction = false;
}

bool WealthyNachling::isMalfunct() const
{
	return m_malfunction;
}

///////////////////////////////////////////////////////////////////
// SmallBot Implementation                                       //
///////////////////////////////////////////////////////////////////

SmallBot::SmallBot(int ID, int xcoor, int ycoor, int hp, StudentWorld* myWorld)
	:Alien(IID_SMALLBOT, hp, xcoor, ycoor, myWorld)
{
	m_xcoor = xcoor;
	m_ycoor = ycoor;
	m_justHit = false;
}

SmallBot::~SmallBot()
{
}

void SmallBot::setIsHit(bool hit)
{
	m_justHit = hit;
}

bool SmallBot::isJustHit() const
{
	return m_justHit;
}

void SmallBot::doSomething()
{
	if(getTick()!=1)
	{
		setTick(1);
		bool c = false;
		this->didICollide(this,getX(),getY(),6000, c);
		int x;
		if(isJustHit())
		{
			if(this->getX() == 0)
			{
				x = 1;
			}
			else if(this->getX() == VIEW_WIDTH-1)
			{
				x = 28;
			}
			else
			{
				int dirdecision = rand()%2; //where 0 is left and 1 is right
				if(dirdecision==0)
				{
					x = getX()-1;
				}
				else
					x = getX()+1;
			}
			this->moveTo(x, getY()-1);
			bool a = false;
			this->didICollide(this,getX(),getY(),6000, a);
			this->setIsHit(false);
		}//end if
		else
		{
			this->moveTo(getX(), getY()-1);
		}
		int q = 100/(this->getMyWorld()->getRound());
		int decision = rand()%q;
		if(q==1) //fires torpedo
		{
			Torpedo* newtor = new Torpedo(IID_TORPEDO, this->getX(), this->getY()-1, ALIEN_OWNER, this->getMyWorld());
		}
		else
		{
			if(this->getX() == (this->getMyWorld()->getMyShip()->getX()))
			{
			
				int alliedprojectiles = this->getMyWorld()->getnEnemyBullets();
				if(alliedprojectiles < (2*this->getMyWorld()->getRound()))
				{
					Bullet* myBullet = new Bullet(IID_BULLET,this->getX(), this->getY()-1, ALIEN_OWNER, getMyWorld());
					getMyWorld()->addActor(myBullet);
					getMyWorld()->playSound(SOUND_ENEMY_FIRE);
				}
			}
		}

	}//end if
	else
	{
		setTick(0);
	}
}

////////////////////////////////////////////////////////
// Goodie Implementation                              //
////////////////////////////////////////////////////////

Goodie::Goodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld)
	:Actor(ID, xcoor, ycoor, 1, myWorld)
{
	m_life = ((100/myWorld->getRound())+30);
	m_localTick = 0;
}

Goodie::~Goodie()
{
}

void Goodie::resetLocalTick()
{
	m_localTick = 0;
}

int Goodie::getLifeTime() const
{
	return m_life;
}

//this function determines if the Goodie should move or not
//The Goodie would move when m_localTick = 2
int Goodie::getLocalTick() const
{
	return m_localTick;
}

void Goodie::incrementLocalTick() 
{
	m_localTick++;
}

void Goodie::doSomething()
{
}

void Goodie::doSomethingGShared()
{
}

////////////////////////////////////////////////////
// EnergyGoodie Implementation                    //
////////////////////////////////////////////////////

EnergyGoodie::EnergyGoodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld)
	:Goodie(IID_ENERGY_GOODIE, xcoor, ycoor, myWorld)
{
}

EnergyGoodie::~EnergyGoodie()
{
}

void EnergyGoodie::doSomething()
{
	ofstream myfile;
	myfile.open("test.txt");
	myfile << "Entering goodie something" << endl;
	bool a = false;
	this->didICollide(this, this->getX(), this->getY(), 0, a);
	if(a)
	{
		myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		getMyWorld()->increaseScore(5000);
		getMyWorld()->playSound(SOUND_GOT_GOODIE);
		getMyWorld()->getMyShip()->resetEnergy(50);
		this->setstatus(false);
		return;
	}
	
	else
	{
		myfile << "Local tick: " << getLocalTick() << endl;
		if(getLocalTick() == 2) //then move
		{
			resetLocalTick();
			//calculate the brightness
			int ticksLeft = this->getLifeTime() - getTick();
			myfile << "Ticks Left: " << ticksLeft << endl;
			myfile << "Lifetime: " << getLifeTime() << endl;
			double bright = (ticksLeft/getLifeTime()) + 0.2;
			this->setBrightness(bright);
			if(getTick() == getLifeTime())
			{
				this->setstatus(false);
				return;
			}

			//move down
			moveTo(getX(), getY()-1);

			bool c = false;
			this->didICollide(this, this->getX(), this->getY(), 0, c);
			if(c)
			{
				myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				getMyWorld()->increaseScore(5000);
				getMyWorld()->getMyShip()->resetEnergy(50);
				return;
			}
		}//end if
		incrementTick(1);
		incrementLocalTick();
	}//end else

}

/////////////////////////////////////////////////////////////////////
// TorpedoGoodie Implemenation                                     //
/////////////////////////////////////////////////////////////////////

TorpedoGoodie::TorpedoGoodie(int ID, int xcoor, int ycoor, StudentWorld* myWorld)
	:Goodie(IID_TORPEDO_GOODIE, xcoor, ycoor, myWorld)
{
}

TorpedoGoodie::~TorpedoGoodie()
{
}

void TorpedoGoodie::doSomething()
{
	ofstream myfile;
	myfile.open("torpedo.txt");
	myfile << "Entering goodie something" << endl;
	bool a = false;
	this->didICollide(this, this->getX(), this->getY(), 0, a);
	if(a)
	{
		myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		getMyWorld()->increaseScore(5000);
		getMyWorld()->playSound(SOUND_GOT_GOODIE);
		getMyWorld()->incrementTorpedoes(5);
		this->setstatus(false);
		return;
	}
	
	else
	{
		myfile << "Local tick: " << getLocalTick() << endl;
		myfile << "Lifetime: " << getLifeTime() << endl;
		if(getLocalTick() == 2) //then move
		{
			resetLocalTick();
			//calculate the brightness
			int ticksLeft = this->getLifeTime() - getTick();
			myfile << "Ticks left: " << ticksLeft << endl;
			double bright = (ticksLeft/getLifeTime()) + 0.2;
			this->setBrightness(bright);
			if(getTick() == getLifeTime())
			{
				this->setstatus(false);
				return;
			}

			//move down
			moveTo(getX(), getY()-1);

			bool c = false;
			this->didICollide(this, this->getX(), this->getY(), 0, c);
			if(c)
			{
				myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				getMyWorld()->increaseScore(5000);
				getMyWorld()->incrementTorpedoes(5);
				return;
			}
		}//end if
		incrementTick(1);
		incrementLocalTick();
	}//end else
}

///////////////////////////////////////////////////////////////////
// FreeShip Implementation                                       //
///////////////////////////////////////////////////////////////////

FreeShip::FreeShip(int ID, int xcoor, int ycoor, StudentWorld* myWorld)
	:Goodie(ID, xcoor, ycoor, myWorld)
{
}

FreeShip::~FreeShip()
{
}

void FreeShip::doSomething()
{
	ofstream myfile;
	myfile.open("freeship.txt");

	myfile << "Entering goodie something" << endl;
	bool a = false;
	this->didICollide(this, this->getX(), this->getY(), 0, a);
	if(a)
	{
		myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		getMyWorld()->increaseScore(5000);
		getMyWorld()->playSound(SOUND_GOT_GOODIE);
		getMyWorld()->incLives();
		this->setstatus(false);
		return;
	}
	
	else
	{
		myfile << "Local tick: " << getLocalTick() << endl;
		if(getLocalTick() == 2) //then move
		{
			resetLocalTick();
			//calculate the brightness
			int ticksLeft = this->getLifeTime() - getTick();
			myfile << "Ticks Left: " << ticksLeft << endl;
			myfile << "Lifetime: " << getLifeTime() << endl;
			double bright = (ticksLeft/getLifeTime()) + 0.2;
			this->setBrightness(bright);
			if(getTick() == getLifeTime())
			{
				this->setstatus(false);
				return;
			}

			//move down
			moveTo(getX(), getY()-1);

			bool c = false;
			this->didICollide(this, this->getX(), this->getY(), 0, c);
			if(c)
			{
				myfile << "FOUND IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				getMyWorld()->increaseScore(5000);
				getMyWorld()->incLives();
				return;
			}
		}//end if
		incrementTick(1);
		incrementLocalTick();
	}//end else
}