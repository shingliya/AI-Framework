#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	,m_TotalTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = new Animation;
} 

SpriteAnimation::~SpriteAnimation()
{
	if(m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if(m_anim)
	{
		m_TotalTime += dt;
		m_currentTime += dt;

		int numFrame = (m_anim->endFrame - m_anim->startFrame + 1);

		if(numFrame < 1)
			numFrame = 1;

		float frameTime = m_anim->animTime/numFrame;

		if(m_currentTime > frameTime)
		{
			m_currentFrame += 1;
			m_currentTime = 0;
		}

		if(m_currentFrame > m_anim->endFrame)
			m_currentFrame = m_anim->startFrame;

		if(m_TotalTime > m_anim->animTime)
		{
			m_anim->ended = true;
			m_playCount += 1;
			if(((m_playCount + 1) < m_anim->repeatCount) || m_anim->repeatCount == 0)
			{
				m_anim->ended = false;
				++m_playCount;
				m_TotalTime = 0;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	//if(textureID > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	//glDrawArrays(GL_TRIANGLES, offset, count);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	/*if(mode == DRAW_LINES)
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if(mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	{
		glDisableVertexAttribArray(3);
	}
} 