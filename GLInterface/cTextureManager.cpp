
#include "cTextureManager.h"
#include "SOIL.h"

using namespace std;

cTextureManager::cTextureManager(void)
{
}


cTextureManager::~cTextureManager(void)
{
}

GLuint cTextureManager::GetTexture(std::string s)
{
	//m_mapTexture[s]
	if (m_mapTexture.find(s) == m_mapTexture.end())
	{
		GLuint tex = SOIL_load_OGL_texture(
			s.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		if (0 == tex)
		{
			return -1;
		}
		else {
			m_mapTexture.insert(make_pair(s, tex));
		}
	}

	return m_mapTexture[s];
}

GLuint cTextureManager::GetTexture(char* sz)
{
	return GetTexture(std::string(sz));
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		glDeleteTextures(1, &it.second);
	}
}
