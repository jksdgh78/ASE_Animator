#pragma once
#include "afxcmn.h"


// CMainProcessDlg dialog

class CMainProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainProcessDlg)

public:
	CMainProcessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainProcessDlg();

	// Dialog Data
	enum { IDD = IDD_MAINPROCESSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	// 폴더 내 ASE 파일 리스트 
	CListCtrl m_FileListCtrl;
	CImageList m_ImageList;

	void ReadFile(CString SelFolder);
	void ReadFile(CString SelFolder, bool bSubFolder);

	int m_nSequenceLength;
	virtual BOOL OnInitDialog();
	int m_nCurrent;
	int m_nEnd;
	afx_msg void OnBnClickedLoadase();
	afx_msg void OnClickAseListItem(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_AniListCtrl;

	BOOL m_bSubFolder;
	BOOL m_bAniPlay;
	BOOL m_bAseLoad;
	afx_msg void OnBnClickedFolderOpen();
	afx_msg void OnClickAniList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAniPlay();
	afx_msg void OnBnClickedAniCapture();

	CListCtrl m_TrackListCtrl;

	CString m_AniName;
	CString m_FrameNum;
	afx_msg void OnBnClickedSaveani();
};
