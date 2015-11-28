#include "Customer.h"



Customer::Customer()
{
	type = CUSTOMER;
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