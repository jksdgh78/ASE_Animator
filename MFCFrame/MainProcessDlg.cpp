// MainProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG_Animation.h"
#include "MainProcessDlg.h"
#include "afxdialogex.h"
#include "CG_AnimationView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

// CMainProcessDlg dialog

IMPLEMENT_DYNAMIC(CMainProcessDlg, CDialogEx)

CMainProcessDlg::CMainProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainProcessDlg::IDD, pParent)
	, m_nCurrent(0)
	, m_nEnd(0)
{
	m_bAseLoad = FALSE;
}

CMainProcessDlg::~CMainProcessDlg()
{
}

void CMainProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASE_LIST, m_FileListCtrl);
	DDX_Text(pDX, IDC_START, m_nCurrent);
	DDX_Text(pDX, IDC_END, m_nEnd);
	DDX_Control(pDX, IDC_ANI_LIST, m_AniListCtrl);
	DDX_Control(pDX, IDC_TRACK, m_TrackListCtrl);
	DDX_Text(pDX, IDC_NEW_ANI_NAME, m_AniName);
	DDX_Text(pDX, IDC_FRAME_TIME, m_FrameNum);
}


BEGIN_MESSAGE_MAP(CMainProcessDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OPENFILE, &CMainProcessDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_LOADASE, &CMainProcessDlg::OnBnClickedLoadase)
	ON_NOTIFY(NM_CLICK, IDC_ASE_LIST, &CMainProcessDlg::OnClickAseListItem)
	ON_BN_CLICKED(IDC_FOLDER_OPEN, &CMainProcessDlg::OnBnClickedFolderOpen)
	ON_NOTIFY(NM_CLICK, IDC_ANI_LIST, &CMainProcessDlg::OnClickAniList)
	ON_BN_CLICKED(IDC_ANI_PLAY, &CMainProcessDlg::OnBnClickedAniPlay)
	ON_BN_CLICKED(IDC_ANI_CAPTURE, &CMainProcessDlg::OnBnClickedAniCapture)
	ON_BN_CLICKED(IDC_SAVEANI, &CMainProcessDlg::OnBnClickedSaveani)
END_MESSAGE_MAP()


// CMainProcessDlg message handlers


void CMainProcessDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here
	ITEMIDLIST	*pidlBrowse;
	TCHAR pszPathname[MAX_PATH];

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Folder");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	m_bSubFolder = TRUE;

	CString SelFolder;
	if (pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
		SelFolder.Format(_T("%s"), pszPathname);
		m_FileListCtrl.DeleteAllItems();
		ReadFile(SelFolder);
		SetDlgItemText(IDC_FILEPATH, SelFolder);
		//버튼상태
		m_bAniPlay = FALSE;
		SetDlgItemText(IDC_ANI_PLAY, _T("PLAY"));
	}
}

void CMainProcessDlg::ReadFile(CString SelFolder)
{
	UpdateData(TRUE);
	ReadFile(SelFolder, m_bSubFolder);

	//CString DispFrameNum;
	//DispFrameNum.Format(_T("(0,%d)"), m_nSequenceLength - 1);
	////SetDlgItemText(IDC_TOTAL_FRAME, DispFrameNum);
}

