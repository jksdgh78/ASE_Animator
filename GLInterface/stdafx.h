#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#ifndef _APPLE_OSX
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif


#define GLIF_DOWN          0x28
#define GLIF_UP           0x26
#define GLIF_RIGHT          0x27
#define GLIF_LEFT           0x25
#define GLIF_PAGEUP			0x21
#define GLIF_PAGEDOWN		0x22

#define PI 3.14159f

#include <glm/glm/fwd.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/vec3.hpp>
#include <glm/glm/vec4.hpp>
#include <glm/glm/vec2.hpp>
#include <glm/glm/mat4x4.hpp>
#include <glm/glm/detail/func_matrix.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>


using namespace glm;

extern int wx, wy;

#define SINGLETONE(class_name) \
private:\
	class_name(void);\
	~class_name(void);\
public:\
	static class_name*	GetInstance()\
	{\
	static class_name instance;\
	return &instance;\
	}


#define SAFE_ADD_REF(p) if (p) {p->AddRef();}
#define SAFE_RELEASE(p) if (p) {p->Release(); p = NULL; }
#define SAFE_DELETE(p) if (p) {delete p; p = NULL; }


#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	varName = var;\
	}\
}

// D3D 정점 FVF플래그 작성 순서 및 플래그
// 
// 1. Position 정점의 좌표 x,y,z(float) :  D3DFVF_XYZ
// 2. RHW (float)                       :  D3DFVF_XYZRHW (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 3. Blending Weight Data 결합 가중치 (float)         : D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 4. Vertex Normal 정점의 법선 벡터 x,y,z(float)    : D3DFVF_NORMAL
// 5. Vertex Point Size 정점의 점 크기 (float)          : D3DFVF_PSIZE
// 6. Diffuse Color (DWORD)                               : D3DFVF_DIFFUSE
// 7. Specular Color (DWORD)                            D3DFVF_SPECULAR
// 8. Texture Coordinate Set 1 (float)                   : D3DFVF_TEX0 - D3DFVF_TEX8

enum FVF
{
	XYZ = 1, DIFFUSE = 2, TEX1 = 4, NORMAL = 8
};

struct ST_PC_VERTEX
{
	//float x, y, z;
	vec3 p;
	vec3 c;

	ST_PC_VERTEX()
		: p(0, 0, 0)
		, c(0, 0, 0) {}
	ST_PC_VERTEX(vec3 _v, vec3 _c)
		: p(_v)
		, c(_c) {}
	enum { FVF = XYZ | DIFFUSE };
};

struct ST_PT_VERTEX
{
	//float x, y, z;
	vec3 p;
	vec2 t;
	enum { FVF = XYZ | TEX1, };
};

struct ST_PN_VERTEX
{
	vec3 p;
	vec3 n;
	ST_PN_VERTEX(vec3 _p, vec3 _n)
		: p(_p)
		, n(_n)
	{
	}
	enum { FVF = XYZ | NORMAL, };
};

struct ST_PNT_VERTEX
{
	vec3 p;
	vec3 n;
	vec2 t;
	ST_PNT_VERTEX() {}
	ST_PNT_VERTEX(vec3 _p, vec3 _n, vec2 _t)
		: p(_p)
		, n(_n)
		, t(_t)
	{
	}
	enum { FVF = XYZ | NORMAL | TEX1, };
};

struct ST_POS_SAMPLE
{
	int			n;
	vec3		v;

	bool operator < (ST_POS_SAMPLE right)
	{
		return n < right.n;
	}
};

struct ST_ROT_SAMPLE
{
	int			n;
	quat		q;

	bool operator < (ST_ROT_SAMPLE right)
	{
		return n < right.n;
	}
};

struct ST_SIZE
{
	float fWidth;
	float fHeight;
	ST_SIZE() : fWidth(0), fHeight(0) {}
	ST_SIZE(float w, float h) : fWidth(w), fHeight(h) {}
};

typedef struct _MATERIAL9 {
	GLfloat   Diffuse[4];        /* Diffuse color RGBA */
	GLfloat   Ambient[4];        /* Ambient color RGB */
	GLfloat   Specular[4];       /* Specular 'shininess' */
	GLfloat   Emissive[4];       /* Emissive color RGB */
	float     Power;          /* Sharpness if specular highlight */

	static void CopyColor(GLfloat* a, GLfloat* b)
	{
		for (int i = 0; i < 4; i++)
		{
			a[i] = b[i];
		}
	}
} MATERIAL9;

#include "cTextureManager.h"