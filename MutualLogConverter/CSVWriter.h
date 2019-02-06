#pragma once

// CSVWriter.h : header file
//

#include <locale>
#include <fstream>
using namespace std;


class CSVWriter
{
public:
	CSVWriter();
	~CSVWriter();
	BOOL IfFileValid(wstring path);
	BOOL SetFile(wstring path);
	BOOL CloseFile();
	void InsertData(wstring content, BOOL new_line, UINT num_copy);
private:
	wofstream m_oFile;
};
