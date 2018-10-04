#pragma once
#include <cFrame.h>
#include <vector>
#include <cMtlTex.h>
#include "cSphere.h"
#include <string>

class cAnimation
{
public:
	cAnimation();
	~cAnimation();

	void Destroy();

private:

	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtlTex, MtlTex);
	SYNTHESIZE(cFrame*, m_pFrame, Frame);
	SYNTHESIZE(int, m_nMinFrameNum, MinFrameNum);
	SYNTHESIZE(int, m_nMaxFrameNum, MaxFrameNum);
	SYNTHESIZE(int, m_nCurFrameNum, CurFrameNum);
	SYNTHESIZE(std::string, m_sAnimationName, AnimationName);
	SYNTHESIZE_PASS_BY_REF(std::vector<Sphere*>, m_vecSphere, VecSphere);
};

