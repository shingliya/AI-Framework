#pragma once
#include "GameObject.h"

class Waitress : public CGameObject
{
	enum STATES
	{
		s_Idle,
		s_Usher,
		s_TakeOrder,
		s_PlaceOrder,
		s_TakeFood,
		s_DeliverFood,
		s_CleanTable
	};

public:
	Waitress();
	~Waitress();

	void setMesh(Mesh* mesh);
	void update(const double & dt);

	Mesh* getMesh();
private:
	Mesh* mesh;
	STATES currentState;
	STATES previousState;
};

