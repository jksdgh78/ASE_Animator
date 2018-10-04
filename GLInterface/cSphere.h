#pragma once
#include "stdafx.h"
class cFrame;

class Sphere
{
private:
	SYNTHESIZE(vec3, m_vPosition, Position);
	SYNTHESIZE(float, m_fRadius, Radius);
	SYNTHESIZE(bool, m_isActive, Active);
	SYNTHESIZE(mat4, m_matLocal, LocalTM );
	SYNTHESIZE(float, m_fScale, Scale);
	SYNTHESIZE(cFrame*, m_pFrame, Frame);
public:
	Sphere();
	~Sphere();

	void Render();
	void Update();


};

