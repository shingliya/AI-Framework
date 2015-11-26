#pragma once
#include "GameObject.h"

class CWorldOBJ : public CGameObject
{
public:
	CWorldOBJ();
	~CWorldOBJ();

	void setMesh(Mesh* mesh);

	Mesh* getMesh();
private:
	Mesh* mesh;
};

