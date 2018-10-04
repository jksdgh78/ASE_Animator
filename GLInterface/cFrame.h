#pragma once
#include "stdafx.h"
#include "cMtlTex.h"
#include <vector>
#include <string>
#include <map>
#include "cSphere.h"

struct stAnimation
{
	std::string sName;
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);

	stAnimation() {}
	stAnimation(std::string s) { sName = s; }
};


class cFrame
{
private:
	SYNTHESIZE(Sphere*, m_pSphere, SelectSphere);
	SYNTHESIZE(std::string, sNodeName, NodeName);
	SYNTHESIZE(std::string, sParentNodeName, ParentNodeName);
	SYNTHESIZE(int, nMaterialRef, MaterialRef);
	std::vector<cFrame*> m_vecChild;
	mat4 m_matWorldTM;
	SYNTHESIZE(mat4, m_matLocalTM, LocalTM);
	SYNTHESIZE(quat, m_qLocalRotate, LocalRotate);
	SYNTHESIZE(mat4, m_matOriginWorldTM, OriginWorldTM);
	SYNTHESIZE(mat4, m_matOriginLocalTM, OriginLocalTM);
	SYNTHESIZE_PASS_BY_REF(std::vector<vec3>, m_vecOriginVertex, VecVertex);
	SYNTHESIZE_PASS_BY_REF(std::vector<vec2>, m_vecTexVertex, VecTexVertex);
	SYNTHESIZE_PASS_BY_REF(std::vector<vec3>, m_vecNormal, VecNormal);
	SYNTHESIZE_PASS_BY_REF(std::vector<int>, m_vecFaceList, FaceList);
	SYNTHESIZE_PASS_BY_REF(std::vector<int>, m_vecTexVertexList, VecTexFace);

	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

	SYNTHESIZE(int, m_nAniCount, AniCount);
	stAnimation* m_pAnimation;

public:
	cFrame();
	~cFrame();

	void Update(int nKeyFrame, mat4* pParent);
	void Render(mat4* pView);
	void AddChild(cFrame* pChild);
	void Destroy();
	void SetWorldTM(mat4* pmat);
	mat4& GetWorldTM();
	void CalcOrginLocalTM(mat4* pParent);
	void GetLocalT( int n,  mat4& mat);
	void GetLocalR( int n,  mat4& mat);

	void GetStaticLocalT(mat4& mat);
	void GetStaticLocalR(mat4& mat);

	void AddAnimation(std::string sName);

	stAnimation* GetAnimation();

	void CaptureAnimation(std::string name, int n);
	void SortAniVector();
	void DestroyAnimation();
};

