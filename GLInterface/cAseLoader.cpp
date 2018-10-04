#include "cAseLoader.h"
#include "cFrame.h"
#include "cMtlTex.h"
#include "Asciitok.h"
#include "cSphere.h"
#include <fstream>
#include <vector>
using namespace std;

cAseLoader::cAseLoader()
	: m_fp(NULL)
	, m_pRootFrame(NULL)
{
}


cAseLoader::~cAseLoader()
{
}

cFrame* cAseLoader::Load(std::vector<cMtlTex*>& vecMtlTex, std::vector<Sphere*>& vecSphere, string szFilename)
{
	cFrame* pFrame = new cFrame;

	fopen_s(&m_fp, szFilename.c_str(), "r");
	while (true)
	{
		char* szToken = GetToken();

		if (szToken == NULL)
		{
			break;
		}
		else if (IsEqual(szToken, ID_SCENE))
		{
			SkipBlock(pFrame);
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
			vecMtlTex = m_vecMtlTex;
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			if (m_pRootFrame == NULL)
			{
				m_pRootFrame = pFrame;
				ProcessGEOMOBJECT(vecSphere, pFrame);
			}
			else
			{
				pFrame = new cFrame;
				ProcessGEOMOBJECT(vecSphere, pFrame);
			}
		}
	}

	fclose(m_fp);

	for each(cMtlTex* p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	m_pRootFrame->CalcOrginLocalTM(NULL);

	return m_pRootFrame;
}

bool cAseLoader::Save(std::string sPath, std::string sAniName, std::vector<cMtlTex*>& vecMtlTex, cFrame * pRootFrame)
{
	ofstream ofTemp;

	ofTemp.open(sPath);
	if (!ofTemp)
	{
		return false;
	}

	//MATERIAL_LIST output start
	ofTemp << "*MATERIAL_LIST {" << endl;
	ofTemp << "\t*MATERIAL_COUNT " << vecMtlTex.size() << endl;

	for (int i = 0; i < vecMtlTex.size(); i++)
	{
		ofTemp << "\t*MATERIAL " << i << " {" << endl;
		OutputProcessMATERIAL(ofTemp, vecMtlTex[i]);
		ofTemp << "\t}" << endl;
	}

	ofTemp << "}" << endl;
	//MATERIAL_LIST output end

	OutputFrame(ofTemp, sAniName, pRootFrame);
	
	return true;
}


void cAseLoader::OutputFrame(OUT std::ofstream& ofTemp, std::string sAniName, cFrame * pFrame)
{
	//GEOMOBJECT output start
	mat4 mat = pFrame->GetOriginWorldTM();
	ofTemp << "*GEOMOBJECT {" << endl;
	ofTemp << "\t*NODE_NAME \"" << pFrame->GetNodeName() << "\"" << endl;
	if(pFrame->GetParentNodeName() != "")
		ofTemp << "\t*NODE_PARENT \"" << pFrame->GetParentNodeName() << "\"" << endl;
	ofTemp << "\t*NODE_TM {" << endl;
	ofTemp << "\t\t*TM_ROW0 " << mat[0][0] << " " << mat[2][0] << " " << mat[1][0] << endl;
	ofTemp << "\t\t*TM_ROW1 " << mat[0][2] << " " << mat[2][2] << " " << mat[1][2] << endl;
	ofTemp << "\t\t*TM_ROW2 " << mat[0][1] << " " << mat[2][1] << " " << mat[1][1] << endl;
	ofTemp << "\t\t*TM_ROW3 " << mat[0][3] << " " << mat[2][3] << " " << mat[1][3] << endl;
	ofTemp << "\t}" << endl;
	ofTemp << "\t*MESH {" << endl;
	ofTemp << "\t\t*MESH_NUMVERTEX " << pFrame->GetVecVertex().size() << endl;
	ofTemp << "\t\t*MESH_NUMFACES " << pFrame->GetFaceList().size()/3 << endl;
	ofTemp << "\t\t*MESH_VERTEX_LIST {" << endl;
	for (int i = 0; i < pFrame->GetVecVertex().size(); i++)
	{
		ofTemp << "\t\t\t*MESH_VERTEX " << i << " " << pFrame->GetVecVertex()[i].x << " "
			<< pFrame->GetVecVertex()[i].z << " " << pFrame->GetVecVertex()[i].y << endl;
	}
	ofTemp << "\t\t}" << endl;

	ofTemp << "\t\t*MESH_FACE_LIST {" << endl;
	for (int i = 0; i < pFrame->GetFaceList().size(); i += 3)
	{
		ofTemp << "\t\t\t*MESH_FACE " << i / 3<< ": A: " << pFrame->GetFaceList()[i] << " B: "
			<< pFrame->GetFaceList()[i + 1] << " C: " << pFrame->GetFaceList()[i + 2] << endl;
	}
	ofTemp << "\t\t}" << endl;
	ofTemp << "\t\t*MESH_NUMTVERTEX " << pFrame->GetVecTexVertex().size() << endl;

	ofTemp << "\t\t*MESH_TVERTLIST {" << endl;
	for (int i = 0; i < pFrame->GetVecTexVertex().size(); i++)
	{
		ofTemp << "\t\t\t*MESH_TVERT " << i << " " << pFrame->GetVecTexVertex()[i].x << " "
			<< (1.f - pFrame->GetVecTexVertex()[i].y) << endl;
	}
	ofTemp << "\t\t}" << endl;
	ofTemp << "\t\t*MESH_NUMTVFACES " << pFrame->GetVecTexFace().size() / 3<< endl;
	ofTemp << "\t\t*MESH_TFACELIST {" << endl;
	for (int i = 0; i < pFrame->GetVecTexFace().size(); i += 3)
	{
		ofTemp << "\t\t\t*MESH_TFACE " << i / 3 << " " << pFrame->GetVecTexFace()[i] << " "
			<< pFrame->GetVecTexFace()[i + 1] << " " << pFrame->GetVecTexFace()[i + 2] << endl;
	}
	ofTemp << "\t\t}" << endl;


	ofTemp << "\t\t*MESH_NORMALS {" << endl;
	for (int i = 0; i < pFrame->GetVecNormal().size() / 3; i++)
	{
		ofTemp << "\t\t\t*MESH_FACENORMAL " << i << endl;
			ofTemp << "\t\t\t\t*MESH_VERTEXNORMAL " << i << " " << pFrame->GetVecNormal()[i * 3].x << " "
				<< pFrame->GetVecNormal()[i * 3].z << " " << pFrame->GetVecNormal()[i * 3].y << endl;
			ofTemp << "\t\t\t\t*MESH_VERTEXNORMAL " << i << " " << pFrame->GetVecNormal()[i * 3 + 2].x << " "
				<< pFrame->GetVecNormal()[i * 3 + 2].z << " " << pFrame->GetVecNormal()[i * 3 + 2].y << endl;
			ofTemp << "\t\t\t\t*MESH_VERTEXNORMAL " << i << " " << pFrame->GetVecNormal()[i * 3 + 1].x << " "
				<< pFrame->GetVecNormal()[i * 3 + 1].z << " " << pFrame->GetVecNormal()[i * 3 + 1].y << endl;
	}
	ofTemp << "\t\t}" << endl;
	ofTemp << "\t}" << endl;
	


	stAnimation* pAni = pFrame->GetAnimation();

	if (pAni != NULL)
	{
		ofTemp << "\t*TM_ANIMATION {" << endl;
		ofTemp << "\t\t*NODE_NAME \"" << pFrame->GetNodeName() << "\"" << endl;
		ofTemp << "\t\t*ANI_NAME \"" << sAniName << "\"" << endl;
		if (!pAni->GetPosTrack().empty())
		{
			ofTemp << "\t\t*CONTROL_POS_TRACK {" << endl;
			for (int i = 0; i < pAni->GetPosTrack().size(); i++)
			{
				ofTemp << "\t\t\t*CONTROL_POS_SAMPLE " << pAni->GetPosTrack()[i].n << " "
					<< pAni->GetPosTrack()[i].v.x << " " << pAni->GetPosTrack()[i].v.z << " " << pAni->GetPosTrack()[i].v.y << endl;

			}
			ofTemp << "\t\t}" << endl;
		}
		if (!pAni->GetRotTrack().empty())
		{
			vector<ST_ROT_SAMPLE> ori;
			ori.resize(pAni->GetRotTrack().size());
			ST_ROT_SAMPLE o;
			quat p;
			for (int i = pAni->GetRotTrack().size() - 1; i >= 0; i--)
			{
				if (i > 0)
				{
					o = pAni->GetRotTrack()[i];
					p = inverse(pAni->GetRotTrack()[i - 1].q);

					o.q = o.q * p;
				}
				else if (i == 0)
				{
					o = pAni->GetRotTrack()[0];
				}

				if (o.q.w > 1.0f) {
					o.q.w = 1.0f;
				}
				else if (o.q.w < -1.0f)
				{
					o.q.w = -1.0f;
				}

				o.q.w = acosf(o.q.w) * 2.0f;

				float f = sinf(o.q.w / 2.0f);

				if (f != 0.f)
				{
					o.q.x = o.q.x / f;
					o.q.y = o.q.y / f;
					o.q.z = o.q.z / f;
				}

				ori[i] = o;
			}

			ofTemp << "\t\t*CONTROL_ROT_TRACK {" << endl;
			for (int i = 0; i < ori.size(); i++)
			{
				ofTemp << "\t\t\t*CONTROL_ROT_SAMPLE " << ori[i].n << " "
					<< ori[i].q.x << " " << ori[i].q.z << " " << ori[i].q.y
					<< " " << ori[i].q.w << endl;

			}
			ofTemp << "\t\t}" << endl;
		}

		ofTemp << "\t}" << endl;
	}
	
	if(pFrame->GetMaterialRef() != -1)
		ofTemp << "\t*MATERIAL_REF " << pFrame->GetMaterialRef() << endl;
	ofTemp << "}" << endl;

	for each (auto p in pFrame->m_vecChild)
	{
		OutputFrame(ofTemp, sAniName, p);
	}
}

char* cAseLoader::GetToken()
{
	bool isQuote = false;
	int nReadCount = 0;

	while (true)
	{
		char c = fgetc(m_fp);
		if (feof(m_fp)) break;
		if (c == '\"')
		{
			if (isQuote)
				break;
			isQuote = true;
			continue;
		}

		if (!isQuote && IsWhite(c))
		{
			if (nReadCount == 0)
				continue;
			break;
		}
		m_szToken[nReadCount++] = c;
	}

	if (nReadCount == 0)
		return NULL;

	m_szToken[nReadCount] = '\0';

	return m_szToken;
}

int cAseLoader::GetInteger()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhite(char c)
{
	return c < 33;
}

bool cAseLoader::IsEqual(char * s1, char * s2)
{
	return strcmp(s1, s2) == 0;
}

void cAseLoader::SkipBlock(cFrame* pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, "*ANI_COUNT"))
		{
			pFrame->SetAniCount(GetInteger());
		}
		else if (IsEqual(szToken, "*ANI_LIST"))
		{
			ProcessANI_NAME(pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessANI_NAME(OUT cFrame * pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, "*ANI_NAME"))
		{
			//pFrame->AddAnimation(GetToken());
		}

	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			m_vecMtlTex.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int nMtlRef = GetInteger();
			m_vecMtlTex[nMtlRef] = new cMtlTex;
			ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL(OUT cMtlTex * pMtlTex)
{
	MATERIAL9 stMtl;
	ZeroMemory(&stMtl, sizeof(MATERIAL9));

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_AMBIENT))
		{
			stMtl.Ambient[0] = GetFloat();
			stMtl.Ambient[1] = GetFloat();
			stMtl.Ambient[2] = GetFloat();
			stMtl.Ambient[3] = 1.0f;
		}
		else if (IsEqual(szToken, ID_DIFFUSE))
		{
			stMtl.Diffuse[0] = GetFloat();
			stMtl.Diffuse[1] = GetFloat();
			stMtl.Diffuse[2] = GetFloat();
			stMtl.Diffuse[3] = 1.0f;
		}
		else if (IsEqual(szToken, ID_SPECULAR))
		{
			stMtl.Specular[0] = GetFloat();
			stMtl.Specular[1] = GetFloat();
			stMtl.Specular[2] = GetFloat();
			stMtl.Specular[3] = 1.0f;
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}

	} while (nLevel > 0);

	pMtlTex->SetMtl(stMtl);
}

