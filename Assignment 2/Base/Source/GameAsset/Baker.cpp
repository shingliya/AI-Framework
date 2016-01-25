#include "Baker.h"

Vector3 Baker::initPos = Vector3(300, 550, 2);
Vector3 Baker::stovePos = Vector3(1155, 669, 2);
Vector3 Baker::fridgePos = Vector3(1155, 571, 2);
Vector3 Baker::tablePos = Vector3(626, 669, 2);

Baker::Baker()
{
	state = s_Baking;
	startToCook = true;
	currentTime = timeLimit = 0.f;
}


Baker::~Baker()
{
}


void Baker::update(double dt)
{
	if (isBaking())
	{
		if (timeLimit == 0)
		{
			startTimer(5, 10);
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

//bool Baker::isIdle()
//{
//	if (state == s_Idle)
//		return true;
//	return false;
//}
//
//bool Baker::isCooking()
//{
//	if (state == s_Cook)
//		return true;
//	return false;
//}
//
//bool Baker::isPlacingFood()
//{
//	if (state == s_placeFood)
//		return true;
//	return false;
//}
//
//void Baker::setToIdle()
//{
//	state = s_Idle;
//}
//
//void Baker::setToCooking()
//{
//	state = s_Cook;
//}
//
//void Baker::setToPlacingFood()
//{
//	state = s_placeFood;
//}

bool Baker::isBaking()
{
	if (state == s_Baking)
		return true;
	return false;
}

bool Baker::isPlacingFood()
{
	if (state == s_PlaceFoodAtTable)
		return true;
	return false;
}

bool Baker::isRestocking()
{
	if (state == s_RestockMaterial)
		return true;
	return false;
}

bool Baker::isOutOfMaterial()
{
	if (state == s_OutOfMaterial)
		return true;
	return false;
}

bool Baker::isCashier()
{
	if (state == s_Cashier)
		return true;
	return false;
}

void Baker::setToBaking()
{
	state = s_Baking;
}

void Baker::setToPlacingFood()
{
	state = s_PlaceFoodAtTable;
}

void Baker::setToRestock()
{
	state = s_RestockMaterial;
}

void Baker::setToInformCashier()
{
	state = s_OutOfMaterial;
}

void Baker::setToCashier()
{
	state = s_Cashier;
}

std::string Baker::renderState()
{
	switch (state)
	{
	case Baker::s_Baking:
		return "Baking";
		break;

	case Baker::s_PlaceFoodAtTable:
		return "Placing Food";
		break;

	case Baker::s_RestockMaterial:
		return "Restocking";
		break;

	case Baker::s_OutOfMaterial:
		return "Inform ing Cashier";
		break;

	case Baker::s_Cashier:
		return "Cashier";
		break;

	default:
		break;
	}

	return "";
}

void Baker::startTimer(float min, float max)
{
	//timerLimit = Math::RandFloatMinMax(min, max);
	//timer = 0;
	timeLimit = Math::RandFloatMinMax(min, max);
	currentTime = 0;
}

void Baker::stopTimer()
{
	//timer = 0;
	//timerLimit = -1;
	currentTime = timeLimit = 0;
}

bool Baker::timerUpdate(const double dt)
{
	currentTime += dt;
	if (currentTime > timeLimit)
		return true;

	return false;
}