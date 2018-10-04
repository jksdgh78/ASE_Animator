#ifndef GL_INTERFACE_H
#define GL_INTERFACE_H

#include "stdafx.h"
#include "cFrame.h"
#include "cAseLoader.h"
#include "cGrid.h"
#include "cAnimation.h"

class CGLInterface
{
public:
	/// Constructor
	CGLInterface();

	/// Destructor
	virtual ~CGLInterface();

	/// Initialization
	void Initialize();

	/// set projection
	void SetProjectView(int cx, int cy);

	/// draw display by OpenGL functions
	void Display();


// Function Definition Start: SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSs
public:

	/// DrawSphere
	//void DrawSphere();

	///// Draw Trinable
	//void DrawTriangle(vec3 &a, vec3 &b, vec3 &c);	

	///// compute normal direction
	//void CrossProduct(vec3 &a, vec3 &b, vec3 &c, vec3 &r);
	//
	///// compute unit vector
	//void DivideTriangle(vec3 &a, vec3 &b, vec3 &c, int n);
	//
	///// normalize vector
	//void Normalize(vec3 &p);

	/// Set the camera WRT WC and display the scene(카메라를 설정하고 scene 전체를 그린다) 
	void DrawScene(); 


	/// Key Event handler
	void OnKeyDown(unsigned char keyValue); 
	void OnMouseWheel(short zDelta);
	void OnMouseMove(UINT nFlags, float pt[2]);
	void OnLButtonUp(UINT nFlags, float pt[2]);
	void OnLButtonDown(UINT nFlags, float pt[2]);
	void OnRButtonUp(UINT nFlags, float pt[2]);
	void OnRButtonDown(UINT nFlags, float pt[2]);
	void OnTimer(int nElapsedTime);

	void OnSave(std::string sPath);
	void OnLoadASE(std::string sPath);
	void OnPlayPause();
	void OnCapture(int nKeyFrame, std::string sAniName);

	/// variable initialization. 그리기에 필요한 변수를 초기화 한다. 
	void InitializeDrawing();			

	void CalculateCamera();

// Function Definition End: EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe
protected:

	std::vector<cAnimation*> m_vecAnimation;
	cAnimation* m_pCurAnimation;

	int m_nPassedTime;
	Sphere* m_pSelectedSphere;
	bool m_isAniStart;
	int m_nAniFrame;

	cGrid* m_pGrid;

	mat4 m_matViewMatrix;
	mat4 m_matProjMatrix;
	
// Variable Definition Start: SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSs
	int  lightSourceSelection;   // 1: point source, 2: specular, 3: 
	
	float m_theta[3];

	//camera
	bool m_isLButtonDown;
	bool m_isRButtonDown;
	vec2 m_vPrevLButtonPT;
	vec2 m_vPrevRButtonPT;

	vec3 m_vCamPos;
	vec3 m_vCamCalculatedPos;
	vec3 m_vCamRot;
// Variable Definition End : EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe

};

#endif
