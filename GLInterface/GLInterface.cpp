
#include "GLInterface.h"
#include <math.h>
#include "cRay.h"
#include "cSphere.h"


int wx = 0;
int wy = 0;

CGLInterface::CGLInterface()
{
	
}

CGLInterface::~CGLInterface()
{
	for each(auto p in m_vecAnimation)
	{
		p->Destroy();
	}
}


void CGLInterface::Initialize()
{
    
	InitializeDrawing();

	m_pGrid = new cGrid;
	m_pGrid->Setup(15, 1);
}

void CGLInterface::SetProjectView(int cx, int cy)
{
	// cx와 cy는 클라이언트 영역의 x축, y축 크기
	if (cx == 0 || cy == 0) return;
	// TODO: Add your message handler code here
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	wx = cx;
	wy = cy;

	// 65.0 fovy
	// ratio x to y
	// znear 
	// zfar
	GLfloat temp2[16];
	mat4 temp;
	gluPerspective(65.0, (GLfloat)cx / (GLsizei)cy, 1.0, 500.0);
	glGetFloatv(GL_PROJECTION_MATRIX, temp2);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[j][i] = temp2[i * 4 + j];
		}
	}

	m_matProjMatrix = glm::perspective(radians(65.0f), (float)cx / (int)cy, (float)1.0, (float)500.0);

	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_MODELVIEW);
}

void CGLInterface::Display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// buffer clear color 설정
	glClear(GL_COLOR_BUFFER_BIT);	// 설정된 color로 버퍼를 clear
	DrawScene();
	glFlush();
}

void CGLInterface::InitializeDrawing()  // 그리기에 필효한 변수를 초기화 한다. 
{
	m_theta[0] = 0;
	m_theta[1] = 0;
	m_theta[2] = 0;

	m_vCamPos.x = 0;
	m_vCamPos.y = 0;
	m_vCamPos.z = 5.f;

	m_vCamCalculatedPos.x = 0;
	m_vCamCalculatedPos.y = 0;
	m_vCamCalculatedPos.z = 1.f;

	m_vCamRot.x = 0;
	m_vCamRot.y = 0;
	m_vCamRot.z = 0;

	m_isLButtonDown = false; 
	m_isRButtonDown = false;	
	m_pSelectedSphere = NULL;
	m_pCurAnimation = NULL;
	m_isAniStart = false;

	m_nPassedTime = 0.f;
	m_nAniFrame = 640;


	//cAnimation*	pAnimation = new cAnimation;
	//cAseLoader loader;
	//pAnimation->SetFrame(loader.Load(pAnimation->GetMtlTex(), pAnimation->GetVecSphere(), "woman/woman_01_run.ASE"));
	//pAnimation->SetAnimationName(loader.GetAniName());

	////	pAnimation->GetFrame()->DestroyAnimation();
	//m_pCurAnimation = pAnimation;
	//m_pCurAnimation->GetFrame()->SortAniVector();

	//if (m_pCurAnimation->GetFrame()->GetAnimation())
	//{
	//	m_pCurAnimation->SetMinFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().front().n);
	//	m_pCurAnimation->SetMaxFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n);
	//	m_nAniFrame = m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n + 640;
	//}

	//m_vecAnimation.push_back(pAnimation);
}

void CGLInterface::CalculateCamera()
{
	quat rotX = angleAxis(glm::radians(m_vCamRot.x), vec3(1, 0, 0));
	quat rotY = angleAxis(glm::radians(m_vCamRot.y), vec3(0, 1, 0));
	
	m_vCamCalculatedPos = rotate(rotY * rotX, m_vCamPos);
}

void CGLInterface::OnTimer(int nElapsedTime)
{
	if(m_isAniStart)
		m_nPassedTime += nElapsedTime;
}

void CGLInterface::OnSave(std::string sPath)
{
	cAseLoader loader;
	//loader.Save("woman/woman_01_salute.ASE", m_pCurAnimation->GetAnimationName(), m_pCurAnimation->GetMtlTex(), m_pCurAnimation->GetFrame());
	loader.Save("woman/" + sPath + ".ASE", m_pCurAnimation->GetAnimationName(), m_pCurAnimation->GetMtlTex(), m_pCurAnimation->GetFrame());
}

