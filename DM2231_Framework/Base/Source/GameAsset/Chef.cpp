#include "Chef.h"



Chef::Chef()
{
	type = CHEF;
	state = s_Idle;
	timmer = -1;
	numOfOrder = numOfCookedFood = 0;
	
	setMesh2();
}


Chef::~Chef()
{
}

void Chef::setMesh2()
{
	head = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Head.obj");
	torso = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Torso.obj");
	leftArm = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Arm.obj");
	rightArm = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Arm.obj");
	leftLeg = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Leg.obj");
	rightLeg = MeshBuilder::GenerateOBJ("Head", "OBJ//Character//model//Leg.obj");

	head->textureArray[0] = LoadTGA("Image//Character//ChefHead.tga");
	torso->textureArray[0] = LoadTGA("Image//Character//ChefTorso.tga");
	leftArm->textureArray[0] = LoadTGA("Image//Character//ChefHand.tga");
	rightArm->textureArray[0] = LoadTGA("Image//Character//ChefHand.tga");
	leftLeg->textureArray[0] = LoadTGA("Image//Character//ChefLeg.tga");
	rightLeg->textureArray[0] = LoadTGA("Image//Character//ChefLeg.tga");
}

Mesh* Chef::getMesh()
{
	return mesh;
}

void Chef::update(double dt)
{
	switch (state)
	{
	case s_Idle:
		if (numOfOrder > 0)
		{
			numOfOrder -= 1;
			state = s_Cook;
		}
		break;
	case s_Cook:
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
				state = s_placeFood;
			}
		}
		break;
	case s_placeFood:
		//move to waypoint and place food
		//if (reach waypoint)
		{
			numOfCookedFood += 1;
			state = s_Idle;
		}
		break;
	}
}