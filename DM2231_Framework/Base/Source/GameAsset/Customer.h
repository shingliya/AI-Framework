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

	void update(double dt/* waiter obj refrence */);

	bool isQueing();
	bool isFollowing();

	void setToFollow();
	void setToSitDown();

	STATES state;
	float timmer;
	int num;
};