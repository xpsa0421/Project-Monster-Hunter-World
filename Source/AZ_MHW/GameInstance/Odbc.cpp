#include "Odbc.h"

void Odbc::Load()
{
	m_dbDataList.clear();
	m_ColumnList.clear();

	// 전체 레코드 조회, 추가, 수정, 삭제
	SQLRETURN hr = SQLExecute(g_hSelectAllStmt);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg(g_hSelectAllStmt);
		return;
	}
	SQLLEN  count; // update, insert, delete 사용가능함다.
	SQLRETURN ret = SQLRowCount(g_hSelectAllStmt, &count);

	SQLSMALLINT colCount;
	SQLNumResultCols(g_hSelectAllStmt, &colCount);

	TColDescription col;

	for (int iCol = 1; iCol < colCount + 1; iCol++)
	{
		col.icol = iCol;
		hr = SQLDescribeCol(g_hSelectAllStmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg(g_hSelectAllStmt);
			break;
		}
		m_ColumnList.push_back(col);
	}


	while (SQLFetch(g_hSelectAllStmt) != SQL_NO_DATA)
	{
		RECORD record;
		dbitem dtItem;
		for (int iCol = 0; iCol < m_ColumnList.size(); iCol++)
		{
			/* SQLBindCol 대체한다.
			   데이터형 상관없이 모든 것을 스트링으로 받겠다.*/
			hr = SQLGetData(g_hSelectAllStmt, m_ColumnList[iCol].icol,
				SQL_WCHAR, m_ColumnList[iCol].bindData,
				sizeof(m_ColumnList[iCol].bindData), NULL);
			if (hr == SQL_SUCCESS)
			{
				record.push_back(m_ColumnList[iCol].bindData);
			}
		}
		if (hr == SQL_SUCCESS)
		{
			m_dbDataList.push_back(record);
		}
	}
	if (g_hSelectAllStmt) SQLCloseCursor(g_hSelectAllStmt);

	SQLFreeStmt(g_hSelectAllStmt, SQL_CLOSE);
} // End Load

void Odbc::Init()
{
	// 환경핸들(g_hEnv), 연결핸들(g_hDbc), 명령핸들(g_hStmt)

	// SQLAllocHandle : 핸들의 할당 함수 
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_ENV, // 할당하고자하는 핸들 타입
		SQL_NULL_HANDLE, // 생성할 부모 핸들 지정
		&g_hEnv // 생성할 핸들의 주소
	);
	if (hr != SQL_SUCCESS)
	{
		return;
	}

	// SQLSetEnvAttr : 환경핸들 속성 지정
	if (SQLSetEnvAttr(g_hEnv, // 환경속성 핸들
		SQL_ATTR_ODBC_VERSION, // 속성의 종류 지정
		(SQLPOINTER)SQL_OV_ODBC3_80, //속성 값 지정
		SQL_IS_INTEGER) != SQL_SUCCESS // 속성 값의 길이
		)
	{
		ErrorMsg(NULL);
		return;
	}

	if (hr == SQL_SUCCESS)
	{
		//연결핸들(g_hDbc)
		// SQLAllocHandle : 핸들의 할당 함수
		hr = SQLAllocHandle(SQL_HANDLE_DBC, // 할당하고자하는 핸들 타입
			g_hEnv, // 생성할 부모 핸들 지정
			&g_hDbc // 생성할 핸들의 주소
		);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg(NULL);
			return;
		}
	}
} // End Init

bool Odbc::CreatePrepare()
{
	// Select All
	//SQLRETURN ret;
	CreateUserAllSelect();

	// Select where
	CreateSelectWhereName();

	// Insert
	//CreateInsertAccount();
	Signup();

	// Update
	CreateUpdate();

	// Delete
	DeleteLoingInfo();

	// Login
	LoginCheck();

	return true;
} // End CreatePrepare

void Odbc::ErrorMsg(SQLHSTMT  stmt)
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError = 0;

	//SQLRETURN hr;
	// 복합에러
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
		//단순에러
	SQLError(g_hEnv, g_hDbc, g_hStmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH + 1, &msgLen);
	{
		//_stprintf(errorMsg, L"SQLSTATE:%s, 진단정보:%s, 에러코드:%d", sqlState, msg, nativeError);
		UE_LOG(LogTemp, Warning, TEXT("SQLSTATE:%s, state info : %s, error code : %d\n"), sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"state info", 0);
	}
} // End ErrorMsg

