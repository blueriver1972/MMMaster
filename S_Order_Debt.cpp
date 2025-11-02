// S_Order_Debt.cpp : implementation file
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"
#include "S_Order_Debt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_reCode;
/////////////////////////////////////////////////////////////////////////////
// CS_Order_Debt
extern CString gscCount, gscCountpw, gcsID, gcsPw, gcsVPw, gcsTotval, gcsMsrate, gcsMAXMdrate, gcsSonjel;
CS_Order_Debt::CS_Order_Debt()
{
}

CS_Order_Debt::~CS_Order_Debt()
{
}


BEGIN_MESSAGE_MAP(CS_Order_Debt, CButton)
	//{{AFX_MSG_MAP(CS_Order_Debt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
//	ON_MESSAGE	 ( WM_USER + XM_RECEIVE_DATA, OnXMReceiveData	  )
//	ON_MESSAGE	 ( WM_USER + XM_TIMEOUT_DATA, OnXMTimeoutData	  )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CS_Order_Debt message handlers


// 계좌번호       
// 입력비밀번호   
// 종목번호       
// 주문수량       
// 주문가         
// 매매구분     //  1 : 매도 , 2 : 매수
// 호가유형코드   //00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가, 07:최우선지정가, 10:보통(IOC), 13:시장가(IOC), 16:최유리(IOC), 20:보통(FOK)
					 //61:장개시전시간외 종가, 81:시간외 종가, 82 "시간외 단일가
// 신용거래코드   
// 대출일         
// 주문조건구분   

//00:보통
//03:시장가
//05:조건부지정가
//06:최유리지정가
//07:최우선지정가
//10:보통(IOC)
//13:시장가(IOC)
//16:최유리(IOC)
//20:보통(FOK)
extern CString gcsEndTime;
bool CS_Order_Debt::StockBuy(CString strcode, CString strOrdQty, CString strOrdPrc, CString mode, CString type)
{	
	CString cstime, c;
	COleDateTime t = COleDateTime::GetCurrentTime();
	cstime.Format("%02d%02d",t.GetHour(), t.GetMinute() );

	if(atoi(cstime) > (atoi(gcsEndTime) + 30)){
		mode = "82";
	}
	else if(atoi(cstime) > atoi(gcsEndTime)){
		mode = "81";
		strOrdPrc.Empty();
	}
	if((mode == "03") || (mode == "06") ||(mode == "07")){
		strOrdPrc.Empty();
	}
	c.Format("%s - %s, %s, %s,  aa: %s : bbb", strcode, mode, type,  strOrdQty, strOrdPrc);
    TRACE(c);
	g_reCode = strcode;
	return CSPAT00600(
		gscCount,
		gscCountpw, 
		strcode,
		strOrdQty,
		strOrdPrc,
		"2",
		mode, //00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가, 07:최우선지정가, 61:장개시전시간외 종가, 81:시간외 종가, 82 "시간외 단일가
		// 유통융자 대출
		type, //000:보통 003:유통/자기융자신규005:유통대주신규007:자기대주신규101:유통융자상환103:자기융자상환105:유통대주상환107:자기대주상환180:예탁담보대출상환(신용)
		"",
		"0"
		);
}

#define SECOND_COUNT	"20208782501"

bool CS_Order_Debt::StockSecondBuy(CString strcode, CString strOrdQty, CString strOrdPrc, CString mode, CString type)
{	
	CString cstime, c;
	COleDateTime t = COleDateTime::GetCurrentTime();
	cstime.Format("%02d%02d",t.GetHour(), t.GetMinute() );

	if(atoi(cstime) > (atoi(gcsEndTime) + 30)){
		mode = "82";
	}
	else if(atoi(cstime) > atoi(gcsEndTime)){
		mode = "81";
		strOrdPrc.Empty();
	}
	if(mode == "03"){
		strOrdPrc.Empty();
	}

	g_reCode = strcode;
	return CSPAT00600(
		SECOND_COUNT, //second
		gscCountpw, 

		strcode,
		strOrdQty,
		strOrdPrc,
		"2",
		mode, //00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가, 07:최우선지정가, 61:장개시전시간외 종가, 81:시간외 종가, 82 "시간외 단일가
		// 유통융자 대출
		type, //000:보통 003:유통/자기융자신규005:유통대주신규007:자기대주신규101:유통융자상환103:자기융자상환105:유통대주상환107:자기대주상환180:예탁담보대출상환(신용)
		"",
		"0"
		);
}

