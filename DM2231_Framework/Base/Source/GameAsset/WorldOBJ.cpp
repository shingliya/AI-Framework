#include "WorldOBJ.h"



CWorldOBJ::CWorldOBJ()
{
	type = STATIC_OBJ;
}


CWorldOBJ::~CWorldOBJ()
{
}

void CWorldOBJ::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* CWorldOBJ::getMesh()
{
	return mesh;
}