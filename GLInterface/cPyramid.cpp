#include "StdAfx.h"
#include "cPyramid.h"


cPyramid::cPyramid(void)
{
}


cPyramid::~cPyramid(void)
{
}

void cPyramid::Setup(vec4 c, glm::mat4* mat /*= NULL*/)
{
	m_vColor = c;

	if (mat)
		m_matWorld = *mat;
	else
		m_matWorld = glm::mat4(1.0f);

	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, -1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0)));

	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, 1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, -1), glm::vec3(0, 0, 0)));

	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, 1), glm::vec3(0, 0, 0)));

	m_vecVertex.push_back(ST_PN_VERTEX(vec3(0, 0, 0), vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(vec3(-1, -1, -1), vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(vec3(-1, -1, 1), vec3(0, 0, 0)));

	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, -1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, 1), glm::vec3(0, 0, 0)));

	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0)));
	m_vecVertex.push_back(ST_PN_VERTEX(glm::vec3(1, -1, 1), glm::vec3(0, 0, 0)));

	for (int i = 0; i < m_vecVertex.size(); i += 3)
	{
		glm::vec3 v1 = m_vecVertex[i + 0].p;
		glm::vec3 v2 = m_vecVertex[i + 1].p;
		glm::vec3 v3 = m_vecVertex[i + 2].p;
		glm::vec3 v12 = v2 - v1;
		glm::vec3 v13 = v3 - v1;
		glm::vec3 n;
		n = cross(v12, v13);
		n = normalize(n);
		
		m_vecVertex[i + 0].n = n;
		m_vecVertex[i + 1].n = n;
		m_vecVertex[i + 2].n = n;
	}
}

void cPyramid::Render(mat4* viewMatrix)
{
	glPushMatrix();

	GLfloat matData[16];
	if (!viewMatrix)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matData[i * 4 + j] = m_matWorld[i][j];
			}
		}
	}
	else
	{
		mat4 tempMat = *viewMatrix * m_matWorld;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matData[i * 4 + j] = tempMat[i][j];
			}
		}
	}

	glLoadIdentity();

	glLoadMatrixf(matData);

	glBegin(GL_POLYGON);
	glColor4f(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);

	for (int i = 0; i < m_vecVertex.size(); i += 3)
	{	
		glNormal3f(m_vecVertex[i].n.x, m_vecVertex[i].n.y, m_vecVertex[i].n.z);
		glVertex3f(m_vecVertex[i].p.x, m_vecVertex[i].p.y, m_vecVertex[i].p.z);

		glNormal3f(m_vecVertex[i + 1].n.x, m_vecVertex[i + 1].n.y, m_vecVertex[i + 1].n.z);
		glVertex3f(m_vecVertex[i + 1].p.x, m_vecVertex[i + 1].p.y, m_vecVertex[i + 1].p.z);

		glNormal3f(m_vecVertex[i + 2].n.x, m_vecVertex[i + 2].n.y, m_vecVertex[i + 2].n.z);
		glVertex3f(m_vecVertex[i + 2].p.x, m_vecVertex[i + 2].p.y, m_vecVertex[i + 2].p.z);
	}
	glEnd();

	
	glPopMatrix();
}