void Odbc::Connect(std::wstring dbName)
{
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	//_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());
	UE_LOG(LogTemp, Warning, TEXT("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());

	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(NULL);
		return;
	}


	//명령핸들(g_hStmt)
	// SQLSetEnvAttr : 환경핸들 속성 지정
	SQLAllocHandle(SQL_HANDLE_STMT, // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hStmt // 생성할 핸들의 주소
	);

	CreatePrepare();
} // End Connect

void Odbc::ConnectMssql(std::wstring dbName)
{
	UE_LOG(LogTemp, Warning, TEXT("odbc connect"));
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	TCHAR  dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);

	TCHAR dsn[] = L"Driver={SQL Server};Server=shader.kr;Address=192.168.0.157,1433;Network=dbmssocn;Database=test_DB;Uid=sa;Pwd=dpsjwl2012;";
	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, dsn, _countof(dsn),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);

	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		// 로그인 실패
		UE_LOG(LogTemp, Warning, TEXT("sql fail / hr : %d"), hr);
		ErrorMsg(NULL);
		return;
	}
	else {
		// 로그인 성공
		UE_LOG(LogTemp, Warning, TEXT("sql seccess / hr : %d"), hr);
	}

	//명령핸들(g_hStmt)
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	CreatePrepare();
} // End ConnetMssql

void Odbc::DisConnect()
{
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	if (g_hSelectAllStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hSelectAllStmt);
	if (g_hInsertStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hInsertStmt);
	if (g_hUpdateStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hUpdateStmt);
	if (g_hDeleteStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hDeleteStmt);
	if (g_hLoginCheckStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hLoginCheckStmt);

	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
} // End DisConnect

//bool Odbc::AddSQL(dbitem& record)
//{
//	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
//	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));
//	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
//	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));
//
//	//m_iSelectLevel = record.level;
//
//	// SQLExecute : 준비된 파라미터만 교체해서 바로 실행한다
//	SQLRETURN hr = SQLExecute(g_hInsertStmt);
//	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
//	{
//		ErrorMsg(g_hInsertStmt);
//		return false;
//	}
//	if (g_hInsertStmt) SQLCloseCursor(g_hInsertStmt);
//	
//	// SQLFreeStmt : 핸들과 관련된 모든 자원을 해제
//	SQLFreeStmt(g_hInsertStmt, SQL_CLOSE);
//
//	return true;
//} // End AddSQL

bool Odbc::AddSQL(dbitem& record)
{
	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));
	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));

	//m_iSelectLevel = record.level;

	// SQLExecute : 준비된 파라미터만 교체해서 바로 실행한다
	SQLRETURN hr = SQLExecute(g_hSignupStmt);
	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(g_hSignupStmt);
		return false;
	}
	if (g_hSignupStmt) SQLCloseCursor(g_hSignupStmt);

	// SQLFreeStmt : 핸들과 관련된 모든 자원을 해제
	SQLFreeStmt(g_hSignupStmt, SQL_CLOSE);

	return true;
} // End AddSQL

bool Odbc::UpdateSQL(dbitem& record, std::wstring selectName)
{
	// 버퍼의 길이가 NCHAR(10) 보다 크면 안된다. 
	ZeroMemory(m_szSelectName, sizeof(TCHAR) * 10);
	CopyMemory(m_szSelectName, selectName.c_str(), selectName.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdateName, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdateName, record.name.c_str(), record.name.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdatePass, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdatePass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));


	m_iUpdateLevel = record.level;
	m_iUpdateSex = record.sex;

	m_iDataLength = sizeof(m_szUpdateName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));
	SWORD sReturn;
	SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sReturn,
		0, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);

	std::time_t now = std::time(NULL); // 1970,01,01, 0시
	//std::tm* ptm = std::localtime(&now);
	std::tm ptm;
	localtime_s(&ptm, &now);

	m_ts.year = ptm.tm_year + 1900;
	m_ts.month = ptm.tm_mon + 1;
	m_ts.day = ptm.tm_mday;
	m_ts.hour = ptm.tm_hour;
	m_ts.minute = ptm.tm_min;
	m_ts.second = ptm.tm_sec;
	m_ts.fraction = 0;

	SQLRETURN hr = SQLExecute(g_hUpdateStmt);
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(g_hUpdateStmt) != SQL_NO_DATA)
		{
			if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
			SQLFreeStmt(g_hUpdateStmt, SQL_RESET_PARAMS);
			SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
			return true;
		}
	}
	ErrorMsg(g_hUpdateStmt);
	if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
	SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
	SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
	return true;
} // End UpdateSQL