void CGLInterface::OnLoadASE(std::string sPath)
{
	if (m_pCurAnimation)
	{
		m_pCurAnimation->Destroy();
		m_pCurAnimation = NULL;
	}

	cAnimation*	pAnimation = new cAnimation;
	cAseLoader loader;
	pAnimation->SetFrame(loader.Load(pAnimation->GetMtlTex(), pAnimation->GetVecSphere(), sPath));
	pAnimation->SetAnimationName(loader.GetAniName());

	//	pAnimation->GetFrame()->DestroyAnimation();
	m_pCurAnimation = pAnimation;
	m_pCurAnimation->GetFrame()->SortAniVector();

	m_isAniStart = false;

	if (m_pCurAnimation->GetFrame()->GetAnimation())
	{
		m_pCurAnimation->SetMinFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().front().n);
		m_pCurAnimation->SetMaxFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n);
		m_nAniFrame = m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n + 640;
	}

	m_vecAnimation.push_back(pAnimation);
}

void CGLInterface::OnPlayPause()
{
	m_isAniStart = !m_isAniStart;
}

void CGLInterface::OnCapture(int nKeyFrame, std::string sAniName)
{
	m_pCurAnimation->SetAnimationName(sAniName);
	m_pCurAnimation->GetFrame()->CaptureAnimation(sAniName, nKeyFrame);
	m_pCurAnimation->GetFrame()->SortAniVector();

	m_pCurAnimation->SetCurFrameNum(nKeyFrame);
	m_pCurAnimation->SetMinFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().front().n);
	m_pCurAnimation->SetMaxFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n);
}

void CGLInterface::OnKeyDown(unsigned char keyValue)
{
	switch (keyValue)
	{
	case 'U': case 'u':
		m_pCurAnimation->GetFrame()->Update(640, NULL);
		break;
	case 'S': case 's':
	{
		//cAseLoader loader;
		////loader.Save("woman/woman_01_salute.ASE", m_pCurAnimation->GetAnimationName(), m_pCurAnimation->GetMtlTex(), m_pCurAnimation->GetFrame());
		//loader.Save("woman/run.ASE", m_pCurAnimation->GetAnimationName(), m_pCurAnimation->GetMtlTex(), m_pCurAnimation->GetFrame());
	}
		break;
	case 'C': case 'c':
	{
		//m_pCurAnimation->GetFrame()->CaptureAnimation(m_pCurAnimation->GetAnimationName(), m_nAniFrame);
		//m_pCurAnimation->SetAnimationName("salute");
		//m_pCurAnimation->GetFrame()->CaptureAnimation("salute", m_nAniFrame);
		//m_pCurAnimation->GetFrame()->SortAniVector();

		//m_pCurAnimation->SetCurFrameNum(m_nAniFrame);
		//m_pCurAnimation->SetMinFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().front().n);
		//m_pCurAnimation->SetMaxFrameNum(m_pCurAnimation->GetFrame()->GetAnimation()->GetRotTrack().back().n);

		//m_nAniFrame += 640;
	}
		break;
	case VK_BACK:
		if (m_pCurAnimation)
		{
			m_pCurAnimation->GetFrame()->DestroyAnimation();
			m_pCurAnimation->SetCurFrameNum(0);
			m_pCurAnimation->SetMaxFrameNum(0);
			m_pCurAnimation->SetMinFrameNum(0);
			m_nAniFrame = 640;
		}
		break;
	case VK_SPACE:
		m_isAniStart = !m_isAniStart;
		break;
	case GLIF_UP:
		if (m_pSelectedSphere)
		{
			cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);

			q = rotate(q, -0.1f, vec3(1, 0, 0));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	case GLIF_DOWN:
		if (m_pSelectedSphere)
		{
			cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);
			q = rotate(q, 0.1f, vec3(1, 0, 0));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	case GLIF_RIGHT:
		if (m_pSelectedSphere)
		{
		 	cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);
			q = rotate(q, 0.1f, vec3(0, 1, 0));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	case GLIF_LEFT:
		if (m_pSelectedSphere)
		{
			cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);
			q = rotate(q, -0.1f, vec3(0, 1, 0));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	case GLIF_PAGEUP:
		if (m_pSelectedSphere)
		{
			cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);
			q = rotate(q, 0.1f, vec3(0, 0, 1));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	case GLIF_PAGEDOWN:
		if (m_pSelectedSphere)
		{
			cFrame* pFrame = m_pSelectedSphere->GetFrame();
			mat4 mat = pFrame->GetLocalTM();
			//mat = m_matLocalTM;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			//mat = transpose(mat);

			quat q = toQuat(mat);
			q = rotate(q, -0.1f, vec3(0, 0, 1));
			pFrame->SetLocalRotate(q);

			mat4 T, R = toMat4(q);
			//R = transpose(R);
			pFrame->GetStaticLocalT(T);
			mat4 LocalTM = R * T;
			pFrame->SetLocalTM(LocalTM);

		}
		break;
	}
}

