#pragma once

#include "Vector3.h"
#include "../Camera3.h"
#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include <unordered_map>

class CGameObject
{
public:
	//std::string ROLES[4] = {"Baker", "Customer", "Shopkeeper"};
	
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
	void setMesh(const char* textureDir);

	//Getter
	float getRotateAngle();
	bool isStaticOBJ();
	bool isMovableObj();
	bool getActive();
	Vector3 getPos();
	Vector3 getRotation();
	Vector3 getVel();
	Vector3 getScale();
	Vector3 getTarget();
	Vector3 getDirection();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

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
	Mesh* mesh;
	Vector3 stateText;


	//Map
	std::unordered_map<int, std::string> nodeList;
};

