#pragma once
#include "GameObject.h"

class Customer : public CGameObject
{
public:
	Customer();
	~Customer();

	void setMesh(Mesh* mesh);

	Mesh* getMesh();
private:
	Mesh* mesh;
};