void CGLInterface::OnMouseWheel(short zDelta)
{
	m_vCamPos.z -= zDelta / 100.f;
	if (m_vCamPos.z < 1)
		m_vCamPos.z = 1;
}

void CGLInterface::OnMouseMove(UINT nFlags, float pt[2])
{
	if (m_isRButtonDown)
	{
		vec2 ptCurrMouse;
		ptCurrMouse.x = pt[0];
		ptCurrMouse.y = pt[1];

		float fDeltaX = ptCurrMouse.x - m_vPrevRButtonPT.x;
		float fDeltaY = ptCurrMouse.y - m_vPrevRButtonPT.y;

		m_vCamRot.x -= (fDeltaX / 10);
		m_vCamRot.y -= (fDeltaY / 10);

		if (m_vCamRot.x < -90.f + 0.01f)
			m_vCamRot.x = -90.f + 0.01f;
		if (m_vCamRot.x > 90.f - 0.01f)
			m_vCamRot.x = 90.f - 0.01f;

		m_vPrevRButtonPT = ptCurrMouse;
	}
}

void CGLInterface::OnLButtonUp(UINT nFlags, float pt[2])
{
	m_isLButtonDown = false;
}

void CGLInterface::OnLButtonDown(UINT nFlags, float pt[2])
{
	m_isLButtonDown = true;
	m_vPrevLButtonPT.x = pt[0];
	m_vPrevLButtonPT.y = pt[1];

	if (m_pCurAnimation)
	{
		cRay r;
		r.SetOrg(m_vCamCalculatedPos);
		r.RayAtViewSpace(m_vCamCalculatedPos, m_matViewMatrix, m_matProjMatrix, m_vPrevLButtonPT.y, m_vPrevLButtonPT.x);

		std::vector<Sphere*>* pVecSphere = &m_pCurAnimation->GetVecSphere();

		Sphere* pSelectedSphere = NULL;
		for (int i = 0; i < pVecSphere->size(); i++)
		{
			Sphere* pSphere = (*pVecSphere)[i];
			pSphere->SetActive(false);
			if (r.IsPicked(pSphere->GetRadius(), pSphere->GetPosition()))
			{
				pSelectedSphere = pSphere;
			}
		}

		if (pSelectedSphere)
		{
			if (pSelectedSphere == m_pSelectedSphere)
			{
				pSelectedSphere->SetActive(false);
				m_pSelectedSphere = NULL;
			}
			else
			{
				pSelectedSphere->SetActive(true);
				m_pSelectedSphere = pSelectedSphere;
			}
		}
	}
}

void CGLInterface::OnRButtonUp(UINT nFlags, float pt[2])
{
	m_isRButtonDown = false;
}

void CGLInterface::OnRButtonDown(UINT nFlags, float pt[2])
{
	m_isRButtonDown = true;
	m_vPrevRButtonPT.x = pt[0];
	m_vPrevRButtonPT.y = pt[1];
}

