#pragma once
#include "GameObject.h"

class Waitress : public CGameObject
{
public:
	Waitress();
	~Waitress();

	void setMesh(Mesh* mesh);

	Mesh* getMesh();
private:
	Mesh* mesh;
};

