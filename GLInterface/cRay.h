#pragma once
#include "stdafx.h"

class cRay
{
protected:
    SYNTHESIZE(fvec3, m_vOrg, Org);
	
	fvec3 m_vDir;

public:
	cRay(void);
	~cRay(void);

	void RayAtViewSpace(vec3 cam, mat4 view, mat4 proj, int x, int y);

	bool IsPicked(float fRadius, vec3 vCenter);
};