void CGLInterface::DrawScene()
{
	// Enable hidden surface removal
	glEnable(GL_DEPTH_TEST);	

	// background color setting
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// initialize modelview matrix as identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	CalculateCamera();

	//gluLookAt(0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0);  // for teapot
	GLfloat temp2[16];
	mat4 temp;

	gluLookAt(m_vCamCalculatedPos.x, m_vCamCalculatedPos.y, m_vCamCalculatedPos.z, 0, 0, 0, 0, 1.0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, temp2);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[j][i] = temp2[i * 4 + j];
		}
	}
	m_matViewMatrix = glm::lookAt(m_vCamCalculatedPos, vec3(0, 0, 0), vec3(0, 1, 0));

	// WC에 대한 Object coordinate를 설정한다. m_theta[]를 변화시켜 animiation 가능 
	glRotatef(m_theta[2], 0.0f, 0.0f, 1.0f);
	glRotatef(m_theta[1], 0.0f, 1.0f, 0.0f);
	glRotatef(m_theta[0], 1.0f, 0.0f, 0.0f);

	///////////
	//??????

	m_pGrid->Render(&m_matViewMatrix);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 광원 위치 
	GLfloat position0[4] = { 20.0f, 100.0f, 20.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	// 주변광
	GLfloat ambient0[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);

	// 난반사광
	GLfloat diffuse0[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);

	// 전반사광
	GLfloat specular0[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	GLfloat spot_direction[3] = { -1.0f, -1.0f, -1.0f };

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	// 절단각(광원에서 빛이 퍼지는 각)
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 200.0);
	// 적당히 반짝이는 점
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (m_pCurAnimation)
	{
		if (m_pCurAnimation->GetFrame() != NULL && m_isAniStart)
		{
			int div = m_pCurAnimation->GetMaxFrameNum() - m_pCurAnimation->GetMinFrameNum();
			int nKeyFrame;
			if (div > 0)
				nKeyFrame = (m_nPassedTime % div) + m_pCurAnimation->GetMinFrameNum();
			else if (div == 0)
				nKeyFrame = div;
			m_pCurAnimation->GetFrame()->Update(nKeyFrame, NULL);
		}

		glEnable(GL_LIGHTING);
		glScalef(10.0f, 10.0f, 10.0f);

		if (m_pCurAnimation->GetFrame())
		{
			m_pCurAnimation->GetFrame()->Render(&m_matViewMatrix);
		}
	}
}


//
//
//void CGLInterface::DrawSphere()
//{
//	///////////////////////////////////////////
//	// Flat-Shaded Sphere
//
//	point3 v[4] = { { 0.0f, 0.0f, 1.0f },
//	{ 0.0f, 0.942809f, -0.333333f },
//	{ -0.816497f, -0.471405f, -0.333333f },
//	{ 0.816497f, -0.471405f, -0.333333f } };
//
//	// level of subdivison
//	int n = 5;
//
//	// a tetrahedron
//	DivideTriangle(v[0], v[1], v[2], n);
//	DivideTriangle(v[0], v[2], v[3], n);
//	DivideTriangle(v[0], v[3], v[1], n);
//	DivideTriangle(v[3], v[2], v[1], n);
//}
//
//
//
//
//
//
//void CGLInterface::DivideTriangle(point3 &a, point3 &b, point3 &c, int n)
//{
//	if( n > 0 )
//	{
//		point3 v1, v2, v3;
//		for(register int j=0; j<3; j++)
//		{
//			v1[j] = a[j] + b[j];
//			v2[j] = b[j] + c[j];
//			v3[j] = c[j] + a[j];
//		}
//		Normalize(v1);
//		Normalize(v2);
//		Normalize(v3);
//
//		DivideTriangle(a, v1, v3, n-1);
//		DivideTriangle(b, v2, v1, n-1);
//		DivideTriangle(c, v3, v2, n-1);
//		DivideTriangle(v1, v2, v3, n-1);
//	}
//	else
//		DrawTriangle(a, b, c);
//}
//
//void CGLInterface::Normalize(point3 &p)
//{
//	double d = p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
//	if( d > 0 )
//	{
//		float len = (float)(1.0 / sqrt(d));
//		p[0] *= len;
//		p[1] *= len;
//		p[2] *= len;
//	}
//}
//
//void CGLInterface::DrawTriangle(point3 &a, point3 &b, point3 &c)
//{
//	point3 n;
//	CrossProduct(a, b, c, n);
//
//	// smooth shading
//	glShadeModel(GL_FLAT);
//
//	glBegin(GL_TRIANGLES);
//	glNormal3fv(a);
//	glVertex3fv(a);
//	glNormal3fv(b);
//	glVertex3fv(b);
//	glNormal3fv(c);
//	glVertex3fv(c);
//	glEnd();
//}
//
//void CGLInterface::CrossProduct(point3 &a, point3 &b, point3 &c, point3 &r)
//{
//	r[0] = (b[1]-a[1])*(c[2]-a[2]) - (b[2]-a[2])*(c[1]-a[1]);
//	r[1] = (b[2]-a[2])*(c[0]-a[0]) - (b[0]-a[0])*(c[2]-a[2]);
//	r[2] = (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
//	Normalize(r);
//}


