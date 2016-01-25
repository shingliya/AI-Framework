#pragma once
#include "GameObject.h"

class Customer : public CGameObject
{
public:
	enum STATES
	{
		s_AtShop,
		s_BrowseProduct,
		s_Paying,
		s_Exit
	};

	Customer();
	~Customer();

	std::string renderState();
	void update(double dt/* waiter obj refrence */);

	bool isAtShop();
	bool isBrowsing();
	bool isPaying();
	bool isExiting();

	void setToAtShop();
	void setToBrowsing();
	void setToPaying();
	void setToExit();

	/*bool isQueing();
	bool isFollowing();
	bool isReadyToOrder();
	bool isWaitingForFood();
	bool isEating();
	bool isLeaving();
	bool isSpillWater();

	void setToQueueing();
	void setToFollow();
	void setToSitDown();
	void setToReadyToOrder();
	void setToOrdered();
	void setToWaitForFood();
	void setToEating();
	void setToLeaving();
	void cleanUpWater();*/

	void startTimer(float min, float max);
	void stopTimer();
	bool timerUpdate(const double dt);

	STATES currentState;
	STATES previouState;
	float timer, timer2;
	float timerLimit;
	bool spillWater;
	bool rngAlready;
	int id;
	Color color[4];
};