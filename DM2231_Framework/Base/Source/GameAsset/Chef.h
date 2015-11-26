#pragma once
#include "GameObject.h"

class Chef : public CGameObject
{
public:
	Chef();
	~Chef();

	void setMesh(Mesh* mesh);

	Mesh* getMesh();
private:
	Mesh* mesh;
};

