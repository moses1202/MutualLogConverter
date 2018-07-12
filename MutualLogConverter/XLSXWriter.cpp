
// XSLXWriter.cpp : implementation file
//

#include "stdafx.h"
#include "MutualLogConverter.h"
#include "XLSXWriter.h"

#define COV	covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional

XLSXWriter::XLSXWriter()
{
	locale::global(std::locale(""));

	Init();
}

XLSXWriter::~XLSXWriter()
{
}

BOOL XLSXWriter::Init()
{
	if (!AfxOleInit()) {
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		::PostMessage(theApp.dlg->GetSafeHwnd(), WM_APP_SEND_ERROR_CODE, XLS_OLE_INIT_FAILED, 0);
		return FALSE;
	}
	if (::CoInitialize( NULL ) == E_INVALIDARG) {
		::PostMessage(theApp.dlg->GetSafeHwnd(), WM_APP_SEND_ERROR_CODE, XLS_OLE_INIT_FAILED, 0);
		return FALSE;
	}
	if(!app.CreateDispatch(_T("Excel.Application"))) {
		//this->MessageBox(L"Create excel app failed!");
		::PostMessage(theApp.dlg->GetSafeHwnd(), WM_APP_SEND_ERROR_CODE, XLS_CREATE_APP_FAILED, 0);
		return FALSE;
	}
	
	return TRUE;
}

BOOL XLSXWriter::SetFile(wstring path)
{
	try {
		covOptional = COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		books = app.get_Workbooks();
		lpDisp = books.Open(CString(path.c_str()), COV);
		book.AttachDispatch(lpDisp);
		sheets = book.get_Worksheets();
		sheet = sheets.get_Item(COleVariant((short)1));
	}
	catch (COleDispatchException *e) {
		TRACE(e->m_strDescription);
	}

	return TRUE;
}

BOOL XLSXWriter::CloseFile()
{
	return TRUE;
}

void XLSXWriter::InsertData(wstring content, BOOL new_line, UINT num_copy)
{
}
