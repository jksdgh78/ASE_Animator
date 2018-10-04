
#include "cFrame.h"
#include <algorithm>

using namespace std;
//test

cFrame::cFrame()
{
	m_pMtlTex = NULL;
	nMaterialRef = -1;
	m_pSphere = NULL;
	m_pAnimation = NULL;
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex)
}

void cFrame::Update(int nKeyFrame, mat4 * pParent)
{
	mat4 matT, matR;
	GetLocalT(nKeyFrame, matT);
	GetLocalR(nKeyFrame, matR);

	//matR = toMat4(m_qLocalRotate);

    matR = transpose(matR);
	//matT = transpose(matT);

	m_matLocalTM = matR * matT;
	//m_matLocalTM = transpo

	if (pParent)
	{
		m_matWorldTM = *pParent * m_matLocalTM;
	}
	else
	{
		m_matWorldTM = m_matLocalTM;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(nKeyFrame, &m_matWorldTM);
	}
}

void cFrame::Render(mat4* pView)
{
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	GLfloat arrMat[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			arrMat[i * 4 + j] = m_matLocalTM[j][i];
		}
	}

	glMultMatrixf(arrMat);

	if (m_pMtlTex)
	{

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_pMtlTex->GetMtl().Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_pMtlTex->GetMtl().Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_pMtlTex->GetMtl().Specular);
		glMaterialf(GL_FRONT, GL_SHININESS, m_pMtlTex->GetMtl().Power);

		glBindTexture(GL_TEXTURE_2D, m_pMtlTex->GetTexture());
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_vecVertex.size(); i += 3)
		{
			glTexCoord2f(m_vecVertex[i].t.x, m_vecVertex[i].t.y);
			glNormal3f(m_vecVertex[i].n.x, m_vecVertex[i].n.y, m_vecVertex[i].n.z);
			glVertex3f(m_vecVertex[i].p.x, m_vecVertex[i].p.y, m_vecVertex[i].p.z);

			glTexCoord2f(m_vecVertex[i + 1].t.x, m_vecVertex[i + 1].t.y);
			glNormal3f(m_vecVertex[i + 1].n.x, m_vecVertex[i + 1].n.y, m_vecVertex[i + 1].n.z);
			glVertex3f(m_vecVertex[i + 1].p.x, m_vecVertex[i + 1].p.y, m_vecVertex[i + 1].p.z);

			glTexCoord2f(m_vecVertex[i + 2].t.x, m_vecVertex[i + 2].t.y);
			glNormal3f(m_vecVertex[i + 2].n.x, m_vecVertex[i + 2].n.y, m_vecVertex[i + 2].n.z);
			glVertex3f(m_vecVertex[i + 2].p.x, m_vecVertex[i + 2].p.y, m_vecVertex[i + 2].p.z);
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	if (m_pSphere)
	{
		GLfloat temp[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, temp);

		mat4 matrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix[j][i] = temp[i * 4 + j];
			}
		}

		mat4 transmatrix = transpose(matrix);
		mat4 invmatrix = inverse(*pView);
		mat4 result = invmatrix * transmatrix;

		vec3 v = result * vec4(0, 0, 0, 1);
		m_pSphere->SetPosition(v);
		m_pSphere->Render();
	}


	for each(auto p in m_vecChild)
	{
		p->Render(pView);
	}

	glPopMatrix();
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	SAFE_DELETE(m_pAnimation);

	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}

	delete this;
}

void cFrame::SetWorldTM(mat4 * pmat)
{
	if (pmat == NULL)
	{
		return;
	}

	m_matWorldTM = *pmat;
}

mat4 & cFrame::GetWorldTM()
{
	return m_matWorldTM;
}

