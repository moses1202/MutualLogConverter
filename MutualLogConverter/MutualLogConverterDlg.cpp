
// MutualLogConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MutualLogConverter.h"
#include "MutualLogConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMutualLogConverterDlg dialog



CMutualLogConverterDlg::CMutualLogConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMutualLogConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Add extra preparation here
	m_CheckBatchMode = FALSE;
}

CMutualLogConverterDlg::~CMutualLogConverterDlg()
{
}

void CMutualLogConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Check(pDX, IDC_CHECK_BATCH, m_CheckBatchMode);
}

BEGIN_MESSAGE_MAP(CMutualLogConverterDlg, CDialog)
	ON_MESSAGE(WM_APP_PROGRESS_UPDATE, &CMutualLogConverterDlg::OnUpdateProgressBar)
	ON_MESSAGE(WM_APP_SEND_ERROR_CODE, &CMutualLogConverterDlg::OnReceiveErrorCode)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONVERT, &CMutualLogConverterDlg::OnBnClickedBtnConvert)
	ON_BN_CLICKED(IDC_BTN_FILE, &CMutualLogConverterDlg::OnBnClickedBtnFile)
	ON_BN_CLICKED(IDC_CHECK_BATCH, &CMutualLogConverterDlg::OnBnClickedCheckBatch)
	ON_BN_CLICKED(IDC_BTN_ABOUT, &CMutualLogConverterDlg::OnBnClickedBtnAbout)
END_MESSAGE_MAP()


// CMutualLogConverterDlg message handlers

void CMutualLogConverterDlg::OnClose()
{
	if(m_pConvertThread) {
		// Set shutdown signal and wait until the thread finish
		InsertMsg(_T("Send shutdown signal."));
		m_ShutdownSignal = TRUE;
		//m_pCSVParser->TerminateParser();
		m_pTerminateThread = AfxBeginThread(TerminateThreadWrap, (LPVOID)this);
	}
	else {
		// Close termination thread
		if(m_pTerminateThread) {
			WaitForSingleObject( m_pTerminateThread->m_hThread, INFINITE );
			delete m_pTerminateThread;
			m_pTerminateThread = NULL;
		}

		// Memory recycle
		if(m_pCSVParser) delete m_pCSVParser;
		//if(m_pCtrl) delete m_pCtrl;

		// Messages
		CDialog::OnCancel();
		CDialog::OnClose();
	}
}

void CMutualLogConverterDlg::OnCancel()
{
	// Do nothing here,
	//  since ESC key would trigger this event
	//  and shut down the program.
	//InsertMsg(_T("Press ESC would not cause the program exit. HAHAHA..."));
}

