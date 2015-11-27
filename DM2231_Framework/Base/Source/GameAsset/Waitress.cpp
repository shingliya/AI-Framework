#include "Waitress.h"



Waitress::Waitress()
{
	currentState = s_Idle;
	previousState = s_Idle;
}


Waitress::~Waitress()
{
}

void Waitress::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* Waitress::getMesh()
{
	return mesh;
}

void Waitress::update(const double & dt)
{
	if (currentState == previousState == s_Idle)
	{
		//Check whether there is customer
		//if(customer == true && customer == true)
		{
			currentState = s_Usher;
		}
	}

	switch (currentState)
	{
	case(s_Usher) :
		//Usher customer to their seat
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