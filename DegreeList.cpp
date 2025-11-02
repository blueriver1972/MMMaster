// DegreeList.cpp : 구현 파일입니다.
//


#include "stdafx.h"
#include "ETrade_Order_Sample.h"

#include "DegreeList.h"

extern T130LIST t1301list;
// CDegreeList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDegreeList, CDialog)

CDegreeList::CDegreeList(CWnd* pParent /*=NULL*/)
	: CDialog(CDegreeList::IDD, pParent)
{

}

CDegreeList::~CDegreeList()
{
}

void CDegreeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlDegreeList);
}


BEGIN_MESSAGE_MAP(CDegreeList, CDialog)
	ON_BN_CLICKED(IDOK, &CDegreeList::OnBnClickedOk)
END_MESSAGE_MAP()


// CDegreeList 메시지 처리기입니다.

BOOL CDegreeList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int width = 57;
	int i = 0;
	m_ctrlDegreeList.InsertColumn(i++, _T("CheTime"), LVCFMT_RIGHT, width);
	m_ctrlDegreeList.InsertColumn(i++, _T("Price  "), LVCFMT_RIGHT, width);
	m_ctrlDegreeList.InsertColumn(i++, _T("Diff"), LVCFMT_RIGHT, 55);
	m_ctrlDegreeList.InsertColumn(i++, _T("Degree"), LVCFMT_RIGHT, width);
	m_ctrlDegreeList.InsertColumn(i++, _T("Rate"), LVCFMT_RIGHT, 55);
	m_ctrlDegreeList.InsertColumn(i++, _T("Rate1"), LVCFMT_RIGHT, 55);
	m_ctrlDegreeList.InsertColumn(i++, _T("Volume"), LVCFMT_RIGHT, width);
	m_ctrlDegreeList.InsertColumn(i++, _T("VolRate"), LVCFMT_RIGHT, width);
	m_ctrlDegreeList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CallDegrView(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CDegreeList::CallDegrView(int mode)
{

	CString cs, csMsg, cshcode, cshname, cspval, csquery, csdate;
	int rate = 0, ms = 0,  msinc = 0;
	__int64 msval = 0, totnum =0, totval = 0;
	int i = 0, j = 0, ix = 0, iy = 0;

	COleDateTime t = COleDateTime::GetCurrentTime();
	COleDateTimeSpan ts;

	m_ctrlDegreeList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|HDF_RIGHT);
	m_ctrlDegreeList.DeleteAllItems();

	if(mode == 0){

		for(i =  0; i < t1301list.itemnumer; i++){

			iy = 1;

			//chetime
			cs = t1301list.item1301[i].cschetime;	
			m_ctrlDegreeList.InsertItem(ix, cs);

			//price
			cs = t1301list.item1301[i].csprice;
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			//diff
			cs = t1301list.item1301[i].csdiff;
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			//degr
			cs = t1301list.item1301[i].cschdegree;
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			//degr rate
			cs.Format("%0.2f", atof(t1301list.item1301[i].cschdegree) - atof(t1301list.item1301[i+1].cschdegree) );
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			//chvol
			cs.Format("%0.2f", atof(t1301list.item1301[i].cschdegree) - atof(t1301list.item1301[i+3].cschdegree) );
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);


			//vol
			cs = t1301list.item1301[i].csvol;
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			//vol rate
			cs.Format("%d", atoi(t1301list.item1301[i].csvol) - atoi(t1301list.item1301[i+1].csvol));
			m_ctrlDegreeList.SetItemText(ix, iy++, cs);

			ix++;

		}

	}
	else{

	}

	//m_ctrlChart.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	return 1;
}



void CDegreeList::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}
