#include "Cashier.h"

Vector3 Cashier::CounterPos = Vector3(855, 483, 2);
Vector3 Cashier::DoorPos = Vector3(898, -30, 2);

Cashier::Cashier()
{
	active = true;
	currentState = s_IDLE;
	previousState = s_IDLE;
}


Cashier::~Cashier()
{
}

std::string Cashier::renderState()
{
	switch (currentState)
	{
	case(s_IDLE) :
		return "Idle";
		break;

	case(s_CHECKFORBREAD) :
		return "Checking For Bread";
		break;

	case(s_RESTOCK) :
		return "Restocking";
		break;

	case(s_SETTLEPURCHASE) :
		return "Settling Purchase";
		break;

	case(s_SHOPPING) :
		return "Shopping";
		break;


	default:
		break;
	}
}

std::string Cashier::renderState2()
{
	switch (previousState)
	{
	case(s_IDLE) :
		return "Idle";
		break;

	case(s_CHECKFORBREAD) :
		return "Checking For Bread";
		break;

	case(s_RESTOCK) :
		return "Restocking";
		break;

	case(s_SETTLEPURCHASE) :
		return "Settling Purchase";
		break;

	case(s_SHOPPING) :
		return "Shopping";
		break;


	default:
		break;
	}
}

bool Cashier::isIdle()
{
	if (currentState == s_IDLE)
		return true;
	return false;
}

bool Cashier::isCheckingForBread()
{
	if (currentState == s_CHECKFORBREAD)
		return true;
	return false;
}

bool Cashier::isRestocking()
{
	if (currentState == s_RESTOCK)
		return true;
	return false;
}

bool Cashier::isSettlingPurchase()
{
	if (currentState == s_SETTLEPURCHASE)
		return true;
	return false;
}

bool Cashier::isShopping()
{
	if (currentState == s_SHOPPING)
		return true;
	return false;
}

void Cashier::setToIdle()
{
	currentState = s_IDLE;
}

void Cashier::setToCheckForBread()
{
	currentState = s_CHECKFORBREAD;
}

void Cashier::setToRestocking()
{
	currentState = s_RESTOCK;
}

void Cashier::setToSettlePurchase()
{
	currentState = s_SETTLEPURCHASE;
}

void Cashier::setToShopping()
{
	currentState = s_SHOPPING;
}