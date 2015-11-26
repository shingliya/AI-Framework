#include "Chef.h"



Chef::Chef()
{
	type = STATIC_OBJ;
}


Chef::~Chef()
{
}

void Chef::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* Chef::getMesh()
{
	return mesh;
}