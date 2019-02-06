
// CSVWriter.cpp : implementation file
//

#include "stdafx.h"
#include "CSVWriter.h"

CSVWriter::CSVWriter()
{
	locale::global(locale(""));
}

CSVWriter::~CSVWriter()
{
	// Call again to prevent memory leak (?)
	locale::global(locale("C"));
}

BOOL CSVWriter::IfFileValid(wstring path)
{
	m_oFile.open(path, ios_base::out | ios_base::in);	// Will not create file
	
	if(m_oFile.is_open() == true) {
		m_oFile.close();
		return FALSE;
	}

	return TRUE;
}

BOOL CSVWriter::SetFile(wstring path)
{
	m_oFile.open(path, ios::out | ios::trunc);	// Write and overwrite
	
	if(m_oFile.is_open() == false)
		return FALSE;

	return TRUE;
}

BOOL CSVWriter::CloseFile()
{
	m_oFile.close();
	if(m_oFile.is_open() == true)
		return FALSE;

	return TRUE;
}

void CSVWriter::InsertData(wstring content, BOOL new_line, UINT num_copy)
{
	if(m_oFile.is_open() == false)
		return;

	if(new_line == TRUE)
		m_oFile << endl;

	for(UINT i = 0; i < num_copy; i++) {
		m_oFile << content << L",";
	}
}
