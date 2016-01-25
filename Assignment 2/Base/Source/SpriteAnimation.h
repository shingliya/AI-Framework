#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include "GL\glew.h"

struct Animation
{
	void Set(int startFrame, int endFrame, int repeat, float time) 
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		ended = false;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
}; class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();

	int m_row;
	int m_col;

	float m_TotalTime;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};