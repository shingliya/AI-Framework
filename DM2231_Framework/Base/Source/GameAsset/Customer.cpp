#include "Customer.h"

Customer::Customer()
{
	type = CUSTOMER;
	active = true;
	state = s_Queing;
	timmer = -1;
}


Customer::~Customer()
{
}

void Customer::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* Customer::getMesh()
{
	return mesh;
}

void Customer::update(double dt/* waiter obj refrence */)
{
	switch (state)
	{
	case(s_Queing) :
		//if (waiter position at que waypoint && waiter state == usher)
		//{
		//	follow waiter
		//}
		//if (position == one of the tables)
		//{
		//	state = s_SitDown;
		//}
		break;

	case(s_SitDown) :
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(5, 10);
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
			timmer = Math::RandFloatMinMax(5, 15);
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
		//if (waiter position at custormer table waypoint && waiter state == TakeOrder)
		//{
		//	state = s_Ordering;
		//	timmer = -1;
		//}
		break;

	case(s_Ordering) :
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
				state = s_WaitForFood;
			}
		}
		break;
	case(s_WaitForFood) :
		//if (waiter position at custormer table waypoint && waiter state == DeliverFood)
		//{
		//	state = s_Eating;
		//}
		break;
	case(s_Eating) :
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(10, 15);
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
	case(s_Leave) :
		//move to exit

		//if (reach exit)
		//{
		//	active = false;
		//}
		break;
	}
}