BOOL CMutualLogConverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_pCSVParser = new CSVParser();
	m_FolderPath = _T("");
	m_pConvertThread = NULL;
	m_pTerminateThread = NULL;
	m_ShutdownSignal = FALSE;
	m_IsConvertThreadRunning = FALSE;
	m_pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROG_FILE);
	m_pCtrl->SetRange(0, 100);

	InsertMsg(STR_GUI_MSG_INIT_COMPLETE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
	
LRESULT CMutualLogConverterDlg::OnReceiveErrorCode(WPARAM wParam , LPARAM lParam)
{
	UINT error_code = wParam;

	switch(error_code) {
	case XLS_CREATE_APP_FAILED:
		MessageBox(L"Create excel app failed!");
		break;
	case XLS_CREATE_FILE_FAILED:
		MessageBox(L"Create excel file failed!");
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CMutualLogConverterDlg::OnUpdateProgressBar(WPARAM wParam , LPARAM lParam)
{
	UINT progress = wParam;

	m_pCtrl->SetPos(progress);

	return 0;
}

void CMutualLogConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMutualLogConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMutualLogConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CMutualLogConverterDlg::TerminateThreadWrap(LPVOID pParam){ ((CMutualLogConverterDlg*)pParam)->TerminateThread(); ExitThread(0); return 0; }
void CMutualLogConverterDlg::TerminateThread(void)
{
	// Wait and close the thread
	if(m_pConvertThread) {
		WaitForSingleObject( m_pConvertThread->m_hThread, INFINITE );
		delete m_pConvertThread;
		m_pConvertThread = NULL;
	}
	
	::PostMessage(this->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	ExitThread(0);
}

UINT CMutualLogConverterDlg::ConvertThreadWrap(LPVOID pParam){ ((CMutualLogConverterDlg*)pParam)->ConvertThread(); ExitThread(0); return 0; }
void CMutualLogConverterDlg::ConvertThread(void)
{
	m_IsConvertThreadRunning = TRUE;
	SetDlgItemText(IDC_BTN_CONVERT, STR_GUI_BTN_CANCEL);
	GetDlgItem(IDC_CHECK_BATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FILE)->EnableWindow(FALSE);

	// Start parsing loop
	for(UINT i = 0; i < m_FileList.size() && (m_ShutdownSignal == FALSE); i++) {
		m_pCtrl->SetPos(0); // Reset progress bar
		
		InsertMsg(L"Parsing file: " + m_FileList[i].Right(m_FileList[i].GetLength() - m_FolderPath.GetLength()));
		if(m_pCSVParser->ParseFile(m_FileList[i], CSV_WRITE) == FALSE)
			InsertMsg(L"Failed to open file: " + m_FileList[i]);
		
		m_pCtrl->SetPos(100); // Fill progress bar
	}

	// Finish message
	InsertMsg(STR_GUI_MSG_COMPLETE);
	m_pCtrl->SetPos(100);
	
	GetDlgItem(IDC_BTN_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_BATCH)->EnableWindow(TRUE);
	SetDlgItemText(IDC_BTN_CONVERT, STR_GUI_BTN_CONVERT);
	m_IsConvertThreadRunning = FALSE;
}


void CMutualLogConverterDlg::OnBnClickedBtnConvert()
{
	// TODO: Add your control notification handler code here
	if(m_FileList.empty())
		return;
	if(m_IsConvertThreadRunning == TRUE) {
		m_ShutdownSignal = TRUE;
		m_pCSVParser->TerminateParser();
		InsertMsg(STR_GUI_MSG_ABORTED);
		return;
	}
	
	// Check if thread alive
	if(m_pConvertThread) {
		DWORD exit_code = 0;
		//InsertMsg(L"Wait for thread.");
		WaitForSingleObject( m_pConvertThread->m_hThread, INFINITE );
		GetExitCodeThread(m_pConvertThread, &exit_code);
		delete m_pConvertThread;
		m_pConvertThread = NULL;
	}
	// Start thread
	m_ShutdownSignal = FALSE;
	m_pConvertThread = AfxBeginThread(ConvertThreadWrap, (LPVOID)this);
}


void CMutualLogConverterDlg::OnBnClickedBtnFile()
{
	// TODO: Add your control notification handler code here
	UINT wk_opt;

	// Get file path by dailog
	wk_opt = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CFileDialog	dlg(TRUE, _T("*.*"), NULL, wk_opt, _T("Log File(*.csv)|*.csv"), this);
	if(dlg.DoModal() != IDOK)
		return;
	
	m_FileList.clear();
	m_FileList.push_back(dlg.GetPathName());
	m_FolderPath = GetFolderPath(dlg.GetPathName(), dlg.GetFileName());
	m_pCSVParser->SetFolderPath(m_FolderPath);

	SetDlgItemText(IDC_EDIT_PATH,  dlg.GetPathName());

	// If batch mode is ON, parse the directory where the file locates
	UpdateData();
	if(m_CheckBatchMode == TRUE) {
		m_FileList = GetFileList(m_FolderPath, _T("*.csv"));
	}
}

CString CMutualLogConverterDlg::GetFolderPath(CString filepath, CString filename)
{
	return filepath.Left(filepath.GetLength() - filename.GetLength());
}

vector<CString> CMutualLogConverterDlg::GetFileList(CString path, CString filter)
{
    CFileFind finder;
	vector<CString> l_fileList;
	CString file_name;

    BOOL bWorking = finder.FindFile(path + filter);
    while (bWorking) {
        bWorking = finder.FindNextFile();
		file_name = finder.GetFileName();
		if (!finder.IsDots() && file_name.Right(CString(_T("_Res.csv")).GetLength()) != _T("_Res.csv")) {
			l_fileList.push_back(path + file_name);
        }
    }

	return l_fileList;
}

void CMutualLogConverterDlg::InsertMsg(CString msg)
{
	CString tmp;
	CEdit* edb = (CEdit*)GetDlgItem(IDC_EDIT_MSG);

	GetDlgItemText(IDC_EDIT_MSG, tmp);
	//SetDlgItemText(IDC_EDIT_MSG, tmp + _T("\r\n") + msg);
	edb->SetWindowTextW(tmp + msg + _T("\r\n"));
	edb->SetFocus();
	edb->SetSel(-1);
}

void CMutualLogConverterDlg::OnBnClickedCheckBatch()
{
	// TODO: Add your control notification handler code here
	// If batch mode is ON, parse the directory where the file locates
	UpdateData();
	if(m_CheckBatchMode == TRUE) {
		m_FileList = GetFileList(m_FolderPath, _T("*.csv"));
	}
	else {
		CString tmp;
		m_FileList.clear();
		GetDlgItemText(IDC_EDIT_PATH, tmp);
		m_FileList.push_back(tmp);
	}
}


void CMutualLogConverterDlg::OnBnClickedBtnAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
