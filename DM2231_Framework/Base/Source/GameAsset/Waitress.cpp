#include "Waitress.h"



Waitress::Waitress()
{
	type = STATIC_OBJ;
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