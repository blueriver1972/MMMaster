// ListofLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"

#include "ListofLog.h"
#include <afxdb.h>
#include <atlstr.h>
#include <stdio.h>

T1511LIST   t1511list;
T1602LIST   t1602list;


// CListofLog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CListofLog, CDialog)

CListofLog::CListofLog(CWnd* pParent /*=NULL*/)
	: CDialog(CListofLog::IDD, pParent)
{

}

CListofLog::~CListofLog()
{
}

void CListofLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ctrlChart);
}


BEGIN_MESSAGE_MAP(CListofLog, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CListofLog::OnBnClickedOk)
END_MESSAGE_MAP()


// CListofLog 메시지 처리기입니다.

BOOL CListofLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int width = 0;
	int i = 0, j = 0;
	CString csquery, csname;
	CString cs;
	COleDateTime dt = COleDateTime::GetCurrentTime();

	CDatabase  m_db;
	CString  sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)";
	CString  sExcelFile = L"test.XLS";
	CString  sSql;



	//	m_shcode = ctrlDB.getMatchID(m_hname, 1);
	cs.Format("%s:%s", m_hname, dt.Format("%Y-%m-%d %H:%M:%S"));
	SetWindowText(cs);



	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(m_gMode == 2){
		width  = 40;

		m_ctrlChart.InsertColumn(i++, _T("  업종명"), LVCFMT_CENTER, 65);
		m_ctrlChart.InsertColumn(i++, _T("    시간"), LVCFMT_RIGHT, 120);
		m_ctrlChart.InsertColumn(i++, _T("현재지수  "), LVCFMT_RIGHT, 60);
		m_ctrlChart.InsertColumn(i++, _T("  %"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("시작"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("고가"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("고가시"), LVCFMT_RIGHT, 50);
		m_ctrlChart.InsertColumn(i++, _T("저가"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("저가시"), LVCFMT_RIGHT, 50);
		m_ctrlChart.InsertColumn(i++, _T("1st명"), LVCFMT_RIGHT, 70);
		m_ctrlChart.InsertColumn(i++, _T("1st%"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("2st명"), LVCFMT_RIGHT, 75);
		m_ctrlChart.InsertColumn(i++, _T("2st%"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("3st명"), LVCFMT_RIGHT, 70);
		m_ctrlChart.InsertColumn(i++, _T("3st%"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("4st명"), LVCFMT_RIGHT, 70);
		m_ctrlChart.InsertColumn(i++, _T("4st%"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("상승"), LVCFMT_RIGHT, 35);
		m_ctrlChart.InsertColumn(i++, _T("상한"), LVCFMT_RIGHT, 35);
		m_ctrlChart.InsertColumn(i++, _T("하락"), LVCFMT_RIGHT, 35);
		m_ctrlChart.InsertColumn(i++, _T("하한"), LVCFMT_CENTER, 35);

		m_ctrlChart.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


		CallChView(m_gMode);
	}
	else {
		width  = 60;
		//m_ctrlChart.InsertColumn(i++, _T("  업종명"), LVCFMT_CENTER, 80);
		if(m_gMode == 1){
			m_ctrlChart.InsertColumn(i++, _T("     Date"), LVCFMT_RIGHT, 120);
		}
		else{
			m_ctrlChart.InsertColumn(i++, _T("  시간"), LVCFMT_RIGHT, 80);
		}
		m_ctrlChart.InsertColumn(i++, _T("사모"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("증권"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("보험"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("투신"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("은행"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("종금"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("기금"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("기타"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("개인"), LVCFMT_RIGHT, width);
		m_ctrlChart.InsertColumn(i++, _T("국가"), LVCFMT_CENTER, width);
		m_ctrlChart.InsertColumn(i++, _T("외인"), LVCFMT_CENTER, width);
		m_ctrlChart.InsertColumn(i++, _T("기관"), LVCFMT_RIGHT, width);
		m_ctrlChart.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

		CallChView(m_gMode);
	}

	SetTimer(1000, 1000*60*1, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CListofLog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1000:

		break;

	}
	CDialog::OnTimer(nIDEvent);
}

int CListofLog::CallChView(int mode)
{

	CString cs, csMsg, cshcode, cshname, cspval, csquery, csdate;
	int rate = 0, ms = 0,  msinc = 0;
	__int64 msval = 0, totnum =0, totval = 0;
	int i = 0, j = 0, ix = 0, iy = 0;

	COleDateTime t = COleDateTime::GetCurrentTime();
	COleDateTimeSpan ts;

	m_ctrlChart.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|HDF_RIGHT);
	m_ctrlChart.DeleteAllItems();	

	if(mode == 2){

		for(i =  0; i < t1511list.itemnumer; i++){

			iy = 1;

			//date
			cs = t1511list.item1511[i].cshname;	
			m_ctrlChart.InsertItem(ix, cs);

			//date
			cs = t1511list.item1511[i].cscur_date;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//지수
			cs = t1511list.item1511[i].cspricejisu;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//등락비율
			cs = t1511list.item1511[i].csdiffjisu;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//시가
			cs = t1511list.item1511[i].csopendiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//최고가
			cs = t1511list.item1511[i].cshighdiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);


			//최고가 시간
			cs = t1511list.item1511[i].cshightime;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//최저가
			cs = t1511list.item1511[i].cslowdiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//최저가 시간
			cs = t1511list.item1511[i].cslowtime;
			m_ctrlChart.SetItemText(ix, iy++, cs);


			//1번째이름
			cs = t1511list.item1511[i].csfirstjname;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//1번째
			cs = t1511list.item1511[i].csfirdiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//2번째이름
			cs = t1511list.item1511[i].cssecondjname;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//2번째
			cs = t1511list.item1511[i].cssecdiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//3번째이름	
			cs = t1511list.item1511[i].csthirdjname;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//3번째
			cs = t1511list.item1511[i].csthrdiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);


			//4번째이름
			cs = t1511list.item1511[i].csfourthjname;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//4번째
			cs = t1511list.item1511[i].csfordiff;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//상승
			cs = t1511list.item1511[i].cshighjo;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//상한가
			cs = t1511list.item1511[i].csupjo;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//하락	
			cs = t1511list.item1511[i].cslowjo;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//하한가 
			cs = t1511list.item1511[i].csdownjo;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			ix++;

		}

	}
	else{
		for(i =  0; i < t1602list.itemnumer; i++){

			iy = 1;

			//time
			if(mode){
				cs = t1602list.item1602[i].cscur_date;
			}
			else{
				cs = t1602list.item1602[i].cscstime;
			}
			m_ctrlChart.InsertItem(ix, cs);

			//사모	
			cs = t1602list.item1602[i].cssv_00;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//증권
			cs = t1602list.item1602[i].cssv_01;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//보험
			cs = t1602list.item1602[i].cssv_02;
			m_ctrlChart.SetItemText(ix, iy++, cs);


			//투신
			cs = t1602list.item1602[i].cssv_03;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//은행
			cs = t1602list.item1602[i].cssv_04;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//종금
			cs = t1602list.item1602[i].cssv_05;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//기금
			cs = t1602list.item1602[i].cssv_06;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//기타
			cs = t1602list.item1602[i].cssv_07;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//개인
			cs = t1602list.item1602[i].cssv_08;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//국가
			cs = t1602list.item1602[i].cssv_11;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//외인	
			cs = t1602list.item1602[i].cssv_17;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			//기관계
			cs = t1602list.item1602[i].cssv_18;
			m_ctrlChart.SetItemText(ix, iy++, cs);

			ix++;

		}
	}

	//m_ctrlChart.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	return 1;
}


CString CListofLog::CommaFormat(CString val)
{
	CString csval;

	char szBuffer[64];
	CString strFileSize;

	NUMBERFMT fmt = {0, 0, 3, _T("."), _T(","), 1};
	::GetNumberFormat(NULL, NULL, val, &fmt, szBuffer, sizeof(szBuffer));

	csval.Format("%s", szBuffer);

	return csval;
}

void CListofLog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}
