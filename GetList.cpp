// GetList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"

#include "MySQLCMD.h"

#include "GetList.h"
#include <sys/timeb.h>

// CGetList

IMPLEMENT_DYNAMIC(CGetList, CButton)

CGetList::CGetList()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CGetList::~CGetList()
{
}


BEGIN_MESSAGE_MAP(CGetList, CButton)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()



// CGetList 메시지 처리기입니다.



void CGetList::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CButton::OnSysCommand(nID, lParam);
}

int CGetList::T0167(void)
{

	// system time
	t0167InBlock	pckInBlock;
	CString csmsg;

	TCHAR			szTrNo[]		= _T0167;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	struct timeb start, end;
	int left;
	int aa = 0, bb = 0;

	memset(&start, 0, sizeof(start));
 	memset(&end, 0, sizeof(start));
	aa = g_iXingAPI.GetTRCountRequest(_T1532);
	bb = g_iXingAPI.GetTRCountLimit(_T1532);
	csmsg.Format("%s - %d - %d\n", _T0167,  aa, bb);
	TRACE(csmsg);
	// 데이터 전송

	ftime(&start);
	while(1){
		ftime(&end);
		left = (int)(end.time - start.time);

		if(left == 0){
			if( (end.millitm - start.millitm) >= 100)
			  break;
		}
		else{
			if( (1000 + end.millitm - start.millitm) >= 100)
			   break;
		}
	}
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.id      , sizeof( pckInBlock.id        ),  ST_TP_ALL       , DATA_TYPE_STRING );			// 종목type                 

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		1						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T8430(void)
{

	// 전체 종목 data 받기 위해서 
	t8430InBlock	pckInBlock;
	CString csmsg;

	TCHAR			szTrNo[]		= _T8430;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.gubun       , sizeof( pckInBlock.gubun        ),  ST_TP_ALL       , DATA_TYPE_STRING );			// 종목type                 

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 1);

	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);
	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		-1						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1532(CString shcode)
{

	t1532InBlock	pckInBlock;
	CString csmsg;

	TCHAR			szTrNo[]		= _T1532;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";
	struct timeb start, end;
	int left;
	int aa = 0, bb = 0;

	memset(&start, 0, sizeof(start));
 	memset(&end, 0, sizeof(start));
	aa = g_iXingAPI.GetTRCountRequest(_T1532);
	bb = g_iXingAPI.GetTRCountLimit(_T1532);
	csmsg.Format("%s - %d - %d\n", _T1532,  aa, bb);
	TRACE(csmsg);
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode      , sizeof( pckInBlock.shcode        ),  shcode       , DATA_TYPE_STRING );			// 단축코드              
         
	//-----------------------------------------------------------
	// 데이터 전송

	ftime(&start);
	while(1){
		ftime(&end);
		left = (int)(end.time - start.time);

		if(left == 0){
			if( (end.millitm - start.millitm) >= 1000)
			  break;
		}
		else{
			if( (1000 + end.millitm - start.millitm) >= 1000)
			   break;
		}
	}			

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1302(CString shcode, int gubun, int cnt)
{

	//분별 검색 
	t1302InBlock	pckInBlock;
	CString csmsg, csgub, cscnt;

	TCHAR			szTrNo[]		= _T1302;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	cscnt.Format("%d", cnt);
	csgub.Format("%d", gubun);

	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 1);

	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);
	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode       , sizeof( pckInBlock.shcode        ),  shcode       , DATA_TYPE_STRING );	// 종목 code             
	SetPacketData( pckInBlock.gubun       , sizeof( pckInBlock.gubun        ),  csgub       , DATA_TYPE_STRING );		// 작업구간 0:초,1:1분,2:3분,3:5분,4:10분,5:30분,6:60분 
	SetPacketData( pckInBlock.cnt       , sizeof( pckInBlock.cnt        ),  cscnt       , DATA_TYPE_STRING );			// count   
	//-----------------------------------------------------------
	// 데이터 전송

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1602(CString upcode, int cnt)
{

	t1602InBlock	pckInBlock;
	CString csmsg, csgub, cscts, cscnt;
	int aa = 0;

	TCHAR			szTrNo[]		= _T1602;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	cscnt.Format("%d", cnt);

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.upcode     , sizeof( pckInBlock.upcode        ),  upcode       , DATA_TYPE_STRING );			// 단축코드              
	SetPacketData( pckInBlock.gubun1      , sizeof( pckInBlock.gubun1        ), _T("2")       , DATA_TYPE_STRING );	  
  	SetPacketData( pckInBlock.gubun2      , sizeof( pckInBlock.gubun2        ),  _T("0")       , DATA_TYPE_STRING );	
	SetPacketData( pckInBlock.cnt      , sizeof( pckInBlock.cnt        ),  cscnt       , DATA_TYPE_STRING );	

	//-----------------------------------------------------------
	
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}