void cAseLoader::ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_BITMAP))
		{
			std::string path = GetToken();
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(path));
			pMtlTex->SetPath(path);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessGEOMOBJECT(OUT std::vector<Sphere*>& vecSphere, OUT cFrame* pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH))
		{
			ProcessMESH(vecSphere, pFrame);
		}
		else if (IsEqual(szToken, ID_NODE_NAME))
		{
			char* name = GetToken();
			m_mapFrame[name] = pFrame;
			pFrame->SetNodeName(name);
		}
		else if (IsEqual(szToken, ID_NODE_PARENT))
		{
			std::string sParentName(GetToken());
			m_mapFrame[sParentName]->AddChild(pFrame);
			pFrame->SetParentNodeName(sParentName);

		}
		else if (IsEqual(szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(pFrame);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			int num = GetInteger();
			pFrame->SetMtlTex(m_vecMtlTex[num]);
			pFrame->SetMaterialRef(num);
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH(OUT std::vector<Sphere*>& vecSphere, OUT cFrame* pFrame)
{
	std::vector<vec3> vecV;
	std::vector<vec2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
		{
			int size = GetInteger();
			vecV.resize(size);
			pFrame->GetVecVertex().resize(size);
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES))
		{
			int size = GetInteger();
			vecVertex.resize(3 * size);
			pFrame->GetFaceList().resize(3 * size);
			pFrame->GetVecNormal().resize(3 * size);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(pFrame, vecV);
		}
		else if (IsEqual(szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(pFrame, vecV, vecVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
		{
			int size = GetInteger();
			vecVT.resize(size);
			pFrame->GetVecTexVertex().resize(size);
		
			if (size == 0)
			{
				Sphere* pSphere = new Sphere;

				pSphere->SetFrame(pFrame);
				pSphere->SetScale(FRAMESCALE);
				pSphere->SetRadius(0.8f);
				pSphere->SetPosition(vec3(0, 0, 0));
				pFrame->SetSelectSphere(pSphere);
				vecSphere.push_back(pSphere);
			}
		}
		else if (IsEqual(szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTLIST(pFrame, vecVT);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVFACES))
		{
			int size = GetInteger();
			pFrame->GetVecTexFace().resize(size * 3);
		}
		else if (IsEqual(szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACELIST(pFrame, vecVT, vecVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(pFrame, vecVertex);
		}

	} while (nLevel > 0);

	// To Do : 프레임의 월드 매트릭스의 역행렬을 정점 정보에 적용하기
	mat4 matWorld = pFrame->GetWorldTM();
	mat4 matInvWorld = inverse(matWorld);
	
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		vec4 temp = { vecVertex[i].p, 1 };
		vecVertex[i].p = temp * matInvWorld;
		temp = { vecVertex[i].n, 0 };
		vecVertex[i].n = temp * matInvWorld;
	}

	pFrame->SetVertex(vecVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(OUT cFrame* pFrame, OUT std::vector<vec3>& vecV)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX))
		{
			int nIndex = GetInteger();
			vec3 p;
			p.x = GetFloat();
			p.z = GetFloat();
			p.y = GetFloat();
			vecV[nIndex] = p;
			pFrame->GetVecVertex()[nIndex] = p;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST(OUT cFrame* pFrame, IN std::vector<vec3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;	
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACE))
		{
			int nFaceIndex = GetInteger();
			GetToken();
			int nA = GetInteger();
			GetToken();
			int nC = GetInteger();
			GetToken();
			int nB = GetInteger();

			pFrame->GetFaceList()[nFaceIndex * 3] = nA;
			pFrame->GetFaceList()[nFaceIndex * 3 + 1] = nC;
			pFrame->GetFaceList()[nFaceIndex * 3 + 2] = nB;

			vecVertex[nFaceIndex * 3 + 0].p = vecV[nA];
			vecVertex[nFaceIndex * 3 + 1].p = vecV[nB];
			vecVertex[nFaceIndex * 3 + 2].p = vecV[nC];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST(OUT cFrame* pFrame, OUT std::vector<vec2>& vecVT)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TVERT))
		{
			int nIndex = GetInteger();
			vec2 p;
			p.x = GetFloat();
			p.y = 1 - GetFloat();
			vecVT[nIndex] = p;
			pFrame->GetVecTexVertex()[nIndex] = p;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST(OUT cFrame* pFrame, IN std::vector<vec2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TFACE))
		{
			int nFaceIndex = GetInteger();
			int nA = GetInteger();
			int nC = GetInteger();
			int nB = GetInteger();

			pFrame->GetVecTexFace()[nFaceIndex * 3] = nA;
			pFrame->GetVecTexFace()[nFaceIndex * 3 + 1] = nC;
			pFrame->GetVecTexFace()[nFaceIndex * 3 + 2] = nB;

			vecVertex[nFaceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[nFaceIndex * 3 + 1].t = vecVT[nB];
			vecVertex[nFaceIndex * 3 + 2].t = vecVT[nC];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS(OUT cFrame* pFrame, OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nFaceIndex = 0;
	int nCount = 0;
	int nModCount[] = { 0, 2, 1 };
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACENORMAL))
		{
			nFaceIndex = GetInteger();
			nCount = 0;

		}
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
		{
			GetToken();
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();
			pFrame->GetVecNormal()[nFaceIndex * 3 + nModCount[nCount]] = vec3(x, y, z);
			vecVertex[nFaceIndex * 3 + nModCount[nCount++]].n = vec3(x, y, z);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessNODE_TM( OUT cFrame * pFrame)
{
	// To Do : 노드의 트랜스폼 매트릭스 정보 읽기
	mat4 mat;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_TM_ROW0))
		{
			mat[0][0] = GetFloat();
			mat[2][0] = GetFloat();
			mat[1][0]= GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW1))
		{
			mat[0][2] = GetFloat();
			mat[2][2] = GetFloat();
			mat[1][2] = GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW2))
		{
			mat[0][1] = GetFloat();
			mat[2][1] = GetFloat();
			mat[1][1] = GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW3))
		{
			mat[0][3] = GetFloat();
			mat[2][3] = GetFloat();
			mat[1][3] = GetFloat();
		}
	} while (nLevel > 0);

	pFrame->SetWorldTM(&mat);
	pFrame->SetOriginWorldTM(mat);
}

