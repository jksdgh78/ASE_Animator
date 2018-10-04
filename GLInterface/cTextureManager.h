#pragma once
#include "stdafx.h"
#include <map>
#include <string>

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
	SINGLETONE(cTextureManager);

private:	
	std::map<std::string, GLuint>	m_mapTexture;

public:
	GLuint GetTexture(std::string s);
	GLuint GetTexture(char* sz);

	void Destroy();

};

