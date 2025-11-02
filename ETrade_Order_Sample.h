// ETrade_Order_Sample.h : main header file for the ETRADE_ORDER_SAMPLE application
//

#if !defined(AFX_ETRADE_ORDER_SAMPLE_H__8D7CDC75_3FDA_4FE8_96BB_32CBE9BAD88A__INCLUDED_)
#define AFX_ETRADE_ORDER_SAMPLE_H__8D7CDC75_3FDA_4FE8_96BB_32CBE9BAD88A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CETrade_Order_SampleApp:
// See ETrade_Order_Sample.cpp for the implementation of this class
//
#define MAX_DB_JONGMOK			1000			//체크 종목 수
#define MAX_DATA				1000			//최대 데이터 크기
#define MAX_JONGMOK				2500		//최대 전체 종목수
#define MAX_PATTEN				6100		//최대 패턴 경우
#define MAX_MA_JONGMOK			3000			//manager 종목 수
#define MAX_MSMD_EVENT			MAX_DATA			//ms/md 수

struct JMINFOAVG{
	CString price;	
	CString volume;	
};

struct TJMAVGINFO5_20{
	CString shcode;		//종목코드
	CString hname;		//이름
	JMINFOAVG avginfo;
};

struct JMAVGINFOList{
	int listnum;
	TJMAVGINFO5_20 jminfo[MAX_JONGMOK];				//종목들을 저장하기 위한 구조체 배열
};

extern JMAVGINFOList jmavginfo5List, jmavginfo20List, jmavginfo60List;

struct COUNTINFO {

    CString    expcode;	// [string,   12] 종목번호                        StartPos 0, Length 12
    CString    jangb;		// [string,   10] 잔고구분                        StartPos 13, Length 10
    CString    janqty;		// [long  ,   18] 잔고수량                        StartPos 24, Length 18
    CString    mdposqt;    // [long  ,   18] 매도가능수량                    StartPos 43, Length 18
    CString    pamt;		// [long  ,   18] 평균단가                        StartPos 62, Length 18
    CString    mamt;		// [long  ,   18] 매입금액                        StartPos 81, Length 18
    CString    sinamt;		// [long  ,   18] 대출금액                        StartPos 100, Length 18
    CString    lastdt;		// [string,    8] 만기일자                        StartPos 119, Length 8
    CString    msat;		// [long  ,   18] 당일매수금액                    StartPos 128, Length 18
    CString    mpms;		// [long  ,   18] 당일매수단가                    StartPos 147, Length 18
    CString    mdat;		// [long  ,   18] 당일매도금액                    StartPos 166, Length 18
    CString    mpmd;		// [long  ,   18] 당일매도단가                    StartPos 185, Length 18
    CString    jsat;		// [long  ,   18] 전일매수금액                    StartPos 204, Length 18
    CString    jpms;		// [long  ,   18] 전일매수단가                    StartPos 223, Length 18
    CString    jdat;		// [long  ,   18] 전일매도금액                    StartPos 242, Length 18
    CString    jpmd;		// [long  ,   18] 전일매도단가                    StartPos 261, Length 18
    CString    sysprocseq;    // [long  ,   10] 처리순번                        StartPos 280, Length 10
    CString    loandt;		// [string,    8] 대출일자                        StartPos 291, Length 8
    CString    hname;		// [string,   20] 종목명                          StartPos 300, Length 20
    CString    marketgb;   // [string,    1] 시장구분                        StartPos 321, Length 1
    CString    jonggb;		// [string,    1] 종목구분                        StartPos 323, Length 1
    CString    janrt;		// [float , 10.2] 보유비중                        StartPos 325, Length 10
    CString    price;		// [long  ,    8] 현재가                          StartPos 336, Length 8
	CString	   rate;        //등락율
    CString    appamt;		// [long  ,   18] 평가금액                        StartPos 345, Length 18
    CString    dtsunik;    // [long  ,   18] 평가손익                        StartPos 364, Length 18
    CString    sunikrt;    // [float , 10.2] 수익율                          StartPos 383, Length 10
    CString    fee;		// [long  ,   10] 수수료                          StartPos 394, Length 10
    CString    tax;		// [long  ,   10] 제세금                          StartPos 405, Length 10
    CString    sininter;   // [long  ,   10] 신용이자                        StartPos 416, Length 10
	CString		msNum;//order number..
};

