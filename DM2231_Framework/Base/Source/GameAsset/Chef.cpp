#include "Chef.h"



Chef::Chef()
{
	type = STATIC_OBJ;
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