void cAseLoader::ProcessTM_ANIMATION(OUT cFrame * pFrame)
{
	// To Do : 트랜스폼의 애니메이션 정보 읽기
	int nLevel = 0;
	std::string aniName;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_ANI_NAME))
		{
			aniName = GetToken();
			if (aniName != "")
			{
				pFrame->AddAnimation(aniName);

				if (m_sAniName == "")
				{
					m_sAniName = aniName;
				}
			}

			
		}
		else if (IsEqual(szToken, ID_POS_TRACK))
		{
			ProcessPOS_TRACK(aniName, pFrame);
		}
		else if (IsEqual(szToken, ID_ROT_TRACK))
		{
			ProcessROT_TRACK(aniName, pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessPOS_TRACK(IN std::string aniName, OUT cFrame * pFrame)
{
	// To Do : 애니메이션 트랙의 위치 정보 읽기
	std::vector<ST_POS_SAMPLE> vecPosTrack;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_POS_SAMPLE))
		{
			ST_POS_SAMPLE s;
			s.n = GetInteger();
			s.v.x = GetFloat();
			s.v.z = GetFloat();
			s.v.y = GetFloat();
			vecPosTrack.push_back(s);
		}
	} while (nLevel > 0);

	pFrame->GetAnimation()->SetPosTrack(vecPosTrack);
}