int CGetList::T1305(CString shcode, int gubun, int cnt)
{
    //기간별 주가
	t1305InBlock	pckInBlock;
	CString csmsg, csgub, cscnt;

	TCHAR			szTrNo[]		= _T1305;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	cscnt.Format("%d", cnt);
	csgub.Format("%d", gubun);

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode       , sizeof( pckInBlock.shcode        ),  shcode       , DATA_TYPE_STRING );			// 종목 code             
	SetPacketData( pckInBlock.dwmcode      , sizeof( pckInBlock.dwmcode        ),  csgub       , DATA_TYPE_LONG );			// 작업구간 1:일,2:주,3:월 
	SetPacketData( pckInBlock.cnt       , sizeof( pckInBlock.cnt        ),  cscnt       , DATA_TYPE_LONG );			// count                   
         
	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 1);

	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);
	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1825(CString search_cd, CString gubun)
{


	// Q종목 검색리스트 
	t1825InBlock	pckInBlock;
	CString csmsg, csgub, csdate;

	TCHAR			szTrNo[]		= _T1825;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	COleDateTime t, t1;
	int aa = 0;

	aa = g_iXingAPI.GetTRCountRequest(_T1825);
	csmsg.Format("%s - %d", _T1825,  aa);
	//TRACE(csmsg);
	
	t = COleDateTime::GetCurrentTime();

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );


	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.search_cd       , sizeof( pckInBlock.search_cd        ),  search_cd       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.gubun       , sizeof( pckInBlock.gubun        ),  gubun       , DATA_TYPE_STRING );	 

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 0);
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1826(int index)
{


	// Q종목 검색리스트 
	t1826InBlock	pckInBlock;
	CString csmsg, csgub, csdate;

	TCHAR			szTrNo[]		= _T1826;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );


	//-----------------------------------------------------------
	// 데이터 입력
	csgub.Format("%d", index);
	SetPacketData( pckInBlock.search_gb       , sizeof( pckInBlock.search_gb        ),  csgub       , DATA_TYPE_STRING );	           
		 

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 2);
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1702(CString shcode)
{


	// Q종목 검색리스트 
	t1702InBlock	pckInBlock;
	CString csmsg, csgub, csdate;

	TCHAR			szTrNo[]		= _T1702;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	csdate.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode       , sizeof( pckInBlock.shcode        ),  shcode       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.todt      , sizeof( pckInBlock.todt        ),  csdate       , DATA_TYPE_STRING );			
	SetPacketData( pckInBlock.volvalgb       , sizeof( pckInBlock.volvalgb        ),  _T("0")       , DATA_TYPE_STRING );			                  
 	SetPacketData( pckInBlock.msmdgb       , sizeof( pckInBlock.msmdgb        ),  "0"       , DATA_TYPE_STRING );		
 	SetPacketData( pckInBlock.cumulgb       , sizeof( pckInBlock.cumulgb        ),  "0"       , DATA_TYPE_STRING );			 

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 3);
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}int CGetList::T1442(int ctsidx)
{


	// Q종목 검색리스트 
	t1442InBlock	pckInBlock;
	CString csmsg, cscode, csidx, csdate;

	TCHAR			szTrNo[]		= _T1442;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	csdate.Format("%d", ctsidx);
	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	if(ctsidx){
		bNext = TRUE;
	}

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.gubun      , sizeof( pckInBlock.gubun        ),  "0"       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.type1      , sizeof( pckInBlock.type1        ),  "0"       , DATA_TYPE_STRING );			
	SetPacketData( pckInBlock.type2      , sizeof( pckInBlock.type2        ),  "6"       , DATA_TYPE_STRING );	
	SetPacketData( pckInBlock.type3      , sizeof( pckInBlock.type3        ),  "0"       , DATA_TYPE_STRING );
	SetPacketData( pckInBlock.idx		 , sizeof( pckInBlock.idx        ),  csdate       , DATA_TYPE_STRING );

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 2);
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}


int CGetList::T1444(int index)
{


	// Q종목 검색리스트 
	t1444InBlock	pckInBlock;
	CString csmsg, cscode, csidx, csdate;

	TCHAR			szTrNo[]		= _T1444;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";


	COleDateTime t, t1;
	
	t = COleDateTime::GetCurrentTime();

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	csdate.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	cscode = "000";
	if(index > 0 ){
		csidx.Format("%d", index);
		//memcpy(szNextKey, (LPSTR)(LPCTSTR)csidx.GetBuffer(), csidx.GetLength());
		bNext = TRUE;
	}
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.upcode      , sizeof( pckInBlock.upcode        ),  cscode       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.idx      , sizeof( pckInBlock.idx        ),  csidx       , DATA_TYPE_LONG );			
		 

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 2);
	
	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}