struct ALLCOUNTINFO{
	int cncnt;
	COUNTINFO jminfo[MAX_MA_JONGMOK];
};

struct EVINFO {

    CString    ordno;		// [long  ,   10] 주문번호                        StartPos 0, Length 10
    CString    expcode;		// [string,   12] 종목번호                        StartPos 11, Length 12
    CString    medosu;		// [string,   10] 구분                            StartPos 24, Length 10
    CString    qty;			// [long  ,    9] 주문수량                        StartPos 35, Length 9
    CString    mprice;		// [long  ,    9] 주문가격                        StartPos 45, Length 9
    CString    cheqty;		// [long  ,    9] 체결수량                        StartPos 55, Length 9
    CString    cheprice;    // [long  ,    9] 체결가격                        StartPos 65, Length 9
    CString    ordrem;		// [long  ,    9] 미체결잔량                      StartPos 75, Length 9
    CString    cfmqty;		// [long  ,    9] 확인수량                        StartPos 85, Length 9
    CString    status;		// [string,   10] 상태                            StartPos 95, Length 10
    CString    orgordno;    // [long  ,   10] 원주문번호                      StartPos 106, Length 10
    CString    ordgb;		// [string,   20] 유형                            StartPos 117, Length 20
    CString    ordtime;		// [string,    8] 주문시간                        StartPos 138, Length 8
    CString    ordermtd;    // [string,   10] 주문매체                        StartPos 147, Length 10
    CString    sysprocseq;	// [long  ,   10] 처리순번                        StartPos 158, Length 10
    CString    hogagb;		// [string,    2] 호가유형                        StartPos 169, Length 2
    CString    curprice;		// [long  ,    8] 현재가                          StartPos 172, Length 8
    CString    orggb;		// [string,    2] 주문구분                        StartPos 181, Length 2
    CString    singb;		// [string,    2] 신용구분                        StartPos 184, Length 2
    CString    loandt;		 // [string,    8] 대출일자                        StartPos 187, Length 8
};

struct ALLMSMDINFO{
	int msmdcnt;
	EVINFO evinfo[MAX_MSMD_EVENT];
};


struct CNTJMINFO{
	CString curprice;
	CString sunikrate;
};


struct MIJMINFO{
	CString cscode;
	CString cshname;
	CString cssysseq;
	CString csjonggb;
	int mode;
	int flag;

	CNTJMINFO cur;
	CNTJMINFO priv;
	CNTJMINFO first;

};

typedef struct __MAJMINFO{

	int cnt;
	MIJMINFO alljminfo[MAX_DATA];

}MAJMINFO;

struct JISUINFO{
	CString curval;
	CString currate;
	CString precurval;
	CString precurrate;
	CString openrate;
	CString lowrate;
	CString hirate;
};

typedef struct __ALLJISUINFO{
	JISUINFO kospi;
	JISUINFO kosdak;
}ALLJISUINFO;


// 하나의 데이터를 저장하기 위한 구조체
struct JMINFO{

	CString csCount;
	CString csPw;

	CString shcode;			//종목코드
	CString hname;			//이름
	CString price;			//가격

	CString mdprice;		//
	CString msprice;		//
	CString sonjelprice;
	CString mglevel;

	CString mscount;

	CString stprice;		//
	CString hiprice;		//
	CString lowprice;		//
	CString jvolume;
	CString avg;

	CString level1;			//03 ms/md type
	CString level2;			//06/07
	CString level3;			//00
	CString urgentflag;		// cancel all and then sell all
	CString diff;
	CString jdiff;
	CString msmode;
	CString tmcode;
	CString upcode;
	CString csjanginfo;

	CString typemode;

	CString curnumber;

	CString perMoney;
	CString perNum;
	CString perOrdNum;

	CString sc_cd;
};

struct ALLJMINFO{
	int jmcnt;
	JMINFO jminfo[MAX_MA_JONGMOK];
};

