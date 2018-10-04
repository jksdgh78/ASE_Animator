#pragma once
#include "stdafx.h"
#include <vector>

class cPyramid
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	glm::mat4					m_matWorld;
	vec4						m_vColor;
public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(vec4 color, glm::mat4* mat = NULL);
	void Render(mat4* viewMatrix = NULL);
};

