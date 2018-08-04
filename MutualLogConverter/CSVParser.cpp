
// CSVParser.cpp : implementation file
//

#include "stdafx.h"
#include "MutualLogConverter.h"
#include "CSVParser.h"

CSVParser::CSVParser()
{
	write_mode = NO_WRITE;
	m_FolderPath = _T("");
	m_pCSVWriter = new CSVWriter();
	m_pXSLXWriter = new XLSXWriter();
}

CSVParser::~CSVParser()
{
	// Memory recycle
	delete m_pCSVWriter;
	delete m_pXSLXWriter;
}

void CSVParser::TerminateParser()
{
	m_TerminateSignal = TRUE;
}

BOOL CSVParser::ParseFile(CString path, UINT mode)
{
	CStdioFile l_csvFile;
	CFileException l_csvFile_exp;
	UINT wk_opt;
	BOOL ret;
	CString read_str;
	wstring l_wstr;
	wstring date, time, len, s_pnt_x, s_pnt_y, direc;
	wstring tmp;
	ULONG64 file_size = 0;
	ULONG64 parsed_len = 0;
	INT x_width = 0;
	BOOL read_first_line = FALSE;
	
	// Open file for read only
	wk_opt = CFile::modeRead | CFile::typeText ;
	ret = l_csvFile.Open(path, wk_opt, &l_csvFile_exp);
	if(ret == FALSE) {
		TRACE(_T("File could not be opened, cause = %d\n"), l_csvFile_exp.m_cause);
		return FALSE;
	}
	//ret = m_pCSVWriter->SetFile((path.Left(path.GetLength() - CString(L".csv").GetLength()) + L"_Res.csv").GetString());
	
	// Open file
	write_mode = mode;
	ret = SetFile(path);

	file_size = l_csvFile.GetLength();

	// Initialize parameter
	m_TerminateSignal = FALSE;
	
	// Start the loop for reading width of X axis
	while(l_csvFile.ReadString(read_str) && (m_TerminateSignal == FALSE)) {
		// Handle them in wstring
		wstringstream wss(read_str.GetString());

		/****** From here, handle all strings in wstring or wchar_t* insdead of CString  ******/
		
		// Date
		if(getline(wss, date, L',') == false || date == L"Date")
			continue;
		// Time
		getline(wss, time, L',');
		// Length
		getline(wss, len, L',');
		// Start Point X
		getline(wss, s_pnt_x, L',');
		// Start Point Y
		getline(wss, s_pnt_y, L',');
		// Direction
		getline(wss, direc, L',');

		if(!StringCompare(s_pnt_x, L"0")) {
			if(x_width < stoi(len) + stoi(s_pnt_x)) {
				x_width = stoi(len) + stoi(s_pnt_x);
			}
		}
		else {
			if(read_first_line == FALSE) {
				read_first_line = TRUE;
				x_width = stoi(len) + stoi(s_pnt_x);
			}
			else
				break;
		}
	}

	// Set the file pointer back to beginning
	l_csvFile.SeekToBegin();

	// Start the loop for pasring file
	while(l_csvFile.ReadString(read_str) && (m_TerminateSignal == FALSE)) {
		// Handle them in wstring
		wstringstream wss(read_str.GetString());

		/****** From here, handle all strings in wstring or wchar_t* insdead of CString  ******/

		// Update current read size of file
		parsed_len += read_str.GetLength() * sizeof(char);

		// Date
		if(getline(wss, date, L',') == false || date == L"Date")
			continue;
		// Time
		getline(wss, time, L',');
		// Length
		getline(wss, len, L',');
		// Start Point X
		getline(wss, s_pnt_x, L',');
		// Start Point Y
		getline(wss, s_pnt_y, L',');
		// Direction
		getline(wss, direc, L',');

		if(StringCompare(s_pnt_x, L"0")) {
			// Insert new line when met X00
			InsertNewLine();
			tmp = L"";
			if(StringCompare(direc, L"X Axis")) {
				if(StringCompare(s_pnt_y, L"0")) {
					// Insert new frame header when met X00, Y00
					InsertNewLine();
					InsertNewLine();
					InsertCellAtBack(L"");
					for(int i = 0; i < x_width; i++)
						InsertCellAtBack(L"RX"+to_wstring(i));
					InsertNewLine();
				}
				tmp += L"TX";
			}
			else {
				if(StringCompare(s_pnt_y, L"0")) {
					// Insert new frame header when met X00, Y00
					InsertNewLine();
					InsertNewLine();
					InsertCellAtBack(L"");
					for(int i = 0; i < x_width; i++)
						InsertCellAtBack(L"TX"+to_wstring(i));
					InsertNewLine();
				}
				tmp += L"RX";
			}
			tmp += s_pnt_y;
			// Insert new line header
			InsertCellAtBack(tmp);
		}

		// Data
		while(getline(wss, l_wstr, L',')) {
			InsertCellAtBack(l_wstr);
		}
		
		// Trigger message for updating progress bar
		::PostMessage(theApp.dlg->GetSafeHwnd(), WM_APP_PROGRESS_UPDATE, (UINT)((parsed_len * 100) / file_size), 0);
	}

	// Close file
	l_csvFile.Close();
	m_pCSVWriter->CloseFile();
	
	return TRUE;
}

BOOL CSVParser::SetFile(CString path)
{
	BOOL ret = FALSE;

	switch(write_mode) {
	case CSV_WRITE:
		ret = m_pCSVWriter->SetFile((path.Left(path.GetLength() - CString(L".csv").GetLength()) + L"_Res.csv").GetString());
		break;
	case XLSX_WRITE:
		ret = m_pXSLXWriter->SetFile((path.Left(path.GetLength() - CString(L".csv").GetLength()) + L"_Res.xslx").GetString());
		break;
	default:
		break;
	}

	return ret;
}

BOOL CSVParser::StringCompare(wstring src, wstring cmp)
{
	trim(src);
	return (src == cmp);
}

BOOL CSVParser::IntegerCompare(wstring src, wstring cmp)
{
	trim(src);
	return (src == cmp);
}

void CSVParser::SetFolderPath(CString folder_path)
{
	m_FolderPath = folder_path;
}

void CSVParser::InsertCellAtBack(const wchar_t* content)
{
	InsertCellAtBack(wstring(content));
}

void CSVParser::InsertCellAtBack(wstring content)
{
	InsertData(content, FALSE, 1);
}

void CSVParser::InsertNewLine()
{
	InsertData(L"", TRUE, 0);
}

void CSVParser::InsertData(wstring content, BOOL new_line, UINT num_copy)
{
	switch(write_mode) {
	case CSV_WRITE:
		m_pCSVWriter->InsertData(content, new_line, num_copy);
		break;
	case XLSX_WRITE:
		m_pXSLXWriter->InsertData(content, new_line, num_copy);
		break;
	default:
		break;
	}
}
