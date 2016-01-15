#pragma once
#include "GameObject.h"

class Customer : public CGameObject
{
public:
	enum STATES
	{
		s_Idle,
		s_Queing,
		s_follow,
		s_SitDown,
		s_ReadyToOrder,
		s_Ordered,
		s_WaitForFood,
		s_Eating,
		s_Leave,
	};

	Customer();
	~Customer();

	std::string renderState();
	void update(double dt/* waiter obj refrence */);

	bool isQueing();
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
	void cleanUpWater();

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
};