typedef struct __MAJMINF{

	int jmIndex;
	int rev;
	ALLCOUNTINFO cntinfo;
	ALLMSMDINFO  msmdinfo;
	ALLJMINFO	mginfo;

}MAJMINF;

struct TJMINFO{

	CString shcode;		//종목코드
	CString hname;		//이름
	CString price;		//가격
	CString diff;

	CString per;
	CString chel;

	CString totval;	  // 누적 금액
	CString volume;   //누적 거래량
	CString jvolume;   //누적 거래량
	CString volume20;   //누적 거래량

	CString chdegree;

	CString openprice;		//가격
	CString hiprice;		//가격
	CString lowprice;		//가격

	CString avg5price;		//가격
	CString avg20price;		//가격
	CString avg60price;		//가격

	CString avg5vol;		//거래량
	CString avg20vol;		//거래량
	CString avg60vol;		//거래량

	CString date52hi;
	CString date52low;

	CString preval;		//전분기
	CString ppreval;	//전전분기

	CString salert;	//전년대비 매출
	CString opert;		//전년대비 영업이익
	CString netrt;		//전년대비 영업이익

	CString org;	//기관
	CString forg;		//외국인
	CString person;		//개인
	CString gita;		//기타
	CString prog;		//프로그램

};


struct TJMINFOLIST {
	int		listnum;
	TJMINFO jminfo[MAX_JONGMOK];
}
;extern TJMINFOLIST tjmList;

// 하나의 데이터를 저장하기 위한 구조체
struct OrderInfo {
	int flagcount;//+/- count
	int flag5index;// 5로 나눈 index
	int orderflag;
	int mode;		//매수/매도/cancel
	int status;     //진행방향 매수/매도/cancel
	int avgprice;//평균가
	int preavg;	//
	int count; 

	CString shcode;		//종목코드
	CString msorgnum;
	CString mdorgnum;	
	CString hname;		//이름
	CString avg;		//가중평균
	CString open;	//시가
	CString high;	//고가
	CString low;	//저가

	CString maxprice;		//max price
	CString lowprice;		//max price

	CString price;		//현재가격
	CString msprice;    //매수가격
	CString chprice;    //체결가격
	CString mdprice;    //매도가격

	CString number;      //주문수량
	CString mscount;	//매수수량
	CString mdcount;	//매도수량	
	CString msjancount;	//매수잔량
	CString mdjancount;	//매도잔량

	CString csvalue;
	CString rate;
	CString vol; //회전율
	CString diff;//등락율

	CString volume; //거래량
	CString volumediff; //전일 거래량
	CString jvolume; //거래량차

	CString total; //시가 총액
	CString memdan; //수량 단위
};




