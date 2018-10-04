#include "cSphere.h"
#include <gl\glut.h>


Sphere::Sphere()
{
	//glutWireSphere
	m_vPosition = vec3(0, 0, 0);
	m_isActive = false;
}


Sphere::~Sphere()
{
}

void Sphere::Render()
{
	glPushMatrix();

	if (m_isActive)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 0, 0);

	glutWireSphere(m_fRadius / m_fScale, 10, 10);

	glPopMatrix();
}

void Sphere::Update()
{
}