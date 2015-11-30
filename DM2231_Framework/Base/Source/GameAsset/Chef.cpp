#include "Chef.h"



Chef::Chef()
{
	type = CHEF;
	state = s_Idle;
	timmer = -1;
	numOfOrder = numOfCookedFood = 0;
}


Chef::~Chef()
{
}


void Chef::update(double dt)
{
	switch (state)
	{
	case s_Idle:
		if (numOfOrder > 0)
		{
			numOfOrder -= 1;
			state = s_Cook;
		}
		break;
	case s_Cook:
		if (timmer == -1)
		{
			timmer = Math::RandFloatMinMax(5, 15);
		}
		else
		{
			timmer -= dt;
			if (timmer < 0)
			{
				timmer = -1;
				state = s_placeFood;
			}
		}
		break;
	case s_placeFood:
		//move to waypoint and place food
		//if (reach waypoint)
		{
			numOfCookedFood += 1;
			state = s_Idle;
		}
		break;
	}
}