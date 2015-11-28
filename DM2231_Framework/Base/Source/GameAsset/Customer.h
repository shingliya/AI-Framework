#pragma once
#include "GameObject.h"

class Customer : public CGameObject
{
public:
	enum STATES
	{
		s_Queing,
		s_SitDown,
		s_ReadyToOrder,
		s_Ordering,
		s_WaitForFood,
		s_Eating,
		s_Leave,
	};

	Customer();
	~Customer();

	void setMesh(Mesh* mesh);

	Mesh* getMesh();
	void update(double dt/* waiter obj refrence */);

	STATES state;
	float timmer;
private:
	Mesh* mesh;
};