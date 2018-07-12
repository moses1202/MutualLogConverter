#pragma once

// XSLXWriter.h : header file
//

#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

#include <locale>
#include <fstream>
using namespace std;


class XLSXWriter : public CDialog
{
public:
	XLSXWriter();
	~XLSXWriter();
	BOOL Init();
	BOOL SetFile(wstring path);
	BOOL CloseFile();
	void InsertData(wstring content, BOOL new_line, UINT num_copy);
private:
	wofstream m_oFile;
	CApplication app; 
	COleVariant covOptional;
	CWorkbook book;  
	CWorkbooks books;  
	CWorksheet sheet;  
	CWorksheets sheets; 
	CRange range;  
	//CMyFont font; 
	CRange cols; 
	LPDISPATCH lpDisp;
};