bool CS_Order_Debt::StockSell(CString strCode, CString strOrdQty, CString strOrdPrc, CString mode, CString type, CString csLoanDt)
{
	CString strLoanDt, cstime;
	COleDateTime t = COleDateTime::GetCurrentTime();

	strLoanDt.Format("%d%d%d",t.GetYear(), t.GetMonth(), t.GetDay());
	cstime.Format("%02d%02d", t.GetHour(), t.GetMinute());

	if(atoi(cstime) >= (atoi(gcsEndTime) + 70)){
		mode = "82";
	}
	else if(atoi(cstime) > atoi(gcsEndTime)){
		mode = "81";
		strOrdPrc.Empty();
	}
	g_reCode = strCode;
	return CSPAT00600(
		gscCount,
		gscCountpw, 
		strCode,
		strOrdQty,
		strOrdPrc,
		"1",
		mode, //00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가, 07:최우선지정가, 61:장개시전시간외 종가, 81:시간외 종가, 82 "시간외 단일가
		type, //000:보통 003:유통/자기융자신규005:유통대주신규007:자기대주신규101:유통융자상환103:자기융자상환105:유통대주상환107:자기대주상환180:예탁담보대출상환(신용)
		// 매수 체결시에 받는 대출일
		csLoanDt,
		"0"
		);
	return true;
}

bool CS_Order_Debt::CSPAT00600(
		CString strAcntNo, CString strInptPwd ,CString strIsuNo ,CString strOrdQty ,CString strOrdPrc       
		,CString strBnsTpCode ,CString strOrdprcPtnCode ,CString strMgntrnCode ,CString strLoanDt, CString strOrdCndiTpCode) 
{
	//-----------------------------------------------------------
	// 현물정상주문(CSPAT00600) ( base21=SONAT000,headtype=B )
	CSPAT00600InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAT00600";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	strOrdQty.Remove(',');
	strOrdPrc.Remove(',');
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.InptPwd      , sizeof( pckInBlock.InptPwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.IsuNo        , sizeof( pckInBlock.IsuNo         ), strIsuNo        , DATA_TYPE_STRING );			// 종목번호                 
	SetPacketData( pckInBlock.OrdQty       , sizeof( pckInBlock.OrdQty        ), strOrdQty       , DATA_TYPE_LONG   );			// 주문수량                 
	SetPacketData( pckInBlock.OrdPrc       , sizeof( pckInBlock.OrdPrc        ), strOrdPrc       , DATA_TYPE_FLOAT_DOT, 2 );	// 주문가, Header Type이 B 인 경우 Data Type이 실수면 소수점을 포함해야 한다.
	SetPacketData( pckInBlock.BnsTpCode    , sizeof( pckInBlock.BnsTpCode     ), strBnsTpCode    , DATA_TYPE_STRING );			// 매매구분                 
	SetPacketData( pckInBlock.OrdprcPtnCode, sizeof( pckInBlock.OrdprcPtnCode ), strOrdprcPtnCode, DATA_TYPE_STRING );			// 호가유형코드             
	SetPacketData( pckInBlock.MgntrnCode   , sizeof( pckInBlock.MgntrnCode    ), strMgntrnCode   , DATA_TYPE_STRING );			// 신용거래코드             
	SetPacketData( pckInBlock.LoanDt       , sizeof( pckInBlock.LoanDt        ), strLoanDt       , DATA_TYPE_STRING );			// 대출일                   
	SetPacketData( pckInBlock.OrdCndiTpCode, sizeof( pckInBlock.OrdCndiTpCode ), strOrdCndiTpCode, DATA_TYPE_STRING ); 			// 주문조건구분             
	TRACE("매수정보 :%s,%s %s, %s, %s, - %s, %s,%s,%s, %s ", strAcntNo,strInptPwd , strIsuNo, strOrdQty, strOrdPrc, strBnsTpCode, strOrdprcPtnCode, strMgntrnCode, strLoanDt, strOrdCndiTpCode);
	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 매수매도 주문 Request ID < 0");
		return false;
	}	
	TRACE("Order True - 매수매도 주문 Request ID : %d\n", nRqID);
	
	return true;

}