void CMainProcessDlg::ReadFile(CString SelFolder, bool bSubFolder)
{
	//bSubFolder = True : 폴더열기
	//bSubFolder = False : 파일열기

	if (bSubFolder)
	{
		CFileFind Finder;
		BOOL Continue;
		SHFILEINFO shInfo;

		CString Name;

		Continue = Finder.FindFile(SelFolder + _T("\\*.*"));

		CList<CString, CString &> listSubFolder;

		while (Continue)
		{
			Continue = Finder.FindNextFile();

			/*if (Finder.IsDirectory() || Finder.IsDots())
			{
			if (Finder.IsDirectory() && !Finder.IsDots())
			{
			listSubFolder.AddTail(Finder.GetFilePath());
			}

			continue;
			}*/

			if (Finder.IsDirectory())
			{
				Name = Finder.GetFileName();
				Name.MakeUpper();

				LV_ITEM lvItem;

				int nItemNum = m_FileListCtrl.GetItemCount();

				SHGetFileInfo(Finder.GetFilePath(), 0, &shInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_SYSICONINDEX);

				lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
				lvItem.iItem = nItemNum;
				lvItem.iSubItem = 0;
				lvItem.iImage = shInfo.iIcon;
				lvItem.pszText = Name.GetBuffer(Name.GetLength());
				m_FileListCtrl.InsertItem(&lvItem);

				m_FileListCtrl.SetItemText(nItemNum, 1, Finder.GetFilePath());
			}


		}

		//m_nEnd = m_FileListCtrl.GetItemCount() - 1;
		//m_nSequenceLength = m_FileListCtrl.GetItemCount();
		UpdateData(FALSE);
	}
	if (!bSubFolder) //return;
	{
		//애니매이션 읽기

		CFileFind Finder;
		BOOL Continue;
		SHFILEINFO shInfo;

		CString Name;

		Continue = Finder.FindFile(SelFolder + _T("\\*.*"));

		CList<CString, CString &> listSubFolder;

		while (Continue)
		{
			Continue = Finder.FindNextFile();

			if (Finder.IsDirectory() || Finder.IsDots())
			{
				if (Finder.IsDirectory() && !Finder.IsDots())
				{
					listSubFolder.AddTail(Finder.GetFilePath());
				}

				continue;
			}

			Name = Finder.GetFileName();
			Name.MakeUpper();

			//ASE
			if (Name.Right(4) != _T(".ASE"))
				continue;

			LV_ITEM lvItem;

			int nItemNum = m_AniListCtrl.GetItemCount();

			SHGetFileInfo(Finder.GetFilePath(), 0, &shInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_SYSICONINDEX);

			lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 0;
			lvItem.iImage = shInfo.iIcon;
			lvItem.pszText = Name.GetBuffer(Name.GetLength());
			m_AniListCtrl.InsertItem(&lvItem);

			m_AniListCtrl.SetItemText(nItemNum, 1, Finder.GetFilePath());
		}

		m_nEnd = m_AniListCtrl.GetItemCount() - 1;
		m_nSequenceLength = m_AniListCtrl.GetItemCount();
		UpdateData(FALSE);
	}

	//POSITION pos = listSubFolder.GetHeadPosition();

	//if (!pos) return;

	//while (pos)
	//{
	//	CString SubFolder = listSubFolder.GetNext(pos);
	//	ReadFile(SubFolder, bSubFolder);
	//}
}

