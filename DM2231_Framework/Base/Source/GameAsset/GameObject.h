#pragma once

#include "Vector3.h"
#include "../Camera3.h"
#include "../MeshBuilder.h"
#include <unordered_map>

class CGameObject
{
public:
	enum GO_TYPE
	{
		STATIC_OBJ,
		MOVABLE_OBJ,
		PLAYER,
		ENEMY,
		PLAYERBULLET,
		ENEMYBULLET
	};
	
	CGameObject();
	virtual ~CGameObject();

	//Setter
	void setRotateAngle(float rotateAngle);
	void setPos(Vector3 pos);
	void setRotation(Vector3 rotation);
	void setVel(Vector3 vel);
	void setScale(Vector3 scale);
	void setDirection(Vector3 direction);
	void setActive(bool active);
	void setType_PlayerBullet();
	void setType_EnemyBullet();

	//Getter
	float getRotateAngle();
	bool isPlayer();
	bool isEnemy();
	bool isStaticOBJ();
	bool isMovableObj();
	bool isPlayerBullet();
	bool isEnemyBullet();
	bool getActive();
	Vector3 getPos();
	Vector3 getRotation();
	Vector3 getVel();
	Vector3 getScale();
	Vector3 getTarget();
	Vector3 getDirection();
	GO_TYPE getType();
protected:
	//float
	static float velSpeed;
	float rotateAngle;

	//bool
	bool active;

	//Class or Struct Variables
	Vector3 pos;
	Vector3 rotation;
	Vector3 vel;
	Vector3 scale;
	Vector3 target;
	Vector3 direction;
	GO_TYPE type;

	//Map
	std::unordered_map<int, std::string> nodeList;
};