bool CS_Order_Debt::StockModify(CString strOrgordNo, CString strCode, CString strOrdQty, CString strOrdPrc, CString mode)
{
	CString strOrdprcPthnCode = "0";

	return CSPAT00700(
		strOrgordNo,
		gscCount,
		gscCountpw, 
		strCode,
		strOrdQty,
		strOrdPrc,
		mode,
		"0"
		);
	return true;
}

bool CS_Order_Debt::CSPAT00700(CString strOrgordNo, 
		CString strAcntNo, CString strInptPwd ,CString strIsuNo ,CString strOrdQty ,CString strOrdPrc,
		CString strOrdprcPtnCode , CString strOrdCndiTpCode) 
{
	//-----------------------------------------------------------
	// 현물정정주문(CSPAT00700) ( base21=SONAT000,headtype=B )
	CSPAT00700InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAT00700";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	strOrdQty.Remove(',');
	strOrdPrc.Remove(',');
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.OrgOrdNo       , sizeof( pckInBlock.OrgOrdNo        ), strOrgordNo       , DATA_TYPE_LONG );		// 원주문번호  
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.InptPwd      , sizeof( pckInBlock.InptPwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.IsuNo        , sizeof( pckInBlock.IsuNo         ), strIsuNo        , DATA_TYPE_STRING );			// 종목번호                 
	SetPacketData( pckInBlock.OrdQty       , sizeof( pckInBlock.OrdQty        ), strOrdQty       , DATA_TYPE_LONG   );			// 주문수량                 
	SetPacketData( pckInBlock.OrdPrc       , sizeof( pckInBlock.OrdPrc        ), strOrdPrc       , DATA_TYPE_FLOAT_DOT, 2 );	// 주문가, Header Type이 B 인 경우 Data Type이 실수면 소수점을 포함해야 한다.
	SetPacketData( pckInBlock.OrdprcPtnCode, sizeof( pckInBlock.OrdprcPtnCode ), strOrdprcPtnCode, DATA_TYPE_STRING );			// 호가유형코드             
	SetPacketData( pckInBlock.OrdCndiTpCode, sizeof( pckInBlock.OrdCndiTpCode ), strOrdCndiTpCode, DATA_TYPE_STRING ); 			// 주문조건구분             

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 매수매도 주문 Request ID < 0");
		return false;
	}
	
	return true;

}

bool CS_Order_Debt::StockCancel(CString strOrgordNo, CString strCode, CString strOrdQty)
{
	CString strOrdprcPthnCode = "0";

	return CSPAT00800(
		strOrgordNo,
		gscCount,
		gscCountpw, 
		strCode,
		strOrdQty
		);
	return true;
}

bool CS_Order_Debt::CSPAT00800(CString strOrgordNo, CString strAcntNo, CString strInptPwd ,CString strIsuNo ,CString strOrdQty ) 
{
	//-----------------------------------------------------------
	// 현물정정주문(CSPAT00800) ( base21=SONAT000,headtype=B )
	CSPAT00800InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAT00800";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );
	strOrdQty.Remove(',');
	//strOrdPrc.Remove(',');
	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.OrgOrdNo       , sizeof( pckInBlock.OrgOrdNo        ), strOrgordNo       , DATA_TYPE_LONG );		// 원주문번호  
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.InptPwd      , sizeof( pckInBlock.InptPwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.IsuNo        , sizeof( pckInBlock.IsuNo         ), strIsuNo        , DATA_TYPE_STRING );			// 종목번호                 
	SetPacketData( pckInBlock.OrdQty       , sizeof( pckInBlock.OrdQty        ), strOrdQty       , DATA_TYPE_LONG   );			// 주문수량                 

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 매수매도 주문 Request ID < 0");
		return false;
	}
	
	return true;

}

bool CS_Order_Debt::StockCheck(CString strAcntNo, CString strPwd, CString strtemp)
{

	return CSPAQ12300(
		strAcntNo,
		strPwd,
		strtemp
		);
	return true;
}

