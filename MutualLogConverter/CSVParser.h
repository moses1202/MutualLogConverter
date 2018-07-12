#pragma once

// CSVParser.h : header file
//

#include "Common.h"
#include "StrTrim.h"
#include "CSVWriter.h"
#include "XLSXWriter.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


class CSVParser : public CDialog
{
public:
	CSVParser();
	~CSVParser();
	BOOL ParseFile(CString path, UINT mode);
	void SetFolderPath(CString folder_path);
	void TerminateParser();
private:
	UINT write_mode;
	CString m_FolderPath;
	CSVWriter *m_pCSVWriter;
	XLSXWriter *m_pXSLXWriter;
	BOOL m_TerminateSignal;

	BOOL StringCompare(wstring src, wstring cmp);
	BOOL IntegerCompare(wstring src, wstring cmp);
	BOOL SetFile(CString path);
	void InsertNewLine();
	void InsertCellAtBack(const wchar_t* content);
	void InsertCellAtBack(wstring content);
	void InsertData(wstring content, BOOL new_line, UINT num_copy);
};
