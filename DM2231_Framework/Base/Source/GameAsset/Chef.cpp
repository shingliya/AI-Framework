#include "Chef.h"



Chef::Chef()
{
	type = CHEF;
	state = s_Idle;
	timmer = -1;
	for (int i = 0; i < MAX_COSTOMER_COUNT; ++i)
	{
		orderList[i] = false;
		cookedFoodList[i] = false;
	}
	currentCustomerOrder = -1;
}


Chef::~Chef()
{
}


void Chef::update(double dt)
{
	switch (state)
	{
	case s_Idle:
		for (int i = 0; i < MAX_COSTOMER_COUNT; ++i)
		{
			if (orderList[i] == true)
			{
				currentCustomerOrder = i;
				state = s_Cook;
				pos.Set(725, 550, 0);
				break;
			}
		}
		break;
	case s_Cook:
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(5, 15);
		}
		else
		{
			timmer -= dt;
			if (timmer < 0)
			{
				timmer = -1;
				state = s_placeFood;
			}
		}
		break;
	case s_placeFood:
		//move to waypoint and place food
		//if (reach waypoint)
		{
			orderList[currentCustomerOrder] = false;
			cookedFoodList[currentCustomerOrder] = true;
			currentCustomerOrder = -1;
			pos.Set(525, 550, 0);
			state = s_Idle;
		}
		break;
	}
}

void Chef::passOrder(bool order[], unsigned sizeOfArray)
{
	memcpy(orderList, order, sizeof(bool)*sizeOfArray);
}