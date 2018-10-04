#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid(void)
{
}


cGrid::~cGrid(void)
{
	for each(auto p in m_vecPyramid)
	{
		SAFE_DELETE(p);
	}
}

void cGrid::Setup(int nLine, float fInterval /*= 1.0f*/)
{
	vec3 c;
	float fMax = nLine * fInterval;
	for (int i = 1; i <= nLine; ++i)
	{
		if (i % 5 == 0)
			c = vec3(255, 255, 255);
		else
			c = vec3(128, 128, 128);
		m_vecVertex.push_back(ST_PC_VERTEX(vec3(-fMax, 0, i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(vec3(fMax, 0, i), c));

		m_vecVertex.push_back(ST_PC_VERTEX(vec3(-fMax, 0, -i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(vec3(fMax, 0, -i), c));

		m_vecVertex.push_back(ST_PC_VERTEX(vec3(i, 0, -fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(vec3(i, 0, fMax), c));

		m_vecVertex.push_back(ST_PC_VERTEX(vec3(-i, 0, -fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(vec3(-i, 0, fMax), c));
	}

	c = vec3(0, 0, 1);
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(0, 0, -fMax), c));
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(0, 0, fMax), c));

	c = vec3(0, 1, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(0, -fMax, 0), c));
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(0, fMax, 0), c));

	c = vec3(1, 0, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(-fMax, 0, 0), c));
	m_vecVertex.push_back(ST_PC_VERTEX(vec3(fMax, 0, 0), c));

	mat4 matS, matR, mat;
	matS = scale(matS, vec3(0.1, 1, 0.1));

	float angle = radians(-90.f);
	cPyramid* pPyramid = new cPyramid;
	matR = rotate(mat4(), angle, vec3(0, 0, 1));

	mat =  matR * matS;

	vec4 color = { 255, 0, 0, 1 };
	pPyramid->Setup(color, &mat);
	m_vecPyramid.push_back(pPyramid);

	angle = radians(180.f);
	pPyramid = new cPyramid;

	matR = rotate(mat4(), angle, vec3(1, 0, 0));
	mat = matR* matS;

	color = { 0, 255, 0, 1 };
	pPyramid->Setup(color, &mat);
	m_vecPyramid.push_back(pPyramid);

	angle = radians(90.f);
	pPyramid = new cPyramid;
	matR = rotate(mat4(), angle, vec3(1, 0, 0));
	
	mat = matR* matS;

	color = { 0, 0, 255, 1 };
	pPyramid->Setup(color, &mat);
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::Render(mat4* viewMatrix)
{
	glPushMatrix();

	glDisable(GL_LIGHTING);
	
	glBegin(GL_LINES);
	for (int i = 0; i < m_vecVertex.size(); i += 2)
	{
		glColor3f(m_vecVertex[i].c[0], m_vecVertex[i].c[1], m_vecVertex[i].c[2]);
		glVertex3f(m_vecVertex[i].p[0], m_vecVertex[i].p[1], m_vecVertex[i].p[2]);
		glColor3f(m_vecVertex[i + 1].c[0], m_vecVertex[i + 1].c[1], m_vecVertex[i + 1].c[2]);
		glVertex3f(m_vecVertex[i + 1].p[0], m_vecVertex[i + 1].p[1], m_vecVertex[i + 1].p[2]);
	}
	glEnd();

	for each(auto p in m_vecPyramid)
		p->Render(viewMatrix);

	glPopMatrix();
}
