#pragma once
#include "GameObject.h"

class Customer : public CGameObject
{
public:
	enum STATES
	{
		s_Queing,
		s_follow,
		s_SitDown,
		s_ReadyToOrder,
		s_Ordering,
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

	void setToFollow();
	void setToSitDown();
	void setToOrdering();
	void setToWaitForFood();
	void setToEating();
	void cleanUpWater();

	STATES state;
	float timmer, timmer2;
	bool spillWater;
	bool rngAlready;
};