BOOL CMainProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LV_COLUMN lvColumn;
	SHFILEINFO shInfo;
	CRect FileListRect;
	HIMAGELIST hImageList = (HIMAGELIST)SHGetFileInfo(_T("C:\\"), 0, &shInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	m_ImageList.Attach(hImageList);

	///파일리스트 초기화
	m_FileListCtrl.GetClientRect(&FileListRect);

	TCHAR *list = _T("File name");
	int nWidth;
	nWidth = FileListRect.Width() - 55;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = list;
	lvColumn.iSubItem = 0;
	lvColumn.cx = nWidth;
	m_FileListCtrl.InsertColumn(0, &lvColumn);
	m_FileListCtrl.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 200, 0);

	long lSetStyle;
	lSetStyle = GetWindowLong(m_FileListCtrl.m_hWnd, GWL_STYLE);
	lSetStyle &= ~LVS_TYPEMASK;
	lSetStyle |= LVS_REPORT;
	SetWindowLong(m_FileListCtrl.m_hWnd, GWL_STYLE, lSetStyle);

	m_FileListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_FileListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_FileListCtrl.SetBkColor(RGB(255, 255, 255));
	m_FileListCtrl.SetTextBkColor(RGB(255, 255, 200));
	m_FileListCtrl.SetTextColor(RGB(100, 20, 20));

	//애니메이션리스트 초기화
	m_AniListCtrl.GetClientRect(&FileListRect);

	list = _T("Animation name");
	nWidth = FileListRect.Width() - 55;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = list;
	lvColumn.iSubItem = 0;
	lvColumn.cx = nWidth;
	m_AniListCtrl.InsertColumn(0, &lvColumn);
	m_AniListCtrl.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 200, 0);

	lSetStyle = GetWindowLong(m_AniListCtrl.m_hWnd, GWL_STYLE);
	lSetStyle &= ~LVS_TYPEMASK;
	lSetStyle |= LVS_REPORT;
	SetWindowLong(m_AniListCtrl.m_hWnd, GWL_STYLE, lSetStyle);

	m_AniListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_AniListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_AniListCtrl.SetBkColor(RGB(255, 255, 255));
	m_AniListCtrl.SetTextBkColor(RGB(255, 255, 200));
	m_AniListCtrl.SetTextColor(RGB(100, 20, 20));

	///파일리스트 초기화
	m_TrackListCtrl.GetClientRect(&FileListRect);

	list = _T("No.");
	nWidth = FileListRect.Width() - 210;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = list;
	lvColumn.iSubItem = 0;
	lvColumn.cx = nWidth;
	m_TrackListCtrl.InsertColumn(0, &lvColumn);
	m_TrackListCtrl.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 40, 0);
	m_TrackListCtrl.InsertColumn(2, _T("X"), LVCFMT_LEFT, 30, 0);
	m_TrackListCtrl.InsertColumn(3, _T("Y"), LVCFMT_LEFT, 30, 0);
	m_TrackListCtrl.InsertColumn(4, _T("Z"), LVCFMT_LEFT, 30, 0);
	m_TrackListCtrl.InsertColumn(5, _T("W"), LVCFMT_LEFT, 30, 0);

	lSetStyle = GetWindowLong(m_TrackListCtrl.m_hWnd, GWL_STYLE);
	lSetStyle &= ~LVS_TYPEMASK;
	lSetStyle |= LVS_REPORT;
	SetWindowLong(m_TrackListCtrl.m_hWnd, GWL_STYLE, lSetStyle);

	m_TrackListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_TrackListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_TrackListCtrl.SetBkColor(RGB(255, 255, 255));
	m_TrackListCtrl.SetTextBkColor(RGB(255, 255, 200));
	m_TrackListCtrl.SetTextColor(RGB(100, 20, 20));
	/*
	m_CurrentImageRed = cmatrix(m_nMH, m_nMW);
	m_CurrentImageGreen = cmatrix(m_nMH, m_nMW);
	m_CurrentImageBlue = cmatrix(m_nMH, m_nMW);

	m_SleepCtrl.SetRange(0, 1000);

	m_SeqTypeCtrl.InsertString(0, _T("*.*"));
	m_SeqTypeCtrl.InsertString(1, _T("BMP"));
	m_SeqTypeCtrl.InsertString(2, _T("JPG"));
	m_SeqTypeCtrl.InsertString(3, _T("PCX"));
	m_SeqTypeCtrl.InsertString(4, _T("256"));
	m_SeqTypeCtrl.InsertString(5, _T("320"));
	m_SeqTypeCtrl.InsertString(6, _T("640"));
	m_SeqTypeCtrl.InsertString(7, _T("BMP+JPG"));
	m_SeqTypeCtrl.InsertString(8, _T("Images"));

	m_SeqTypeCtrl.SetCurSel(8);*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ASE FILE LOAD
void CMainProcessDlg::OnBnClickedLoadase()
{
	// TODO: Add your control notification handler code here

	//버튼상태
	m_bAniPlay = FALSE;
	SetDlgItemText(IDC_ANI_PLAY, _T("PLAY"));

	m_bAseLoad = TRUE;

	m_AniListCtrl.SetItemState(m_nCurrent, LVIS_SELECTED, LVIS_SELECTED);
	m_AniListCtrl.EnsureVisible(m_nCurrent, FALSE);

	CString FileName = m_AniListCtrl.GetItemText(m_nCurrent, 1);
	FileName.MakeUpper();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	CCG_AnimationView *pView = (CCG_AnimationView *)pChild->GetActiveView();

	std::string str = FileName;
	pView->OnLoadASE(str);

	//?? 파일경로 넘겨줘야함
	//?? animation 갯수랑 이름 받아서 리스트에 출력해야함
	// 벡터로 주면 리스트에 추가하기
}


void CMainProcessDlg::OnClickAseListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nCurrent = pNMListView->iItem;//


									//int idx = pNMListView -> iItem;// 선택된 아이템값의 아이템을 (0,1 ... n 번째 인덱스) 한개 가져온다.
									//CString sIndexValue;sIndexValue = m_listDataTable.GetItemText(idx, 1);
	UpdateData(FALSE);

	*pResult = 0;
}


void CMainProcessDlg::OnBnClickedFolderOpen()
{
	// TODO: Add your control notification handler code here
	m_FileListCtrl.SetItemState(m_nCurrent, LVIS_SELECTED, LVIS_SELECTED);
	m_FileListCtrl.EnsureVisible(m_nCurrent, FALSE);

	CString FileName = m_FileListCtrl.GetItemText(m_nCurrent, 1);
	FileName.MakeUpper();

	//AfxMessageBox(FileName);

	m_bSubFolder = FALSE;
	m_AniListCtrl.DeleteAllItems();
	ReadFile(FileName);
	//?? 파일경로 넘겨줘야함
	//?? animation 갯수랑 이름 받아서 리스트에 출력해야함
	// 벡터로 주면 리스트에 추가하기

}


void CMainProcessDlg::OnClickAniList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nCurrent = pNMListView->iItem;//
									//int idx = pNMListView -> iItem;// 선택된 아이템값의 아이템을 (0,1 ... n 번째 인덱스) 한개 가져온다.
									//CString sIndexValue;sIndexValue = m_listDataTable.GetItemText(idx, 1);
	UpdateData(FALSE);
	*pResult = 0;
}


void CMainProcessDlg::OnBnClickedAniPlay()
{
	// TODO: Add your control notification handler code here
	//눌르면 재생, 글씨 PAUSE 로 바꾸고
	//SPACE BAR



	if (!m_bAseLoad)
	{
		CString str;
		str.Format(_T("%s"), "로드안댐");
		AfxMessageBox(str);
		return;
	}

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	CCG_AnimationView *pView = (CCG_AnimationView *)pChild->GetActiveView();
	pView->OnPlayPause();
	//재생
	if (m_bAniPlay == FALSE)
	{
		m_bAniPlay = TRUE;
		SetDlgItemText(IDC_ANI_PLAY, _T("PAUSE"));
	}
	//정지
	else
	{
		m_bAniPlay = FALSE;
		SetDlgItemText(IDC_ANI_PLAY, _T("PLAY"));
	}
}


void CMainProcessDlg::OnBnClickedAniCapture()
{
	// TODO: Add your control notification handler code here
	if (!m_bAseLoad)
	{
		return;
	}

	UpdateData(TRUE); // 파일 이름 읽어오는 것

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	CCG_AnimationView *pView = (CCG_AnimationView *)pChild->GetActiveView();

	if (m_AniName == "")
	{
		CString str;
		str.Format(_T("%s"), "애니 이름 요구");
		AfxMessageBox(str);
		return;
	}
	else if (m_FrameNum == "")
	{
		CString str;
		str.Format(_T("%s"), "애니 이름 요구");
		AfxMessageBox(str);
		return;
	}

	pView->OnCapture(atoi((std::string(m_FrameNum)).c_str()),
		(std::string)m_AniName);
}


void CMainProcessDlg::OnBnClickedSaveani()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	CCG_AnimationView *pView = (CCG_AnimationView *)pChild->GetActiveView();

	UpdateData(TRUE);
	if (m_AniName == "")
	{
		CString str;
		str.Format(_T("%s"), "애니 이름 요구");
		AfxMessageBox(str);
		return;
	}
	pView->OnSaveASE((std::string)m_AniName);
}
