#include "Customer.h"

Customer::Customer()
{
	type = CUSTOMER;
	active = true;
	state = s_Queing;
	timmer = timmer2 = -1;
	spillWater = false;
	rngAlready = false;
}


Customer::~Customer()
{
}

std::string Customer::renderState()
{
	switch (state)
	{
	case Customer::s_Queing:
		return "Queing";
		break;
	case Customer::s_follow:
		return "Following";
		break;
	case Customer::s_SitDown:
		return "Sitted";
		break;
	case Customer::s_ReadyToOrder:
		return "Ready To Order";
		break;
	case Customer::s_WaitForFood:
		return "Waiting for Food";
		break;
	case Customer::s_Eating:
		return "Eating";
		break;
	case Customer::s_Leave:
		return "Leaving";
		break;
	default:
		break;
	}

	return "";
}

void Customer::update(double dt/* waiter obj refrence */)
{
	switch (state)
	{
	case(s_SitDown) :
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(5, 20);
		}
		else
		{
			timmer -= dt;
			if (timmer < 0)
			{
				timmer = -1;
				state = s_ReadyToOrder;
			}
		}
		break;

	case(s_ReadyToOrder) :
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(15, 25);
		}
		else
		{
			timmer -= dt;
			if (timmer < 0)
			{
				timmer = -1;
				state = s_Leave;
			}
		}
		break;
	case(s_Eating) :
		if (!spillWater)
		{
			if (timmer == -1)
			{
				timmer = Math::RandFloatMinMax(10, 15);
				timmer2 = Math::RandFloatMinMax(3, 8);
			}
			else
			{
				timmer -= dt;
				if (!rngAlready && timmer < timmer2)
				{
					if (Math::RandIntMinMax(1, 100) < 30)
					{
						spillWater = true;
					}
					rngAlready = true;
				}
				if (timmer < 0)
				{
					timmer = -1;
					state = s_Leave;
				}
			}
		}
		break;
	}
}

bool Customer::isQueing()
{
	if (state == s_Queing)
		return true;
	return false;
}

bool Customer::isFollowing()
{
	if (state == s_follow)
		return true;
	return false;
}

bool Customer::isReadyToOrder()
{
	if (state == s_ReadyToOrder)
		return true;
	return false;
}

bool Customer::isWaitingForFood()
{
	if (state == s_WaitForFood)
		return true;
	return false;
}

bool Customer::isLeaving()
{
	if (state == s_Leave)
		return true;
	return false;
}

bool Customer::isEating()
{
	if (state == s_Eating)
		return true;
	return false;
}

bool Customer::isSpillWater()
{
	return spillWater;
}


void Customer::setToFollow()
{
	state = s_follow;
}

void Customer::setToSitDown()
{
	state = s_SitDown;
}

void Customer::setToOrdering()
{
	timmer = -1;
	state = s_Ordering;
}

void Customer::setToWaitForFood()
{
	timmer = -1;
	state = s_WaitForFood;
}

void Customer::setToEating()
{
	timmer = -1;
	state = s_Eating;
}

void Customer::cleanUpWater()
{
	spillWater = false;
}