#pragma once
#include "stdafx.h"

class cMtlTex;
class cFrame;
class Sphere;

#include <map>
#include <string>
#include <vector>
#include <fstream>

#define FRAMESCALE 10.0f

class cAseLoader
{
private:
	FILE* m_fp;
	char m_szToken[1024];

	SYNTHESIZE(std::string, m_sAniName, AniName);
	std::vector<cMtlTex*>			m_vecMtlTex;
	// std::vector<cGroup*>			m_vecGroup;

	cFrame* m_pRootFrame;
	std::map<std::string, cFrame*>	m_mapFrame;

public:
	cAseLoader();
	~cAseLoader();

	cFrame* Load(std::vector<cMtlTex*>& vecMtlTex, std::vector<Sphere*>& vecSphere, std::string szFilename);
	bool Save(std::string sPath, std::string sAniName, std::vector<cMtlTex*>& vecMtlTex, cFrame* pRootFrame);

private:
	char* GetToken();
	int GetInteger();
	float GetFloat();
	bool IsWhite(char c);
	bool IsEqual(char* s1, char* s2);
	void SkipBlock(cFrame* pFrame);

	void ProcessMATERIAL_LIST();
	void ProcessMATERIAL(OUT cMtlTex* pMtlTex);
	void ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex);
	//void ProcessGEOOBJECT(OUT cGroup* pGroup);
	//void ProcessMESH(OUT cGroup* pGroup);
	void ProcessGEOMOBJECT(OUT std::vector<Sphere*>& vecSphere, OUT cFrame* pFrame);
	void ProcessMESH(OUT std::vector<Sphere*>& vecSphere, OUT cFrame* pFrame);
	void ProcessMESH_VERTEX_LIST(OUT cFrame* pFrame, OUT std::vector<vec3>& vecV);
	void ProcessMESH_FACE_LIST(OUT cFrame* pFrame, IN std::vector<vec3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessMESH_TVERTLIST(OUT cFrame* pFrame, OUT std::vector<vec2>& vecVT);
	void ProcessMESH_TFACELIST(OUT cFrame* pFrame, IN std::vector<vec2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessMESH_NORMALS(OUT cFrame* pFrame, OUT std::vector<ST_PNT_VERTEX>& vecVertex);

	void ProcessNODE_TM(OUT cFrame* pFrame);
	void ProcessTM_ANIMATION(OUT cFrame* pFrame);
	void ProcessANI_NAME(OUT cFrame* pFrame);
	void ProcessPOS_TRACK(IN std::string aniName, OUT cFrame* pFrame);
	void ProcessROT_TRACK(IN std::string aniName, OUT cFrame* pFrame);

	void OutputProcessMATERIAL(OUT std::ofstream& out, OUT cMtlTex* pMtlTex);
	void OutputFrame(OUT std::ofstream& ofTemp, std::string sAniName, cFrame* pFrame);
};

