#include "Waitress.h"

Vector3 Waitress::queuePos = Vector3(66, 170, 0);


Waitress::Waitress()
{
	type = WAITRESS;
	currentState = s_Idle;
	previousState = s_Idle;
	usheringCustomer = false;
	CH = NULL;
}


Waitress::~Waitress()
{
}

void Waitress::update(const double & dt)
{
	//Check if the Waitress is currently idling
	if (currentState == s_Idle)
	{
		switch(previousState)
		{
		case(s_Usher):
			//if(customerOrder = true)
			//	currentState = s_TakeOrder;
			//else
			//	previousState = s_TakeOrder;
			break;

		case(s_TakeOrder):
			/*if(
			break;*/

		default:
			break;
		}

	}
	else
	{
		switch (currentState)
		{
		case(s_Usher) :
			//Usher customer to their seat

			//if(table == false || customer = false)
			{
				previousState = currentState;
				currentState = s_Idle;
			}
			break;

		case(s_TakeOrder):
			//Take order from customer
			break;

		case(s_PlaceOrder) :
			//Place order with chef
			break;

		case(s_TakeFood) :
			//Take order from chef
			break;

		case(s_DeliverFood) :
			//Delivery food to customer
			break;

		case(s_CleanTable) :
			//Clean table after customer is done
			break;
		}
	}
}

bool Waitress::isIdle()
{
	if (currentState == s_Idle)
		return true;
	return false;
}

bool Waitress::isUsher()
{
	if (currentState == s_Usher)
		return true;
	return false;
}

bool Waitress::isTakingOrder()
{
	if (currentState == s_TakeOrder)
		return true;
	return false;
}

bool Waitress::isTakingFood()
{
	if (currentState == s_TakeFood)
		return true;
	return false;
}

bool Waitress::isDeliveringFood()
{
	if (currentState == s_DeliverFood)
		return true;
	return false;
}

bool Waitress::isCleaningTable()
{
	if (currentState == s_CleanTable)
		return true;
	return false;
}

bool Waitress::isCleaningFloor()
{
	if (currentState == s_CleanFloor)
		return true;
	return false;
}

void Waitress::setToUsher()
{
	currentState = s_Usher;
}