bool Odbc::ReadRecord(std::wstring selectName)
{
	ZeroMemory(retName, sizeof(TCHAR) * 10);
	ZeroMemory(retPass, sizeof(TCHAR) * 10);
	// 중의 : sizeof(TCHAR)*10 의 설정이 중요함.
	SQLRETURN hr1 = SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, sizeof(TCHAR) * 10, &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, sizeof(TCHAR) * 10, &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);
	SQLBindCol(g_hReadStmt, 8, SQL_TYPE_TIMESTAMP, &logoutTS, sizeof(logoutTS), &llogout);
	m_iDataLength = sizeof(m_szReadName);
	m_cbColumn = SQL_NTS;


	SQLRETURN ret = SQLBindParameter(g_hReadStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szReadName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	if (!selectName.empty())
	{
		ZeroMemory(m_szReadName, sizeof(TCHAR) * 10);
		CopyMemory(m_szReadName, selectName.c_str(), selectName.size() * sizeof(TCHAR));
		SQLRETURN hr = SQLExecute(g_hReadStmt);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (SQLFetch(g_hReadStmt) != SQL_NO_DATA)
			{
				if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
				SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
				SQLFreeStmt(g_hReadStmt, SQL_UNBIND);
				SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
				return true;
			}
		}
	}
	ErrorMsg(g_hReadStmt);
	if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
	SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
	return false;
}

bool Odbc::DeleteSQL(const TCHAR* szName)
{
	if (szName != nullptr)
	{
		ZeroMemory(m_szDeleteName, sizeof(m_szDeleteName));
		// TODO szName 왜 8바이트로 들어오는지 확인해야함 임시로 m_szDeleteName 사이즈 받는걸로 변경
		CopyMemory(m_szDeleteName, szName, sizeof(m_szDeleteName));
		//CopyMemory(m_szDeleteName, szName, sizeof(szName));

		// SQLExecute : 준비된 파라미터만 교체해서 바로 실행한다
		SQLRETURN hr = SQLExecute(g_hDeleteStmt);

		if (hr == SQL_SUCCESS)
		{
			// SQLFetch 결과 집합에서 바인딩된 데이터를 반환
			if (SQLFetch(g_hDeleteStmt) != SQL_NO_DATA)
			{
				if (g_hDeleteStmt) SQLCloseCursor(g_hDeleteStmt);
				SQLFreeStmt(g_hDeleteStmt, SQL_CLOSE);
				return true;
			}
		}
	}

	ErrorMsg(g_hDeleteStmt);
	if (g_hDeleteStmt) SQLCloseCursor(g_hDeleteStmt);
	SQLFreeStmt(g_hDeleteStmt, SQL_CLOSE);
	return false;

}

bool Odbc::Signup()
{
	// Login 로그인 체크
	SQLRETURN ret;
	//insert into dbo.user_table(Name, Pass) values('kim', 'dd');
	std::wstring sql7 = L"insert into user_table (Name, Pass) values(?,?)";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hSignupStmt // 생성할 핸들의 주소
	);

	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hSignupStmt, // Handle
		(SQLTCHAR*)sql7.c_str(), // 실행할 sql 문
		SQL_NTS // TextLength?
	);

	m_iLoginIdDataLength = sizeof(m_szInsertName);
	m_iLoginPwDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hSignupStmt, // 명령핸들
		1, // 파라미터 번호
		SQL_PARAM_INPUT, // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szInsertName, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hSignupStmt, // 명령핸들
		2, // 파라미터 번호
		SQL_PARAM_INPUT, // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iLoginPwDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szInsertPass, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hSignupStmt);
		return false;
	}


	return true;
}

bool Odbc::LoginCheckSQL(const TCHAR* szName, const TCHAR* szPw)
{
	UE_LOG(LogTemp, Warning, TEXT("LoginCheckSQL"));
	if (szName != nullptr && szPw != nullptr)
	{
		ZeroMemory(m_szLoginId, sizeof(m_szLoginId));
		CopyMemory(m_szLoginId, szName, sizeof(m_szLoginId));
		ZeroMemory(m_szLoginPw, sizeof(m_szLoginPw));
		CopyMemory(m_szLoginPw, szPw, sizeof(m_szLoginPw));

		// SQLExecute : 준비된 파라미터만 교체해서 바로 실행한다
		SQLRETURN hr = SQLExecute(g_hLoginCheckStmt);

		if (hr == SQL_SUCCESS)
		{
			// SQLFetch 결과 집합에서 바인딩된 데이터를 반환
			if (SQLFetch(g_hLoginCheckStmt) != SQL_NO_DATA)
			{
				if (g_hLoginCheckStmt) SQLCloseCursor(g_hLoginCheckStmt);
				SQLFreeStmt(g_hLoginCheckStmt, SQL_CLOSE);
				return true;
			}
		}
	}

	ErrorMsg(g_hLoginCheckStmt);
	if (g_hLoginCheckStmt) SQLCloseCursor(g_hLoginCheckStmt);
	SQLFreeStmt(g_hLoginCheckStmt, SQL_CLOSE);
	return false;
}

