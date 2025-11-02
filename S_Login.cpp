// S_Login.cpp : implementation file
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"
#include "S_Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CS_Login

CS_Login::CS_Login()
{
}

CS_Login::~CS_Login()
{
}


BEGIN_MESSAGE_MAP(CS_Login, CButton)
	//{{AFX_MSG_MAP(CS_Login)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_MESSAGE( WM_USER + XM_LOGIN,  OnLogin  )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CS_Login message handlers

bool CS_Login::ConnectServer()
{
	//-----------------------------------------------------------------------------
	// 이미 접속되어 있으면 접속을 종료한다.
	if( g_iXingAPI.IsConnected() )
	{
		g_iXingAPI.Disconnect();
	}
	TRACE("ConnectServer\n");
	//-----------------------------------------------------------------------------
	// 서버 IP
	CString strServerIP;
	strServerIP = "api.ls-sec.co.kr"; //"hts.etrade.co.kr";
	//strServerIP = "demo.etrade.co.kr";
		
	//-----------------------------------------------------------------------------
	// 서버 Port
	int nServerPort = 20001;
	
	//-----------------------------------------------------------------------------
	// 공유기 사용
	int nSendPacketSize = -1;		// -1 로 설정하면 기본값을 사용한다.
		
	//-----------------------------------------------------------------------------
	// 서버연결시간
	int nConnectTimeOut = -1;		// -1 로 설정하면 기본값을 사용한다.
	
	
	//-----------------------------------------------------------------------------
	// 서버접속
	BOOL bResult = g_iXingAPI.Connect( 
		AfxGetMainWnd()->GetSafeHwnd(),			// Connect가 된 이후에 Disconnect 메시지를 받을 윈도우 Handle
												// Login 윈도우는 Login 만 처리하는 윈도우 이므로 Disconnect는 메인윈도우에서 받는다.

		strServerIP,							// 서버주소

		nServerPort,							// 서버포트

		WM_USER,								// XingAPI에서 사용하는 메시지의 시작번호, Windows에서는 사용자메시지를 0x400(=WM_USER) 이상을
												// 사용해야 함. 기본적으로는 WM_USER를 사용하면 되지만 프로그램 내부에서 메시지 ID가 겹치게 되면
												// 이 값을 조정하여 메시지 ID 충돌을 피할수 있음

		nConnectTimeOut,						// 지정한 시간이상(1/1000 초 단위)으로 시간이 걸리게 될 경우 연결실패로 간주함

		nSendPacketSize							// 보내어지는 Packet Size, -1 이면 기본값 사용
												// 인터넷 공유기등에서는 특정 크기 이상의 데이터를 한번에 보내면 에러가 떨어지는 경우가 발생
												// 이럴 경우에 한번에 보내는 Packet Size를 지정하여 그 이상 되는 Packet은 여러번에 걸쳐 전송
	);

	//-----------------------------------------------------------------------------
	// 접속실패 처리
	if( bResult == FALSE )
	{
		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg = g_iXingAPI.GetErrorMessage( nErrorCode );
		
		//TRACE("XingAPI 서버 접속 실패",strMsg);
		//AfxMessageBox(strMsg);
		return FALSE;
	}

	return TRUE;
}


bool CS_Login::Login(CString csid, CString cspw, CString cscerti)
{
	//-----------------------------------------------------------------------------
	// 로그인
	BOOL bResult = g_iXingAPI.Login( 
		GetSafeHwnd(),							// Login 성공여부 메시지를 받을 윈도우
		csid, //STR_ID,									// 사용자 ID
		cspw, //STR_PASSWORD_ID,						// 사용자 비밀번호
		cscerti, //STR_PASSWORD_CERTI,						// 공인인증 비밀번호
		0,										// 0 : 실서버, 1 : 모의투자서버
		1									// 로그인 중에 공인인증 에러가 발생시 에러메시지 표시여부
	);

	//-----------------------------------------------------------------------------
	// 로그인 에러 발생,
	//		이것은 로그인 사전단계에서 발생한 에러이며 로그인 과정에서 발생한 에러는
	//		메시지로 알려준다.
	if( bResult == FALSE )
	{
		EnableWindow( TRUE );

		int nErrorCode = g_iXingAPI.GetLastError();
		CString strMsg;
		
		strMsg.Format( _T("XingAPI 로그인 실패 - %s"), g_iXingAPI.GetErrorMessage( nErrorCode ) );
		
		//AfxMessageBox(strMsg);		
		return FALSE;
	}

	return TRUE;
}

//================================================================================================
// 로그인 결과
//================================================================================================
CString csLoginMsg;
LRESULT CS_Login::OnLogin( WPARAM wParam, LPARAM lParam )
{
	LPCSTR pszCode = (LPCSTR)wParam;
	LPCSTR pszMsg  = (LPCSTR)lParam;


	csLoginMsg.Format( "[%s] %s", pszCode, pszMsg );

	EnableWindow( TRUE );

	if( atoi( pszCode ) == 0 ) {
		// 로그인 성공
		csLoginMsg.Format("LogOnOK");
		m_gnLogonFlag = 1;
	} else {
		// 로그인 실패
		csLoginMsg.Format("XingAPI 로그인 실패-2\n",csLoginMsg);	
		m_gnLogonFlag = 0;
	}

	//TRACE(csLoginMsg);
	return 0L;
}