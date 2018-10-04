#pragma once
#include "stdafx.h"
#include <string>
class cMtlTex
{

private:
	SYNTHESIZE(MATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE(GLuint, m_pTexture, Texture);
	SYNTHESIZE(std::string, m_sPath, Path);

public:
	cMtlTex(void);
	~cMtlTex(void);

	void Release() {
		
	}
};

