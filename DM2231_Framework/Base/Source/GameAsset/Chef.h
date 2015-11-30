#pragma once
#include "GameObject.h"

class Chef : public CGameObject
{
public:
	enum STATES
	{
		s_Idle,
		s_Cook,
		s_placeFood,
	};
	Chef();
	~Chef();

	void update(double dt);

	STATES state;
	float timmer;
	int numOfOrder, numOfCookedFood;
};