void cFrame::CalcOrginLocalTM(mat4 * pParent)
{
	if (pParent)
	{
		mat4 matInvParent = transpose(*pParent);
		matInvParent = inverse(matInvParent);
		matInvParent = transpose(matInvParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
		m_matOriginLocalTM = m_matLocalTM;
	}
	else
	{
		m_matLocalTM = m_matWorldTM;
		m_matOriginLocalTM = m_matWorldTM;
	}

	mat4 Rot = m_matLocalTM;
	Rot[0][3] = 0;
	Rot[1][3] = 0;
	Rot[2][3] = 0;

	//Rot = transpose(Rot);
	m_qLocalRotate = toQuat(Rot);

	for each(auto p in m_vecChild)
	{
		p->CalcOrginLocalTM(&m_matWorldTM);
	}
}

void cFrame::GetLocalT( int n,  mat4 & mat)
{
	mat = mat4(1.0f);

	if (!m_pAnimation)
	{
		mat[0][3] = m_matLocalTM[0][3];
		mat[1][3] = m_matLocalTM[1][3];
		mat[2][3] = m_matLocalTM[2][3];

		return;
	}
	
	if (m_pAnimation->GetPosTrack().empty())
	{
		mat[0][3] = m_matLocalTM[0][3];
		mat[1][3] = m_matLocalTM[1][3];
		mat[2][3] = m_matLocalTM[2][3];
	}
	else if (n <= m_pAnimation->GetPosTrack().front().n)
	{
		mat[0][3] = m_pAnimation->GetPosTrack().front().v.x;
		mat[1][3] = m_pAnimation->GetPosTrack().front().v.y;
		mat[2][3] = m_pAnimation->GetPosTrack().front().v.z;
	}
	else if (n >= m_pAnimation->GetPosTrack().back().n)
	{
		mat[0][3] = m_pAnimation->GetPosTrack().back().v.x;
		mat[1][3] = m_pAnimation->GetPosTrack().back().v.y;
		mat[2][3] = m_pAnimation->GetPosTrack().back().v.z;
	}
	else {
		int nNext = 0;
		for (size_t i = 0; i < m_pAnimation->GetPosTrack().size(); i++)
		{
			if (n < m_pAnimation->GetPosTrack()[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;

		float t = (n - m_pAnimation->GetPosTrack()[nPrev].n)
			/ (float)(m_pAnimation->GetPosTrack()[nNext].n - m_pAnimation->GetPosTrack()[nPrev].n);

		vec3 v = mix(m_pAnimation->GetPosTrack()[nPrev].v, m_pAnimation->GetPosTrack()[nNext].v, t);

		mat[0][3] = v.x;
		mat[1][3] = v.y;
		mat[2][3] = v.z;
	}
}

void cFrame::GetLocalR( int n,  mat4 & mat)
{
	mat = mat4(1.0f);
	
	if (!m_pAnimation)
	{
		mat = transpose(m_matLocalTM);
		//mat = m_matLocalTM;
		mat[3][0] = 0;
		mat[3][1] = 0;
		mat[3][2] = 0;

		mat4 rot = transpose(mat);
		m_qLocalRotate = toQuat(rot);

		return;
	}

	if (m_pAnimation->GetRotTrack().empty())
	{
		mat = transpose(m_matLocalTM);
		//mat = m_matLocalTM;
		mat[3][0] = 0;
		mat[3][1] = 0;
		mat[3][2] = 0;

		mat4 rot = transpose(mat);
		m_qLocalRotate = toQuat(rot);

	}
	else if (n < m_pAnimation->GetRotTrack().front().n)
	{
		m_qLocalRotate = m_pAnimation->GetRotTrack().front().q;
		mat = toMat4(m_pAnimation->GetRotTrack().front().q);
		//mat = rot * mat;
	}
	else if (n >= m_pAnimation->GetRotTrack().back().n)
	{
		m_qLocalRotate = m_pAnimation->GetRotTrack().back().q;
		mat = toMat4(m_pAnimation->GetRotTrack().back().q);
		//mat = rot * mat;
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_pAnimation->GetRotTrack().size(); ++i)
		{
			if (n < m_pAnimation->GetRotTrack()[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (n - m_pAnimation->GetRotTrack()[nPrev].n)
			/ (float)(m_pAnimation->GetRotTrack()[nNext].n - m_pAnimation->GetRotTrack()[nPrev].n);

		quat q;
		q = slerp(m_pAnimation->GetRotTrack()[nPrev].q, m_pAnimation->GetRotTrack()[nNext].q, t);

		m_qLocalRotate = q;

		mat = toMat4(q);
		//mat = rot * mat;
	}
}

void cFrame::GetStaticLocalT(mat4 & mat)
{
	mat = mat4(1.0f);

	mat[0][3] = m_matLocalTM[0][3];
	mat[1][3] = m_matLocalTM[1][3];
	mat[2][3] = m_matLocalTM[2][3];

}

void cFrame::GetStaticLocalR(mat4 & mat)
{
}

void cFrame::AddAnimation(std::string sName)
{
	m_pAnimation = new stAnimation(sName);
}


stAnimation * cFrame::GetAnimation()
{
	return m_pAnimation;
}

void cFrame::CaptureAnimation(string name, int n)
{
	if (m_pSphere)
	{
		mat4 mat = m_matLocalTM;
		//mat = m_matLocalTM;
		mat[0][3] = 0;
		mat[1][3] = 0;
		mat[2][3] = 0;

		mat = transpose(mat);

		m_qLocalRotate = toQuat(mat);

		ST_ROT_SAMPLE rot;
		rot.n = n;
		rot.q = m_qLocalRotate;

		if (!m_pAnimation)
		{
			stAnimation* pAnimation = NULL;
			pAnimation = new stAnimation;
			pAnimation->sName = name;
			
			pAnimation->GetRotTrack().push_back(rot);
			m_pAnimation = pAnimation;
		}
		else
		{
			m_pAnimation->GetRotTrack().push_back(rot);
		}
	}

	for each(auto p in m_vecChild)
	{
		p->CaptureAnimation(name, n);
	}
}

void cFrame::SortAniVector()
{
	if (m_pAnimation)
	{
		sort(m_pAnimation->GetPosTrack().begin(), m_pAnimation->GetPosTrack().end());
		sort(m_pAnimation->GetRotTrack().begin(), m_pAnimation->GetRotTrack().end());
	}

	for each(auto p in m_vecChild)
	{
		p->SortAniVector();
	}
}

void cFrame::DestroyAnimation()
{
	if (m_pAnimation)
	{
		SAFE_DELETE(m_pAnimation);
	}

	for each(auto p in m_vecChild)
	{
		p->DestroyAnimation();
	}
}
