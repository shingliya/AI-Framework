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
		s_CleanTable,
		s_CleanFloor
	};

public:
	Waitress();
	~Waitress();

	void update(const double & dt);

private:
	STATES currentState;
	STATES previousState;
};

