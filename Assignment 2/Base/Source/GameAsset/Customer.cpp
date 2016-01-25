#include "Customer.h"


Customer::Customer()
{
	active = false;
	currentState = s_AtShop;
	//currentState = s_Queing;
	//previouState = s_Idle;
	timer = 0;
	timerLimit = -1;
	spillWater = false;
	rngAlready = false;
	id = 0;

	color[0] = Color(0.2, 0.4, 0.5);
	color[1] = Color(1, 0.2, 0.6);
	color[2] = Color(0.6, 0.7, 1);
	color[3] = Color(1, 0.4, 1);
}


Customer::~Customer()
{
}

std::string Customer::renderState()
{
	if (active)
	{
		switch (currentState)
		{
		case s_AtShop:
			return "Arrival";
			break;

		case s_BrowseProduct:
			return "Browsing";
			break;

		case s_Paying:
			return "Paying";
			break;

		case s_Exit:
			return "Exiting";
			break;

		default:
			break;
		}
	}

	return "Inactive";
}

void Customer::update(double dt)
{
	/*switch (currentState)
	{
	case s_Queing:
		if (timerLimit == -1)
		{
			startTimer(30, 60);
		}
		else
		{
			if (timerUpdate(dt))
			{
				previouState = currentState;
				setToLeaving();
				stopTimer();
			}
		}
		break;

	case s_SitDown:
		if (timerLimit == -1)
		{
			startTimer(10, 20);
		}
		else
		{
			if (timerUpdate(dt))
			{
				setToReadyToOrder();
				stopTimer();
			}
		}
		break;

	case s_ReadyToOrder:
		if (timerLimit == -1)
		{
			startTimer(10, 20);
		}
		else
		{
			if (timerUpdate(dt))
			{
				previouState = currentState;
				setToLeaving();
				stopTimer();
			}
		}
		break;

	case s_Eating:
		if (timerLimit == -1)
		{
			startTimer(10, 20);
		}
		else
		{
			if (timerUpdate(dt))
			{
				previouState = currentState;
				setToLeaving();
				stopTimer();
			}
		}
	}*/
}

//bool Customer::isQueing()
//{
//	if (currentState == s_Queing)
//		return true;
//	return false;
//}
//
//bool Customer::isFollowing()
//{
//	if (currentState == s_follow)
//		return true;
//	return false;
//}
//
//bool Customer::isReadyToOrder()
//{
//	if (currentState == s_ReadyToOrder)
//		return true;
//	return false;
//}
//
//bool Customer::isWaitingForFood()
//{
//	if (currentState == s_WaitForFood)
//		return true;
//	return false;
//}
//
//bool Customer::isLeaving()
//{
//	if (currentState == s_Leave)
//		return true;
//	return false;
//}
//
//bool Customer::isEating()
//{
//	if (currentState == s_Eating)
//		return true;
//	return false;
//}
//
//bool Customer::isSpillWater()
//{
//	return spillWater;
//}
//
//void Customer::setToQueueing()
//{
//	currentState = s_Queing;
//	previouState = s_Idle;
//}
//
//void Customer::setToFollow()
//{
//	currentState = s_follow;
//}
//
//void Customer::setToSitDown()
//{
//	currentState = s_SitDown;
//}
//
//void Customer::setToReadyToOrder()
//{
//	currentState = s_ReadyToOrder;
//}
//
//void Customer::setToOrdered()
//{
//	currentState = s_Ordered;
//}
//
//void Customer::setToWaitForFood()
//{
//	currentState = s_WaitForFood;
//}
//
//void Customer::setToEating()
//{
//	currentState = s_Eating;
//}
//
//void Customer::setToLeaving()
//{
//	currentState = s_Leave;
//}
//
//void Customer::cleanUpWater()
//{
//	spillWater = false;
//}
bool Customer::isAtShop()
{
	if (currentState == s_AtShop)
		return true;
	return false;
}

bool Customer::isBrowsing()
{
	if (currentState == s_BrowseProduct)
		return true;
	return false;
}

bool Customer::isPaying()
{
	if (currentState == s_Paying)
		return true;
	return false;
}

bool Customer::isExiting()
{
	if (currentState == s_Exit)
		return true;
	return false;
}

void Customer::setToAtShop()
{
	currentState = s_AtShop;
}

void Customer::setToBrowsing()
{
	currentState = s_BrowseProduct;
}

void Customer::setToPaying()
{
	currentState = s_Paying;
}

void Customer::setToExit()
{
	currentState = s_Exit;
}

void Customer::startTimer(float min, float max)
{
	timerLimit = Math::RandFloatMinMax(min, max);
	timer = 0;
}

void Customer::stopTimer()
{
	timer = 0;
	timerLimit = -1;
}

bool Customer::timerUpdate(const double dt)
{
	timer += dt;
	if (timer > timerLimit)
	{
		return true;
	}
	return false;
}