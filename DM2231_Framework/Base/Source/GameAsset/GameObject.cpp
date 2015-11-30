#include "GameObject.h"

float CGameObject::velSpeed = 30.f;

CGameObject::CGameObject() :
	rotateAngle(0),
	pos(0),
	vel(0),
	target(0),
	direction(0),
	type(STATIC_OBJ),
	scale(0),
	rotation(0),
	nodeList(NULL)
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::setRotateAngle(float rotateAngle)
{
	this->rotateAngle = rotateAngle;
}

void CGameObject::setPos(Vector3 pos)
{
	this->pos = pos;
}

void CGameObject::setRotation(Vector3 rotation)
{
	this->rotation = rotation;
}

void CGameObject::setVel(Vector3 vel)
{
	this->vel = vel;
}

void CGameObject::setScale(Vector3 scale)
{
	this->scale = scale;
}

void CGameObject::setDirection(Vector3 direction)
{
	this->direction = direction;
}

void CGameObject::setActive(bool active)
{
	this->active = active;
}

void CGameObject::setMesh(const char* textureDir)
{
}

float CGameObject::getRotateAngle()
{
	return rotateAngle;
}


bool CGameObject::isStaticOBJ()
{
	if (type == STATIC_OBJ)
		return true;
	return false;
}

bool CGameObject::isMovableObj()
{
	if (type == MOVABLE_OBJ)
		return true;
	return false;
}

bool CGameObject::getActive()
{
	return active;
}

Vector3 CGameObject::getPos()
{
	return pos;
}

Vector3 CGameObject::getRotation()
{
	return rotation;
}

Vector3 CGameObject::getVel()
{
	return vel;
}

Vector3 CGameObject::getScale()
{
	return scale;
}

Vector3 CGameObject::getTarget()
{
	return target;
}

Vector3 CGameObject::getDirection()
{
	return direction;
}

CGameObject::GO_TYPE CGameObject::getType()
{
	return type;
}

void CGameObject::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* CGameObject::getMesh()
{
	return mesh;
}