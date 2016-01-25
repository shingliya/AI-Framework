#pragma once
#include "GameObject.h"

const int MAX_COSTOMER_COUNT = 6;

class Baker : public CGameObject
{
public:
	enum STATES
	{
		s_Baking,
		s_PlaceFoodAtTable,
		s_RestockMaterial,
		s_OutOfMaterial,
		s_Cashier,
	};
	Baker();
	~Baker();

	void update(double dt);
	void passOrder(bool order[], unsigned sizeOfArray);

	/*bool isIdle();
	bool isCooking();
*/
	bool isBaking();
	bool isPlacingFood();
	bool isRestocking();
	bool isOutOfMaterial();
	bool isCashier();
	std::string renderState();

	/*void setToIdle();
	void setToCooking();
	void setToPlacingFood();*/
	void setToBaking();
	void setToPlacingFood();
	void setToRestock();
	void setToInformCashier();
	void setToCashier();

	void startTimer(float min, float max);
	void stopTimer();
	bool timerUpdate(const double dt);

	static Vector3 stovePos;
	static Vector3 initPos;
	static Vector3 fridgePos;
	static Vector3 tablePos;
	STATES state;
	float currentTime, timeLimit;
	bool startToCook;
};