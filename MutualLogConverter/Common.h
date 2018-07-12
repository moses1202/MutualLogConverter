#pragma once

// Languge Selection: Defined in Project's Properties
#ifdef TSTLANG_JAPANESE
	#include "StringJpn.h"
#elif TSTLANG_CHINESE
	#include "StringChi.h"
#else
	#include "StringEng.h"
#endif

//プロジェクト内共通の定義値を記述します
// Common definition of project

// App defined message code
#define WM_APP_SEND_ERROR_CODE		(WM_APP + 11)
#define WM_APP_PROGRESS_UPDATE		(WM_APP + 12)

// Error code
#define XLS_OLE_INIT_FAILED		(0x0000)
#define XLS_CREATE_APP_FAILED	(0x0001)
#define XLS_CREATE_FILE_FAILED	(0x0002)

// Write mode code
#define NO_WRITE	(0x1000)
#define CSV_WRITE	(0x1001)
#define XLSX_WRITE	(0x1002)
#define XLS_WRITE	(0x1003)

// Common constant variables
#define STR_VAR_CSV_EXT			_T(".csv")
#define STR_VAR_CSV_EXT_FILTER	_T("*.csv")
#define STR_VAR_XLSX_EXT		_T(".xlsx")
#define STR_VAR_XLSX_EXT_FILTER	_T("*.xlsx")
#define STR_VAR_XLS_EXT			_T(".xls")
#define STR_VAR_XLS_EXT_FILTER	_T("*.xls")
#define STR_VAR_CSV_POST_EXT	_T("_Res.csv")
#define STR_VAR_NEW_LINE		_T("\r\n")
#define STR_VAR_OUT_FOLDER		_T("\\res\\")
