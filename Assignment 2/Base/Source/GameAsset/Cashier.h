#pragma once
#include "GameObject.h"

class Cashier : public CGameObject
{
public:
	enum STATES
	{
		s_IDLE,
		s_CHECKFORBREAD,
		s_RESTOCK,
		s_SETTLEPURCHASE,
		s_SHOPPING
	};

	Cashier();
	~Cashier();

	static Vector3 CounterPos;
	static Vector3 DoorPos;

	bool isIdle();
	bool isCheckingForBread();
	bool isRestocking();
	bool isSettlingPurchase();
	bool isShopping();

	void setToIdle();
	void setToCheckForBread();
	void setToRestocking();
	void setToSettlePurchase();
	void setToShopping();

	STATES currentState;
	STATES previousState;
	std::string renderState();
	std::string renderState2();
};