typedef struct __tagQ12300INFO {
    CString    IsuNo;    // [string,   12] 종목번호                        StartPos 0, Length 12
    CString    IsuNm               ;// 40];    // [string,   40] 종목명                          StartPos 12, Length 40
    CString    SecBalPtnCode       ;//  2];    // [string,    2] 유가증권잔고유형코드            StartPos 52, Length 2
    CString    SecBalPtnNm         ;// 40];    // [string,   40] 유가증권잔고유형명              StartPos 54, Length 40
    CString    BalQty              ;// 16];    // [long  ,   16] 잔고수량                        StartPos 94, Length 16
    CString    BnsBaseBalQty       ;// 16];    // [long  ,   16] 매매기준잔고수량                StartPos 110, Length 16
    CString    CrdayBuyExecQty     ;// 16];    // [long  ,   16] 금일매수체결수량                StartPos 126, Length 16
    CString    CrdaySellExecQty    ;// 16];    // [long  ,   16] 금일매도체결수량                StartPos 142, Length 16
    CString    SellPrc             ;// 21];    // [double, 21.4] 매도가                          StartPos 158, Length 21
    CString    BuyPrc              ;// 21];    // [double, 21.4] 매수가                          StartPos 179, Length 21
    CString    SellPnlAmt          ;// 16];    // [long  ,   16] 매도손익금액                    StartPos 200, Length 16
    CString    PnlRat              ;// 18];    // [double, 18.6] 손익율                          StartPos 216, Length 18
    CString    NowPrc              ;// 15];    // [double, 15.2] 현재가                          StartPos 234, Length 15
    CString    CrdtAmt             ;// 16];    // [long  ,   16] 신용금액                        StartPos 249, Length 16
    CString    DueDt               ;//  8];    // [string,    8] 만기일                          StartPos 265, Length 8
    CString    PrdaySellExecPrc    ;// 13];    // [double, 13.2] 전일매도체결가                  StartPos 273, Length 13
    CString    PrdaySellQty        ;// 16];    // [long  ,   16] 전일매도수량                    StartPos 286, Length 16
    CString    PrdayBuyExecPrc     ;// 13];    // [double, 13.2] 전일매수체결가                  StartPos 302, Length 13
    CString    PrdayBuyQty         ;// 16];    // [long  ,   16] 전일매수수량                    StartPos 315, Length 16
    CString    LoanDt              ;//  8];    // [string,    8] 대출일                          StartPos 331, Length 8
    CString    AvrUprc             ;// 13];    // [double, 13.2] 평균단가                        StartPos 339, Length 13
    CString    SellAbleQty         ;// 16];    // [long  ,   16] 매도가능수량                    StartPos 352, Length 16
    CString    SellOrdQty          ;// 16];    // [long  ,   16] 매도주문수량                    StartPos 368, Length 16
    CString    CrdayBuyExecAmt     ;// 16];    // [long  ,   16] 금일매수체결금액                StartPos 384, Length 16
    CString    CrdaySellExecAmt    ;// 16];    // [long  ,   16] 금일매도체결금액                StartPos 400, Length 16
    CString    PrdayBuyExecAmt     ;// 16];    // [long  ,   16] 전일매수체결금액                StartPos 416, Length 16
    CString    PrdaySellExecAmt    ;// 16];    // [long  ,   16] 전일매도체결금액                StartPos 432, Length 16
    CString    BalEvalAmt          ;// 16];    // [long  ,   16] 잔고평가금액                    StartPos 448, Length 16
    CString    EvalPnl             ;// 16];    // [long  ,   16] 평가손익                        StartPos 464, Length 16
    CString    MnyOrdAbleAmt       ;// 16];    // [long  ,   16] 현금주문가능금액                StartPos 480, Length 16
    CString    OrdAbleAmt          ;// 16];    // [long  ,   16] 주문가능금액                    StartPos 496, Length 16
    CString    SellUnercQty        ;// 16];    // [long  ,   16] 매도미체결수량                  StartPos 512, Length 16
    CString    SellUnsttQty        ;// 16];    // [long  ,   16] 매도미결제수량                  StartPos 528, Length 16
    CString    BuyUnercQty         ;// 16];    // [long  ,   16] 매수미체결수량                  StartPos 544, Length 16
    CString    BuyUnsttQty         ;// 16];    // [long  ,   16] 매수미결제수량                  StartPos 560, Length 16
    CString    UnsttQty            ;// 16];    // [long  ,   16] 미결제수량                      StartPos 576, Length 16
    CString    UnercQty            ;// 16];    // [long  ,   16] 미체결수량                      StartPos 592, Length 16
    CString    PrdayCprc           ;// 15];    // [double, 15.2] 전일종가                        StartPos 608, Length 15
    CString    PchsAmt             ;// 16];    // [long  ,   16] 매입금액                        StartPos 623, Length 16
    CString    RegMktCode          ;//  2];    // [string,    2] 등록시장코드                    StartPos 639, Length 2
    CString    LoanDtlClssCode     ;//  2];    // [string,    2] 대출상세분류코드                StartPos 641, Length 2
    CString    DpspdgLoanQty       ;// 16];    // [long  ,   16] 예탁담보대출수량                StartPos 643, Length 16

}Q12300INFO;

