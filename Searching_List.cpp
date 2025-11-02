// Searching_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Searching_List.h"
#include "ETrade_Order_Sample.h"
#include "ETrade_Order_Sample.h"

// CSearching_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSearching_List, CDialog)


T1511LIST listitme;

CSearching_List::CSearching_List(CWnd* pParent /*=NULL*/)
	: CDialog(CSearching_List::IDD, pParent)
{

}

CSearching_List::~CSearching_List()
{
}

void CSearching_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}


BEGIN_MESSAGE_MAP(CSearching_List, CDialog)
	ON_BN_CLICKED(IDOK, &CSearching_List::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CSearching_List::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int width = 0;
	int i = 0, j = 0;
	CString csquery, csname;
	CString cs;
	COleDateTime dt = COleDateTime::GetCurrentTime();


	CString  sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)";
	CString  sExcelFile = L"test.XLS";
	CString  sSql;



	width  = 100;


	m_ctrlList.InsertColumn(i++, _T("종목"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("현재가"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("등락률"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("거래량"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("시가"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("고가"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("고가시간"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("저가"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("저가시간"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("개인"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("국가"), LVCFMT_CENTER, width);
	m_ctrlList.InsertColumn(i++, _T("외인"), LVCFMT_CENTER, width);
	m_ctrlList.InsertColumn(i++, _T("기관"), LVCFMT_RIGHT, width);
	m_ctrlList.InsertColumn(i++, _T("기타"), LVCFMT_RIGHT, width);
	m_ctrlList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CallChView(0);


	SetTimer(1000, 1000*60*1, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSearching_List::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1000:

		break;

	}
	CDialog::OnTimer(nIDEvent);
}

int CSearching_List::CallChView(int mode)
{

	CString cs, csMsg, cshcode, cshname, cspval, csquery, csdate;
	int rate = 0, ms = 0,  msinc = 0;
	__int64 msval = 0, totnum =0, totval = 0;
	int i = 0, j = 0, ix = 0, iy = 0;

	COleDateTime t = COleDateTime::GetCurrentTime();
	COleDateTimeSpan ts;

	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|HDF_RIGHT);
	m_ctrlList.DeleteAllItems();	



	for(i =  0; i < listitme.itemnumer; i++){

		iy = 1;

		//종목
		cs = listitme.item1511[i].cshname;	
		m_ctrlList.InsertItem(ix, cs);

		//현재가
		cs = listitme.item1511[i].cspricejisu;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//등락비율
		cs = listitme.item1511[i].csdiffjisu;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//시가
		cs = listitme.item1511[i].csopendiff;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//최고가
		cs = listitme.item1511[i].cshighdiff;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//최고가 시간
		cs = listitme.item1511[i].cshightime;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//최저가
		cs = listitme.item1511[i].cslowdiff;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//최저가 시간
		cs = listitme.item1511[i].cslowtime;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//개인
		cs = listitme.item1511[i].cshightime;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//기관
		cs = listitme.item1511[i].cslowdiff;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//외국인
		cs = listitme.item1511[i].cslowtime;
		m_ctrlList.SetItemText(ix, iy++, cs);

		//기타
		cs = listitme.item1511[i].cslowtime;
		m_ctrlList.SetItemText(ix, iy++, cs);

		ix++;

	}
	

	//m_ctrlChart.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	return 1;
}


CString CSearching_List::CommaFormat(CString val)
{
	CString csval;

	char szBuffer[64];
	CString strFileSize;

	NUMBERFMT fmt = {0, 0, 3, _T("."), _T(","), 1};
	::GetNumberFormat(NULL, NULL, val, &fmt, szBuffer, sizeof(szBuffer));

	csval.Format("%s", szBuffer);

	return csval;
}


// CSearching_List 메시지 처리기입니다.

void CSearching_List::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