int CGetList::T1301(CString cscode, CString csvol, CString csCT, CString csST, CString csET)
{


	// Q종목 검색리스트 
	t1301InBlock	pckInBlock;
	CString csmsg, csidx, csdate;

	TCHAR			szTrNo[]		= _T1301;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";
    struct timeb start, end;
	int left;

	memset(&start, 0, sizeof(start));
 	memset(&end, 0, sizeof(start));

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	if(!csCT.IsEmpty() ){
		bNext = TRUE;
	}
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode      , sizeof( pckInBlock.shcode        ),  cscode       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.cvolume      , sizeof( pckInBlock.cvolume        ),  csvol       , DATA_TYPE_LONG );			
	SetPacketData( pckInBlock.starttime     , sizeof( pckInBlock.starttime        ),  csST       , DATA_TYPE_STRING );	           
	SetPacketData( pckInBlock.endtime      , sizeof( pckInBlock.endtime        ),  csET       , DATA_TYPE_STRING );			
	SetPacketData( pckInBlock.cts_time      , sizeof( pckInBlock.cts_time        ),  csCT       , DATA_TYPE_STRING );	           
	
	ftime(&start);
	while(1){
		ftime(&end);
		left = (int)(end.time - start.time);

		if(left == 0){
			if( (end.millitm - start.millitm) >= 498)
			  break;
		}
		else{
			if( (1000 + end.millitm - start.millitm) >= 498)
			   break;
		}
	}			

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		1						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		TRACE(csmsg);
		return false;
	}
	return true;
	
}

int CGetList::T1102(CString shcode)
{


	t1102InBlock	pckInBlock;
	CString csmsg;

	TCHAR			szTrNo[]		= _T1102;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";
	int aa = 0;

	struct timeb start, end;
	int left;


	memset(&start, 0, sizeof(start));
 	memset(&end, 0, sizeof(start));

	aa = g_iXingAPI.GetTRCountRequest(_T1102);
	csmsg.Format("%s - %d", _T1102,  aa);
	//TRACE(csmsg);

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.shcode      , sizeof( pckInBlock.shcode        ),  shcode       , DATA_TYPE_STRING );			// 단축코드              
         
	//-----------------------------------------------------------
	// 데이터 전송

	ftime(&start);
	while(1){
		ftime(&end);
		left = (int)(end.time - start.time);

		if(left == 0){
			if( (end.millitm - start.millitm) >= 99)
			  break;
		}
		else{
			if( (1000 + end.millitm - start.millitm) >= 99)
			   break;
		}
	}			

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}
int CGetList::T1636(int gubun, int ctsidx)
{

	t1636InBlock	pckInBlock;
	CString csmsg, csgub, cscts;
	int aa = 0;

	TCHAR			szTrNo[]		= _T1636;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	if(ctsidx){
		bNext = TRUE;
	}

	csgub.Format("%d", gubun);
	cscts.Format("%d", ctsidx);
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.gubun      , sizeof( pckInBlock.gubun        ),  csgub       , DATA_TYPE_STRING );			// 단축코드              
	SetPacketData( pckInBlock.gubun1      , sizeof( pckInBlock.gubun1        ), _T("0")       , DATA_TYPE_STRING );	  
  	SetPacketData( pckInBlock.gubun2      , sizeof( pckInBlock.gubun2        ),  _T("1")       , DATA_TYPE_STRING );	
	SetPacketData( pckInBlock.cts_idx      , sizeof( pckInBlock.cts_idx        ),  cscts       , DATA_TYPE_STRING );	

	//-----------------------------------------------------------
	// 데이터 전송
	COleDateTimeSpan ts;
	ts.SetDateTimeSpan(0, 0, 0, 0);
	COleDateTime t, t1;

	t = COleDateTime::GetCurrentTime();
	t.operator+=(ts);

	aa = g_iXingAPI.GetTRCountRequest(_T1636);
	csmsg.Format("%s - %d", _T1636,  aa);
	//TRACE(csmsg);

	while(1){
		Sleep(1);
		t1 = COleDateTime::GetCurrentTime();
		if(t1 >= t)
			break;
	}		

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}




int CGetList::T1637(CString cscode, CString cubun1,  CString cubun2)
{

	t1637InBlock	pckInBlock;
	CString csmsg, csgub, cscts;
	int aa = 0;

	TCHAR			szTrNo[]		= _T1637;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.gubun1      , sizeof( pckInBlock.gubun1        ),  cubun1       , DATA_TYPE_STRING );			             
	SetPacketData( pckInBlock.gubun2      , sizeof( pckInBlock.gubun1        ), cubun2       , DATA_TYPE_STRING );	  
  	SetPacketData( pckInBlock.shcode      , sizeof( pckInBlock.shcode        ),  cscode       , DATA_TYPE_STRING );	// 단축코드 
	SetPacketData( pckInBlock.cts_idx      , sizeof( pckInBlock.cts_idx        ),  "9999"       , DATA_TYPE_STRING );	

	

	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		2						// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);

	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		csmsg.Format(_T("Error [%d] %s"), nRqID, g_iXingAPI.GetErrorMessage( nRqID ));
		//::AfxMessageBox(csmsg);
		return false;
	}
	return true;
	
}



int CGetList::getCurInfo(CString shcode)
{
	T1102(shcode);
	return 0;
}


