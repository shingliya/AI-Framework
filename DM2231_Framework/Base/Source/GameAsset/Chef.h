#pragma once
#include "GameObject.h"

const int MAX_COSTOMER_COUNT = 6;

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
	void passOrder(bool order[], unsigned sizeOfArray);
	bool takeFood(int slot);

	STATES state;
	float timmer;
	bool orderList[MAX_COSTOMER_COUNT], cookedFoodList[MAX_COSTOMER_COUNT];
	int currentCustomerOrder;
	int numOfReadyFood;
};