typedef struct _tagQ13700INFO{

	int checkflag;
	CString shcode;		//종목코드
	CString hname;		//이름
	CString price;		//가격

    CString    OrdDt;    // [string,    8] 주문일                          StartPos 0, Length 8
    CString    MgmtBrnNo;    // [string,    3] 관리지점번호                    StartPos 8, Length 3
    CString    OrdMktCode;    // [string,    2] 주문시장코드                    StartPos 11, Length 2
    CString    OrdNo;    // [long  ,   10] 주문번호                        StartPos 13, Length 10
    CString    OrgOrdNo;    // [long  ,   10] 원주문번호                      StartPos 23, Length 10
    CString    IsuNo;    // [string,   12] 종목번호                        StartPos 33, Length 12
    CString    IsuNm;    // [string,   40] 종목명                          StartPos 45, Length 40
    CString    BnsTpCode;    // [string,    1] 매매구분                        StartPos 85, Length 1
    CString    BnsTpNm;    // [string,   10] 매매구분                        StartPos 86, Length 10
    CString    OrdPtnCode;    // [string,    2] 주문유형코드                    StartPos 96, Length 2
    CString    OrdPtnNm;    // [string,   40] 주문유형명                      StartPos 98, Length 40
    CString    OrdTrxPtnCode;    // [long  ,    9] 주문처리유형코드                StartPos 138, Length 9
    CString    OrdTrxPtnNm;    // [string,   50] 주문처리유형명                  StartPos 147, Length 50
    CString    MrcTpCode;    // [string,    1] 정정취소구분                    StartPos 197, Length 1
    CString    MrcTpNm;    // [string,   10] 정정취소구분명                  StartPos 198, Length 10
    CString    MrcQty;    // [long  ,   16] 정정취소수량                    StartPos 208, Length 16
    CString    MrcAbleQty;    // [long  ,   16] 정정취소가능수량                StartPos 224, Length 16
    CString    OrdQty;    // [long  ,   16] 주문수량                        StartPos 240, Length 16
    CString    OrdPrc;    // [double, 15.2] 주문가격                        StartPos 256, Length 15
    CString    ExecQty;    // [long  ,   16] 체결수량                        StartPos 271, Length 16
    CString    ExecPrc;    // [double, 15.2] 체결가                          StartPos 287, Length 15
    CString    ExecTrxTime;    // [string,    9] 체결처리시각                    StartPos 302, Length 9
    CString    LastExecTime;    // [string,    9] 최종체결시각                    StartPos 311, Length 9
    CString    OrdprcPtnCode;    // [string,    2] 호가유형코드                    StartPos 320, Length 2
    CString    OrdprcPtnNm;    // [string,   40] 호가유형명                      StartPos 322, Length 40
    CString    OrdCndiTpCode;    // [string,    1] 주문조건구분                    StartPos 362, Length 1
    CString    AllExecQty;    // [long  ,   16] 전체체결수량                    StartPos 363, Length 16
    CString    RegCommdaCode;    // [string,    2] 통신매체코드                    StartPos 379, Length 2
    CString    CommdaNm;    // [string,   40] 통신매체명                      StartPos 381, Length 40
    CString    MbrNo;    // [string,    3] 회원번호                        StartPos 421, Length 3
    CString    RsvOrdYn;    // [string,    1] 예약주문여부                    StartPos 424, Length 1
    CString    LoanDt;    // [string,    8] 대출일                          StartPos 425, Length 8
    CString    OrdTime;    // [string,    9] 주문시각                        StartPos 433, Length 9
    CString    OpDrtnNo;    // [string,   12] 운용지시번호                    StartPos 442, Length 12
    CString    OdrrId;    // [string,   16] 주문자ID                        StartPos 454, Length 16
}Q13700INFO;

typedef struct _tagOrderList {
	int cnt;								
	Q13700INFO list[20];						
}ORDLIST;

//-----------------------------------------------------------------------------------------------
// 하나의 데이터를 저장하기 위한 구조체
struct ItemInfo {
	CString shcode;		//종목코드
	CString hname;		//이름
	CString price;		//가격
	CString search_gb;		//검색구분
	CString search_cd;		//검색코드
	CString number;   // 검색코드 number
	CString search_nm; //검색 이름
	CString diff;
	CString datetemp;
};

//-----------------------------------------------------------------------------------------------
// 하나의 종목 데이터를 저장하기 위한 구조체
struct SearchList {
	int number;								
	ItemInfo data[MAX_JONGMOK];						
};

//-----------------------------------------------------------------------------------------------
// 하나의 데이터를 저장하기 위한 구조체
struct Data {
	CString date;		//날짜
	LONG startVal;		//시가
	LONG highVal;		//고가
	LONG lowVal;		//저가
	LONG lastVal;		//종가
	LONG vol;			//거래량
};

