#include "Customer.h"

Customer::Customer()
{
	type = CUSTOMER;
	active = true;
	state = s_Queing;
	timmer = -1;
	setMesh2();
}


Customer::~Customer()
{
}

void Customer::setMesh2()
{
	head = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Head.obj");
	torso = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Torso.obj");
	leftArm = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Arm.obj");
	rightArm = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Arm.obj");
	leftLeg = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Leg.obj");
	rightLeg = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Leg.obj");

	head->textureArray[0] = LoadTGA("Image//Character//CustomerHead.tga");
	torso->textureArray[0] = LoadTGA("Image//Character//CustomerTorso.tga");
	leftArm->textureArray[0] = LoadTGA("Image//Character//CustomerHand.tga");
	rightArm->textureArray[0] = LoadTGA("Image//Character//CustomerHand.tga");
	leftLeg->textureArray[0] = LoadTGA("Image//Character//CustomerLeg.tga");
	rightLeg->textureArray[0] = LoadTGA("Image//Character//CustomerLeg.tga");
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