bool CS_Order_Debt::CSPAQ12300(CString strAcntNo, CString strInptPwd ,CString strtemp) 
{
	//-----------------------------------------------------------
	// 현물정정주문(CSPAT00800) ( base21=SONAT000,headtype=B )
	CSPAQ12300InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAQ12300";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.RecCnt       , sizeof( pckInBlock.RecCnt        ), "1"       , DATA_TYPE_LONG );		// 레코드갯수 
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.Pwd      , sizeof( pckInBlock.Pwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.BalCreTp        , sizeof( pckInBlock.BalCreTp         ), "1"        , DATA_TYPE_STRING );			// 잔고생성구분                 
	SetPacketData( pckInBlock.CmsnAppTpCode       , sizeof( pckInBlock.CmsnAppTpCode        ), "1"       , DATA_TYPE_STRING   );			// 수수료적용구분     
	SetPacketData( pckInBlock.D2balBaseQryTp        , sizeof( pckInBlock.D2balBaseQryTp         ), "1"        , DATA_TYPE_STRING );			// 잔고기준조회구분                 
	SetPacketData( pckInBlock.UprcTpCode       , sizeof( pckInBlock.UprcTpCode        ), "1"       , DATA_TYPE_STRING   );			// 단가구분   

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 현물계좌 잔고내역 Request ID < 0");
		return false;
	}
	
	return true;

}

bool CS_Order_Debt::CheckVal(void)
{

	return CSPAQ12200(
		gscCount,
		gscCountpw
		);
	return true;
}

bool CS_Order_Debt::CSPAQ12200(CString strAcntNo, CString strInptPwd ) 
{
	//-----------------------------------------------------------
	// 현물정정주문(CSPAT00800) ( base21=SONAT000,headtype=B )
	CSPAQ02200InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAQ12200";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.RecCnt       , sizeof( pckInBlock.RecCnt        ), "1"       , DATA_TYPE_LONG );		// 레코드갯수 
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.Pwd		 , sizeof( pckInBlock.Pwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.MgmtBrnNo      , sizeof( pckInBlock.MgmtBrnNo         ), " "        , DATA_TYPE_STRING );			               
	SetPacketData( pckInBlock.BalCreTp      , sizeof( pckInBlock.BalCreTp        ), " "       , DATA_TYPE_STRING   );			    


	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		30							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 현물계좌 예수금/주문가능금액 Request ID < 0");
		return false;
	}
	
	return true;

}

bool CS_Order_Debt::OrderCheck(CString strAcntNo, CString strPwd, CString strmode)
{

	return CSPAQ13700(
		strAcntNo,
		strPwd,
		strmode
		);
	return true;
}

bool CS_Order_Debt::CSPAQ13700(CString strAcntNo, CString strInptPwd ,CString strmode) 
{
	//-----------------------------------------------------------
	// 현물정정주문(CSPAT00800) ( base21=SONAT000,headtype=B )
	CSPAQ13700InBlock1	pckInBlock;

	TCHAR			szTrNo[]		= "CSPAQ13700";
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.RecCnt       , sizeof( pckInBlock.RecCnt        ), "1"       , DATA_TYPE_LONG );		// 레코드갯수 
	SetPacketData( pckInBlock.AcntNo       , sizeof( pckInBlock.AcntNo        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.InptPwd      , sizeof( pckInBlock.InptPwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.OrdMktCode        , sizeof( pckInBlock.OrdMktCode         ), "00"        , DATA_TYPE_STRING );			// 잔고생성구분                 
	SetPacketData( pckInBlock.BnsTpCode       , sizeof( pckInBlock.BnsTpCode        ), strmode       , DATA_TYPE_STRING   );			// 수수료적용구분     
	SetPacketData( pckInBlock.ExecYn        , sizeof( pckInBlock.ExecYn         ), "3"        , DATA_TYPE_STRING );			// 잔고기준조회구분                 
	SetPacketData( pckInBlock.BkseqTpCode       , sizeof( pckInBlock.BkseqTpCode        ), "1"       , DATA_TYPE_STRING   );			// 단가구분   
	SetPacketData( pckInBlock.OrdPtnCode       , sizeof( pckInBlock.OrdPtnCode        ), "00"       , DATA_TYPE_STRING   );			// 단가구분   

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		3							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Order Fail - 현물계좌 잔고내역 Request ID < 0");
		return false;
	}
	
	return true;

}


bool CS_Order_Debt::CheckCount()
{

	return T0424(
		gscCount,
		gscCountpw, 
		"2",
		"2",
		"0",
		"1",
		""	
		);
	return true;
}



