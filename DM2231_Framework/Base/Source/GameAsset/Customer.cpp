#include "Customer.h"



Customer::Customer()
{
	type = STATIC_OBJ;
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