bool Odbc::CreateUserAllSelect()
{
	// select * from [table]
	SQLRETURN ret;
	std::wstring sql = L"{CALL usp_User_AllSelect}";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hSelectAllStmt // 생성할 핸들의 주소
	);
	ret = SQLPrepare(g_hSelectAllStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hSelectAllStmt);
		return false;
	}
	return true;
}

bool Odbc::CreateSelectWhereName()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{ ?=CALL usp_Select_WhereName(?)}";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hReadStmt // 생성할 핸들의 주소
	);
	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hReadStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hReadStmt, // 명령핸들
		1, // 파라미터 번호
		SQL_PARAM_OUTPUT, // 파라미터 용도
		SQL_C_SHORT, // 파라미터 데이터 타입(C형)
		SQL_INTEGER, // 파라미터 데이터 타입(SQL형)
		0, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		&sRet, // 실제 파라미터와 연결될 변수의 주소
		0, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, // 명령핸들
		2, // 파라미터 번호
		SQL_PARAM_INPUT,  // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szSelectName, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, _countof(retName), &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, _countof(retPass), &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);

	return true;
}

bool Odbc::CreateInsertAccount()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{?=CALL usp_InsertAccount(?,?)}";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hInsertStmt // 생성할 핸들의 주소
	);
	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hInsertStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_INTEGER,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	return true;
}

bool Odbc::CreateUpdate()
{
	SQLRETURN ret;
	std::wstring sql = L"{?=CALL usp_UpdateUserInfo(?,?,?,?,?)}";

	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hUpdateStmt // 생성할 핸들의 주소
	);
	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hUpdateStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	return true;
}

bool Odbc::DeleteLoingInfo()
{
	// Delete
	SQLRETURN ret;
	std::wstring sql5 = L"delete from user_table where Name=?";
	//std::wstring sql5 = L"select * from user_table where Name =?";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hDeleteStmt // 생성할 핸들의 주소
	);

	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hDeleteStmt, // Handle
		(SQLTCHAR*)sql5.c_str(), // 실행할 sql 문
		SQL_NTS // TextLength?
	);

	m_iDataLength = sizeof(m_szDeleteName);
	m_cbColumn = SQL_NTS;

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hDeleteStmt, // 명령핸들
		1, // 파라미터 번호
		SQL_PARAM_INPUT, // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szDeleteName, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}
	return true;
}

bool Odbc::LoginCheck()
{
	// Login 로그인 체크
	SQLRETURN ret;

	std::wstring sql6 = L"select * from user_table where Name =? and Pass=?";
	//std::wstring sql6 = L"select * from user_table where Name =?";
	// SQLAllocHandle : 핸들의 할당 함수
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // 할당하고자하는 핸들 타입
		g_hDbc, // 생성할 부모 핸들 지정
		&g_hLoginCheckStmt // 생성할 핸들의 주소
	);

	// SQLPrepare 파라미터만 바꿔서 sql 실행
	ret = SQLPrepare(g_hLoginCheckStmt, // Handle
		(SQLTCHAR*)sql6.c_str(), // 실행할 sql 문
		SQL_NTS // TextLength?
	);

	m_iLoginIdDataLength = sizeof(m_szLoginId);
	m_iLoginPwDataLength = sizeof(m_szLoginPw);
	m_cbColumn = SQL_NTS;

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hLoginCheckStmt, // 명령핸들
		1, // 파라미터 번호
		SQL_PARAM_INPUT, // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szLoginId, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	// ?를 파라미터 마커라고 하며, 실행중 바인딩된 변수값으로 대체 ? 만큼 SQLBindParameter 할당 해야 한다.
	ret = SQLBindParameter(g_hLoginCheckStmt, // 명령핸들
		2, // 파라미터 번호
		SQL_PARAM_INPUT, // 파라미터 용도
		SQL_UNICODE, // 파라미터 데이터 타입(C형)
		SQL_CHAR, // 파라미터 데이터 타입(SQL형)
		m_iLoginPwDataLength, // 파라미터 변수의 크기
		0, // 파라미터 변수의 자리수
		m_szLoginPw, // 실제 파라미터와 연결될 변수의 주소
		m_iDataLength, // 파라미터의 문자열이나 이진형일때 버퍼의 크기
		&m_cbColumn // 길이와 상태 인수
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hLoginCheckStmt);
		return false;
	}


	return true;
}