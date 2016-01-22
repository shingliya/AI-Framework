#pragma once
#include "GameObject.h"

const int MAX_COSTOMER_COUNT = 6;

class Chef : public CGameObject
{
public:
	enum STATES
	{
		s_Idle,
		s_Cook,
		s_placeFood,
	};
	Chef();
	~Chef();

	void update(double dt);
	void passOrder(bool order[], unsigned sizeOfArray);

	bool isIdle();
	bool isCooking();
	bool isPlacingFood();
	std::string renderState();

	void setToIdle();
	void setToCooking();
	void setToPlacingFood();
	void startTimer(float min, float max);
	void stopTimer();
	bool timerUpdate(const double dt);

	static Vector3 stovePos;
	static Vector3 initPos;
	STATES state;
	float timmer;
	bool orderList[MAX_COSTOMER_COUNT], cookedFoodList[MAX_COSTOMER_COUNT];
	int currentCustomerOrder;
	float timer, timer2;
	float timerLimit;
	bool startToCook;
};