bool CS_Order_Debt::T0424(
		CString strAcntNo, CString strInptPwd , CString strexpcode ,CString strchegb       
		,CString strmedosu ,CString strsortgb ,CString strcts ) 
{
	//-----------------------------------------------------------

	t0424InBlock	pckInBlock;

	TCHAR			szTrNo[]		= NAME_t0424;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.accno      , sizeof( pckInBlock.accno        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.passwd     , sizeof( pckInBlock.passwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.prcgb        , sizeof( pckInBlock.prcgb         ), strexpcode        , DATA_TYPE_STRING );			// 단가구분                
	SetPacketData( pckInBlock.chegb       , sizeof( pckInBlock.chegb        ), strchegb       , DATA_TYPE_STRING   );			// 체결구분                 
	SetPacketData( pckInBlock.dangb   , sizeof( pckInBlock.dangb     ), strmedosu    , DATA_TYPE_STRING );			// 단일가구분                
	SetPacketData( pckInBlock.charge, sizeof( pckInBlock.charge ),		strsortgb, DATA_TYPE_STRING );			// 제비용포함여부             
	SetPacketData( pckInBlock.cts_expcode   , sizeof( pckInBlock.cts_expcode    ), strcts   , DATA_TYPE_STRING );			// CTS_종목번호           

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		10							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("fail T0424 Request ID < 0");
		return false;
	}
	
	return true;

}

bool CS_Order_Debt::CheckOrder(CString csnextvla)
{

	return T0425(
		gscCount,
		gscCountpw, 
		"",
		"0",
		"0",
		"1",
		csnextvla	
		);
	return true;
}

bool CS_Order_Debt::T0425(
		CString strAcntNo, CString strInptPwd , CString strexpcode ,CString strchegb       
		,CString strmedosu ,CString strsortgb ,CString strcts ) 
{
	//-----------------------------------------------------------

	t0425InBlock	pckInBlock;

	TCHAR			szTrNo[]		= NAME_t0425;
	BOOL			bNext			= FALSE;
	char			szNextKey[]		= "";

	if(!strcts.IsEmpty()){
		bNext			= TRUE;
	}
	//-----------------------------------------------------------
	// 데이터 초기화
	FillMemory( &pckInBlock, sizeof( pckInBlock ), ' ' );

	//-----------------------------------------------------------
	// 데이터 입력
	SetPacketData( pckInBlock.accno      , sizeof( pckInBlock.accno        ), strAcntNo       , DATA_TYPE_STRING );			// 계좌번호                 
	SetPacketData( pckInBlock.passwd     , sizeof( pckInBlock.passwd       ), strInptPwd      , DATA_TYPE_STRING );			// 입력비밀번호             
	SetPacketData( pckInBlock.expcode        , sizeof( pckInBlock.expcode         ), strexpcode        , DATA_TYPE_STRING );			// 종목번호                 
	SetPacketData( pckInBlock.chegb       , sizeof( pckInBlock.chegb        ), strchegb       , DATA_TYPE_STRING   );			// 주문수량                 
	SetPacketData( pckInBlock.medosu   , sizeof( pckInBlock.medosu     ), strmedosu    , DATA_TYPE_STRING );			// 매매구분                 
	SetPacketData( pckInBlock.sortgb, sizeof( pckInBlock.sortgb ),		strsortgb, DATA_TYPE_STRING );			// 호가유형코드             
	SetPacketData( pckInBlock.cts_ordno   , sizeof( pckInBlock.cts_ordno    ), strcts   , DATA_TYPE_STRING );			// 신용거래코드             

	//-----------------------------------------------------------
	// 데이터 전송
	int nRqID = g_iXingAPI.Request( 
		AfxGetMainWnd()->GetSafeHwnd(),			// 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
		szTrNo,						// TR 번호
		&pckInBlock,				// InBlock 데이터
		sizeof( pckInBlock ),		// InBlock 데이터 크기
		bNext,						// 다음조회 여부 : 다음조회일 경우에 세팅한다.
		szNextKey,					// 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
		10							// Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
	);


	//-----------------------------------------------------------
	// Request ID가 0보다 작을 경우에는 에러이다.
	if( nRqID < 0 )
	{
		TRACE("Fail - T0425 Request ID < 0");
		return false;
	}
	
	return true;

}