//-----------------------------------------------------------------------------------------------
// 하나의 종목 데이터를 저장하기 위한 구조체
struct Company {
	CString strJongMok, strName;				//종목코드와 종목명을 저장한다.
	int quantity;								//해당 종목에 저장되는 데이터의 갯수를 저장한다.
	Data data;						//데이터를 저장하기 위한 구조체 배열
	CString keyToRecv;							//xing으로부터 데이터를 연속으로 받을 때 키를 저장한다.
};

//-----------------------------------------------------------------------------------------------
// 모든 데이터를 저장하기 위한 구조체로서 모든 종목의 배열을 포함한다.
struct AllCompany {
	int quantity;								//read number.
	Company company[MAX_JONGMOK];				//종목들을 저장하기 위한 구조체 배열
};

struct QData {
	CString shcode;	
	CString cur_date;
	CString hname;	
	CString sign;
	CString signcnt;
	CString curprice;	
	CString	chdata;
	CString diff;
	CString volume;
	CString volumerate;
	CString index;
	CString sms;
	CString fsms;
	CString rate;
	CString mstype;
	CString msg;
};
struct QInfo{
	int Qnum;								//read number.
	QData qitem[MAX_DATA];				//종목들을 저장하기 위한 구조체 배열
};

typedef struct _t1301item
{
    char    chetime             [  10]; // [string,   10] 시간                           StartPos 0, Length 10
    char    price               [   8]; // [long  ,    8] 현재가                         StartPos 11, Length 8
    char    sign                [   1]; // [string,    1] 전일대비구분                   StartPos 20, Length 1
    char    change              [   8]; // [long  ,    8] 전일대비                       StartPos 22, Length 8
    char    diff                [   6]; // [float ,  6.2] 등락율                         StartPos 31, Length 6
    char    cvolume             [  12]; // [long  ,   12] 체결수량                       StartPos 38, Length 12
    char    chdegree            [   8]; // [float ,  8.2] 체결강도                       StartPos 51, Length 8
    char    volume              [  12]; // [long  ,   12] 거래량                         StartPos 60, Length 12
    char    mdvolume            [  12]; // [long  ,   12] 매도체결수량                   StartPos 73, Length 12
    char    mdchecnt            [   8]; // [long  ,    8] 매도체결건수                   StartPos 86, Length 8
    char    msvolume            [  12]; // [long  ,   12] 매수체결수량                   StartPos 95, Length 12
    char    mschecnt            [   8]; // [long  ,    8] 매수체결건수                   StartPos 108, Length 8
    char    revolume            [  12]; // [long  ,   12] 순체결량                       StartPos 117, Length 12
    char    rechecnt            [   8]; // [long  ,    8] 순체결건수                     StartPos 130, Length 8
} t1301item, *pt1301item;

struct PATTENInfo {
	int		ms;
	int		fms;
	int	patten;
	__int64	msval;
	__int64	fmsval;

};

struct MaListInfo {
	CString shcode;		//종목코드
	CString hname;		//이름
	CString price;		//가격
	CString msprice;     //매수 가격
	CString totval;	  // 누적 금액
	CString volume;   //누적 거래량
	CString msvol;    //매수 수량
	CString totvol;     //total 거래량
	CString jvolume;   // 전일 동시간대 거래량.
	CString vol;		//체결 강도
	CString janginfo;	//장인포
	CString diff;
	CString sms;		//cdc순매수
	CString smsval;		//cdc순매수금액
	CString sinc;		//cdc순증감 
	CString fsms;		//외국계 매수
	CString fsmsval;	//외국계 순매수금액
	CString fsinc;		//외국계 순증감 
	CString frate;
	CString psms;		//프로그램 매수
	CString pmsval;		//프로그램 순매수금액
	CString pmsrank;
	CString psmd;		//프로그램 매도
	CString pmdval;		//프로그램 순도수금액
	CString pmdrank;
	CString twentyflag;		//20일선돌파
	CString newpriceflag;	//신고가
	CString newpricedate;   //전 시고가 date
	CString fsmsrank;		//외인 순 매수 상위
	CString valrate;  //매수금 비중(기관/외국계/프로그램)
	CString volrate;  //매수량(기관/외국계)