void cAseLoader::ProcessROT_TRACK(IN std::string aniName, OUT cFrame * pFrame)
{
	// To Do : 애니메이션 트랙의 회전 정보 읽기
	std::vector<ST_ROT_SAMPLE> vecRotTrack;
	std::vector<ST_ROT_SAMPLE> vecOriginRotTrack;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_ROT_SAMPLE))
		{
			ST_ROT_SAMPLE s, s_ori;
			s.n = GetInteger();
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();
			float w = GetFloat();

			s_ori.n = s.n;
			s_ori.q.x = x;
			s_ori.q.y = y;
			s_ori.q.z = z;
			s_ori.q.w = w;

			vecOriginRotTrack.push_back(s_ori);

			s.q.x = x * sinf(w / 2.0f);
			s.q.y = y * sinf(w / 2.0f);
			s.q.z = z * sinf(w / 2.0f);
			s.q.w = cosf(w / 2.0f);

			if (!vecRotTrack.empty())
			{
				s.q = s.q * vecRotTrack.back().q;
			}

			vecRotTrack.push_back(s);
		}
	} while (nLevel > 0);

	pFrame->GetAnimation()->SetRotTrack(vecRotTrack);
}

void cAseLoader::OutputProcessMATERIAL(OUT std::ofstream & out, OUT cMtlTex * pMtlTex)
{
	out << "\t\t*MATERIAL_AMBIENT " << pMtlTex->GetMtl().Ambient[0] << " " <<
		pMtlTex->GetMtl().Ambient[1] << " " << pMtlTex->GetMtl().Ambient[2] << endl;
	out << "\t\t*MATERIAL_DIFFUSE " << pMtlTex->GetMtl().Diffuse[0] << " " <<
		pMtlTex->GetMtl().Diffuse[1] << " " << pMtlTex->GetMtl().Diffuse[2] << endl;
	out << "\t\t*MATERIAL_SPECULAR " << pMtlTex->GetMtl().Specular[0] << " " <<
		pMtlTex->GetMtl().Specular[1] << " " << pMtlTex->GetMtl().Specular[2] << endl;
	out << "\t\t*MATERIAL_SHINE " << pMtlTex->GetMtl().Power << endl;
	
	string path = pMtlTex->GetPath();
	if (path != "")
	{
		out << "\t\t*MAP_DIFFUSE {" << endl;
		out << "\t\t\t*BITMAP \"" << path << "\""<< endl;
		out << "\t\t}" << endl;
	}
}

