#include "Customer.h"

Customer::Customer()
{
	type = CUSTOMER;
	active = true;
	currentState = s_Queing;
	previouState = s_Idle;
	timer = 0;
	timerLimit = -1;
	spillWater = false;
	rngAlready = false;
	id = 0;
}


Customer::~Customer()
{
}

std::string Customer::renderState()
{
	switch (currentState)
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
	case Customer::s_Ordered:
		return "Ordered";
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

void Customer::update(double dt)
{
	switch (currentState)
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

	//case s_Ordering:
	//	if (timerLimit == -1)
	//	{
	//		startTimer(10, 20);
	//	}
	//	else
	//	{
	//		if(timerUpdate(dt))
	//		{

	//		}
	//	}
	//	break;
		/*case(s_SitDown) :
			if (timer == -1)
			{
				timer = Math::RandFloatMinMax(5, 20);
			}
			else
			{
				timer -= dt;
				if (timer < 0)
				{
					timer = -1;
					currentState = s_ReadyToOrder;
				}
			}
			break;

		case(s_ReadyToOrder) :
			if (timer == -1)
			{
				timer = Math::RandFloatMinMax(15, 25);
			}
			else
			{
				timer -= dt;
				if (timer < 0)
				{
					timer = -1;
					currentState = s_Leave;
				}
			}
			break;
		case(s_Eating) :
			if (!spillWater)
			{
				if (timer == -1)
				{
					timer = Math::RandFloatMinMax(10, 15);
					timer2 = Math::RandFloatMinMax(3, 8);
				}
				else
				{
					timer -= dt;
					if (!rngAlready && timer < timer2)
					{
						if (Math::RandIntMinMax(1, 100) < 30)
						{
							spillWater = true;
						}
						rngAlready = true;
					}
					if (timer < 0)
					{
						timer = -1;
						currentState = s_Leave;
					}
				}
			}
			break;*/
	}
}

bool Customer::isQueing()
{
	if (currentState == s_Queing)
		return true;
	return false;
}

bool Customer::isFollowing()
{
	if (currentState == s_follow)
		return true;
	return false;
}

bool Customer::isReadyToOrder()
{
	if (currentState == s_ReadyToOrder)
		return true;
	return false;
}

bool Customer::isWaitingForFood()
{
	if (currentState == s_WaitForFood)
		return true;
	return false;
}

bool Customer::isLeaving()
{
	if (currentState == s_Leave)
		return true;
	return false;
}

bool Customer::isEating()
{
	if (currentState == s_Eating)
		return true;
	return false;
}

bool Customer::isSpillWater()
{
	return spillWater;
}

void Customer::setToQueueing()
{
	currentState = s_Queing;
	previouState = s_Idle;
}

void Customer::setToFollow()
{
	currentState = s_follow;
}

void Customer::setToSitDown()
{
	currentState = s_SitDown;
}

void Customer::setToReadyToOrder()
{
	currentState = s_ReadyToOrder;
}

void Customer::setToOrdered()
{
	currentState = s_Ordered;
}

void Customer::setToWaitForFood()
{
	currentState = s_WaitForFood;
}

void Customer::setToEating()
{
	currentState = s_Eating;
}

void Customer::setToLeaving()
{
	currentState = s_Leave;
}

void Customer::cleanUpWater()
{
	spillWater = false;
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