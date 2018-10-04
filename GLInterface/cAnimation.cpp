#include "cAnimation.h"



cAnimation::cAnimation()
{
	m_nCurFrameNum = 0;
	m_nMaxFrameNum = 0;
	m_nMinFrameNum = 0;
}


cAnimation::~cAnimation()
{
}

void cAnimation::Destroy()
{
	for (int i = 0; i < m_vecMtlTex.size(); i++)
	{
		SAFE_DELETE(m_vecMtlTex[i]);
	}

	for (int i = 0; i < m_vecSphere.size(); i++)
	{
		SAFE_DELETE(m_vecSphere [i]);
	}

	m_pFrame->Destroy();

	delete this;
}
