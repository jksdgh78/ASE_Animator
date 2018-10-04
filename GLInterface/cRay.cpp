#include "cRay.h"


cRay::cRay(void)
	: m_vOrg(0, 0, 0)
	, m_vDir(0, 0, 0)
{
}


cRay::~cRay(void)
{
}

void cRay::RayAtViewSpace(vec3 cam, mat4 view, mat4 proj, int x, int y)
{
	vec3 vProjDir;

	vProjDir.x = (2.f * x / wx - 1.f) / proj[0][0];
	vProjDir.y = (-2.f * y / wy + 1.f) / proj[1][1];
	vProjDir.z = -1;

	m_vOrg = vec3(0, 0, 0);
	m_vDir = vProjDir;

	mat4 matViewInverse = inverse(view);

	vec4 vOrg = { m_vOrg, 1 };
	vec4 vDir = { m_vDir, 0 };

	m_vOrg =  matViewInverse * vOrg;
	vec3 vDir3;
	vDir = normalize( matViewInverse * vDir);
	vDir3 = vDir;
	m_vDir = normalize(vDir3);
}

// To Do : ���� ������ �Ǿ����� Ȯ��	

bool cRay::IsPicked(float fRadius, vec3 vCenter)
{
	float b, c;
	b = 2.f * dot(m_vDir, (m_vOrg - vCenter));
	c = dot((m_vOrg - vCenter), (m_vOrg - vCenter)) - fRadius * fRadius;

	float discriminant; //�Ǻ��� b^2 - 4ac;
	discriminant = b * b - 4 * c;

	if (discriminant < 0) return false;//������ ����

	discriminant = sqrtf(discriminant);//��Ʈ�����
	float s0 = (-b + discriminant) / 2.0f; // -b + ��Ʈ(�Ǻ���)�� ���
	float s1 = (-b - discriminant) / 2.0f; // -b - ��Ʈ(�Ǻ���)�� ���

										   //�ΰ��� �� �� ��� �ϳ��� ����� ������ �����Ƿ� true ����
	if (s0 >= 0.f || s1 >= 0.f)
		return true;

	return false;
}



