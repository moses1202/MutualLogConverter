
// MutualLogConverterDlg.h : header file
//

#include "Common.h"
#include "CSVParser.h"

#include <vector>
#include <iostream>
using namespace std;

#pragma once


// CMutualLogConverterDlg dialog
class CMutualLogConverterDlg : public CDialog
{
// Construction
public:
	CMutualLogConverterDlg(CWnd* pParent = NULL);	// standard constructor
	~CMutualLogConverterDlg();	// standard destrcotor

// Dialog Data
	enum { IDD = IDD_MUTUALLOGCONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString m_FolderPath;
	vector<CString> m_FileList;
	CSVParser *m_pCSVParser;

	BOOL m_ShutdownSignal;
	BOOL m_IsConvertThreadRunning;
	CWinThread* m_pTerminateThread;
	CWinThread* m_pConvertThread;
	CProgressCtrl* m_pCtrl;

	// Regular functions
	CString GetFolderPath(CString filepath, CString filename);
	vector<CString> GetFileList(CString path, CString filter);
	void InsertMsg(CString msg);
	
	static UINT TerminateThreadWrap(LPVOID pParam);
	void TerminateThread(void);
	static UINT ConvertThreadWrap(LPVOID pParam);
	void ConvertThread(void);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg LRESULT OnUpdateProgressBar(WPARAM wParam , LPARAM lParam);
	afx_msg LRESULT OnReceiveErrorCode(WPARAM wParam , LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_CheckBatchMode;

	afx_msg void OnBnClickedBtnConvert();
	afx_msg void OnBnClickedBtnFile();
	afx_msg void OnBnClickedCheckBatch();
	afx_msg void OnBnClickedBtnAbout();
};
