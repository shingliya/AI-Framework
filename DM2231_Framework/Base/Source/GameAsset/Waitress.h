#pragma once
#include "GameObject.h"

class Waitress : public CGameObject
{
public:
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

	Waitress();
	~Waitress();

	bool isIdle();
	bool isUsher();
	bool isTakingOrder();
	bool isPlacingOrder();
	bool isTakingFood();
	bool isDeliveringFood();
	bool isCleaningTable();
	bool isCleaningFloor();

	void setToIdle();
	void setToUsher();
	void setToTakeOrder();
	void setToPlaceOrder();
	void setToTakeFood();
	void setToDeliveryFood();
	void setToCleanTable();

	void update(const double & dt);
	std::string renderState();

	STATES currentState;
	STATES previousState;
	static Vector3 queuePos;
	Vector3 initPos;
	std::string state;
	bool usheringCustomer;
};

