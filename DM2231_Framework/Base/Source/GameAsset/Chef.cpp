#include "Chef.h"

Vector3 Chef::initPos = Vector3(300, 550, 2);
Vector3 Chef::stovePos = Vector3(500, 550, 2);

Chef::Chef()
{
	type = CHEF;
	state = s_Idle;
	timmer = -1;
	timer = 0;
	timerLimit = -1;
	startToCook = false;
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
	//switch (state)
	//{
	//case s_Idle:
	//	for (int i = 0; i < MAX_COSTOMER_COUNT; ++i)
	//	{
	//		if (orderList[i] == true)
	//		{
	//			currentCustomerOrder = i;
	//			state = s_Cook;
	//			pos.Set(500, 550, 2);
	//			break;
	//		}
	//	}
	//	break;
	//case s_Cook:
	//	if (timmer == -1)
	//	{
	//		timmer = Math::RandFloatMinMax(5, 15);
	//	}
	//	else
	//	{
	//		timmer -= dt;
	//		if (timmer < 0)
	//		{
	//			timmer = -1;
	//			state = s_placeFood;
	//		}
	//	}
	//	break;
	//case s_placeFood:
	//	//move to waypoint and place food
	//	//if (reach waypoint)
	//	{
	//		orderList[currentCustomerOrder] = false;
	//		cookedFoodList[currentCustomerOrder] = true;
	//		currentCustomerOrder = -1;
	//		pos.Set(300, 550, 2);
	//		state = s_Idle;
	//	}
	//	break;
	//}
	if (isCooking())
	{
		if (timerLimit == -1)
		{
			startTimer(3, 5);
		}
		else
		{
			if (timerUpdate(dt))
			{
				setToPlacingFood();
				stopTimer();
			}
		}
	}
}

void Chef::passOrder(bool order[], unsigned sizeOfArray)
{
	memcpy(orderList, order, sizeof(bool)*sizeOfArray);
}

bool Chef::isIdle()
{
	if (state == s_Idle)
		return true;
	return false;
}

bool Chef::isCooking()
{
	if (state == s_Cook)
		return true;
	return false;
}

bool Chef::isPlacingFood()
{
	if (state == s_placeFood)
		return true;
	return false;
}

void Chef::setToIdle()
{
	state = s_Idle;
}

void Chef::setToCooking()
{
	state = s_Cook;
}

void Chef::setToPlacingFood()
{
	state = s_placeFood;
}

std::string Chef::renderState()
{
	switch (state)
	{
	case Chef::s_Idle:
		return "Idle";
		break;
	case Chef::s_Cook:
		return "Cooking";
		break;
	case Chef::s_placeFood:
		return "Placing Food";
		break;

	default:
		break;
	}

	return "";
}

void Chef::startTimer(float min, float max)
{
	timerLimit = Math::RandFloatMinMax(min, max);
	timer = 0;
}

void Chef::stopTimer()
{
	timer = 0;
	timerLimit = -1;
}

bool Chef::timerUpdate(const double dt)
{
	timer += dt;
	if (timer > timerLimit)
	{
		return true;
	}
	return false;
}