	int orderflag; //주문 flag

	CString mode1;
	CString mode2;
	int	chpattenP[MAX_PATTEN];
	int	chpattenM[MAX_PATTEN];
	PATTENInfo	PATP[MAX_PATTEN];
	PATTENInfo	PATM[MAX_PATTEN];

};

struct OUTList {
	int		outnumber;
	CString outlist[100];
};


struct MAList {
	int		itemnumer;
	int		search_cd_num;
	int		OrdNum;
	CString analyst;
	CString	analyet;
	CString analyperiod;
	CString analymin;
	CString	analymax;
	CString search_cd[10];
	MaListInfo data[MAX_DB_JONGMOK];						
};

struct T1301INFO {

	CString cschetime;
	CString csprice;
	CString csdiff;
	CString cschvol;
	CString cschdegree;
	CString csvol;
	CString csrevol;

};

struct T130LIST {
	int		itemnumer;
	T1301INFO item1301[300];
};

struct T1511INFO {
    CString csgubun;
	CString csshcode;
	CString cshname;
	CString cscur_date;
	CString cspricejisu;
	CString csdiffjisu;
	CString csprediff;
	CString csopendiff;
	CString cshighdiff;
	CString cshightime;
	CString cslowdiff;
	CString cslowtime;
	CString csfirstjname;
	CString csfirdiff;
	CString cssecondjname;
	CString cssecdiff;
	CString csthirdjname;
	CString csthrdiff;
	CString csfourthjname;
	CString csfordiff;
	CString cshighjo;
	CString csupjo;
	CString cslowjo;
	CString csdownjo;
};

struct T1511LIST {
	int		itemnumer;
	T1511INFO item1511[500];
};

struct T1602INFO{

	CString cscur_date;
	CString csupmode;
	CString cscstime;
	CString cssv_00;
	CString cssv_01;
	CString cssv_02;
	CString cssv_03;
	CString cssv_04;
	CString cssv_05;
	CString cssv_06;
	CString cssv_07;
	CString cssv_08;
	CString cssv_11;
	CString cssv_17;
	CString cssv_18;
};

struct T1602LIST {
	int		itemnumer;
	T1602INFO item1602[1000];
};

struct ETF_T1602INFO{
	T1602INFO t1602avg;
	T1602INFO t1602rate[2];
};

struct T1302INFO{
	CString cschetime;
	CString cschdegree;
	CString csrate;	
	CString csclose;
	CString csopen;
	CString cshi;
	CString cslow;
};

struct T1302AVGINFO{
	CString cschetime;
	CString cs5avg;
	CString cs10avg;
	CString cs20avg;
	CString cs60avg;
};

struct MGJMINFO{

	int mode;
	CString csmstime;
	CString csmdtime;
	CString csmdprice;
	CString csmsordprice;

	CString csshcode;
	CString csmdposqt;
	CString cssunikrt;
	CString cspamt;
	CString csmamt;
	CString csdtsunik;
	CString csappamt;
	CString csprice;
	CString csstprice;
	CString cshiprice;
	CString cslowprice;
	CString csuplmprice;
	CString csdnlmprice;
	CString csrate;
	CString csdegree;
	CString csvol;
	CString csmode;

	CString cspreopen;
	CString csprehi;
	CString csprelow;
	CString cspreclose;
	CString csprerate;
	CString csrevolume;
	CString csprerevolume;
	CString csremaxprice;
	CString csavg5;
	CString csavg20;
	CString csavg60;

	int remaxvolume;

	T1302INFO t1302[10];
	T1302AVGINFO t1302avg[5];

};


struct T1637LIST {
	int count;
	int index;
    CString shcodelist[1000];
};


class CETrade_Order_SampleApp : public CWinApp
{
public:
	CETrade_Order_SampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETrade_Order_SampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CETrade_Order_SampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETRADE_ORDER_SAMPLE_H__8D7CDC75_3FDA_4FE8_96BB_32CBE9BAD88A__INCLUDED_)

extern CETrade_Order_SampleApp theApp;
