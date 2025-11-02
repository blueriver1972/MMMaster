// analyPatten.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"

#include "analyPatten.h"


// CanalyPatten 대화 상자입니다.

IMPLEMENT_DYNAMIC(CanalyPatten, CDialog)

CanalyPatten::CanalyPatten(CWnd* pParent /*=NULL*/)
	: CDialog(CanalyPatten::IDD, pParent)
	, m_Indexof(0), m_nSortColumn(0), m_bSortAscending(false)
	, m_nSMSval(0)
{

	m_brush.CreateSolidBrush(GRAY_BRUSH);
}

CanalyPatten::~CanalyPatten()
{

}

void CanalyPatten::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlMS);
	DDX_Control(pDX, IDC_LIST3, m_ctrlMD);
	DDX_Control(pDX, IDC_STATIC_MS, m_ctrlMSdata);
	DDX_Control(pDX, IDC_STATIC_MD, m_ctrlMDdata);
	DDX_Control(pDX, IDC_STATIC_SMS, m_ctrlSMSdata);
}


BEGIN_MESSAGE_MAP(CanalyPatten, CDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CanalyPatten::OnLvnColumnclickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CanalyPatten::OnLvnColumnclickList3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CanalyPatten::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST3, &CanalyPatten::OnNMCustomdrawList3)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CanalyPatten 메시지 처리기입니다.
extern ONEANAY   oneanaly;
BOOL CanalyPatten::OnInitDialog()
{
	CDialog::OnInitDialog();
	int width = 100;
	int i = 0, j = 0;
	CString csname;
	CString cs;
	COleDateTime dt = COleDateTime::GetCurrentTime();

	m_shcode = ctrlDB.getMatchID(m_hname, 1);
	cs.Format("%s:%s", m_hname, dt.Format("%Y-%m-%d %H:%M:%S"));
	SetWindowText(cs);

	//m_ctrlMS.SetExtendedStyle(LVS_EX_GRIDLINES|LVIF_TEXT);
	//m_ctrlMD.SetExtendedStyle(LVS_EX_GRIDLINES|LVIF_TEXT);



	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrlMS.InsertColumn(i++, _T("패턴횟수"), LVCFMT_CENTER, 100);
	m_ctrlMS.InsertColumn(i++, _T("패턴크기"), LVCFMT_CENTER, width);
	m_ctrlMS.InsertColumn(i++, _T("거래량"), LVCFMT_CENTER, width);
	m_ctrlMS.InsertColumn(i++, _T("증감"), LVCFMT_CENTER, width);


	i = 0;
	m_ctrlMD.InsertColumn(i++, _T("패턴횟수"), LVCFMT_CENTER, 110);
	m_ctrlMD.InsertColumn(i++, _T("패턴크기"), LVCFMT_CENTER, width);
	m_ctrlMD.InsertColumn(i++, _T("거래량"), LVCFMT_CENTER, width);
	m_ctrlMD.InsertColumn(i++, _T("증감"), LVCFMT_CENTER, width);

	m_ctrlMS.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrlMD.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	oneanaly.analyst.Empty();
	oneanaly.analyet.Empty();
	oneanaly.itemnumer = 0;

	for(i =  0; i < MAX_JONGMOK; i++){
		for(j = 0; j < MAX_PATTEN; j++){
			oneanaly.data.PATP[j].patten = 0;					
			oneanaly.data.PATP[j].ms = 0;
			oneanaly.data.PATP[j].msval = 0;
			oneanaly.data.chpattenP[j] = 0;

			oneanaly.data.PATM[j].patten = 0;				
			oneanaly.data.PATM[j].ms = 0;
			oneanaly.data.PATM[j].msval = 0;
			oneanaly.data.chpattenM[j] = 0;			
		}
	}
	oneanaly.analymin = m_minval;
	oneanaly.analymax = m_maxval;
	oneanaly.analyperiod = m_anyperiod;

	cs.Format("%02d%02d00", dt.GetHour(), dt.GetMinute());

	ctrlDB.calloneanaly(m_shcode, "090000", cs, oneanaly.analymin, oneanaly.analymax, 1);

	CallView(0);
	SetTimer(7000, 1000*60*2, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



CString CanalyPatten::CommaFormat(CString val)
{
	CString csval;

	char szBuffer[64];
	CString strFileSize;

	NUMBERFMT fmt = {0, 0, 3, _T("."), _T(","), 1};
	::GetNumberFormat(NULL, NULL, val, &fmt, szBuffer, sizeof(szBuffer));

	csval.Format("%s", szBuffer);

	return csval;
}


static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CListCtrl *pListCtrl = (CListCtrl*) lParamSort;
    
    LVFINDINFO info1, info2;
    info1.flags = LVFI_PARAM;
    info1.lParam = lParam1;
    info2.flags = LVFI_PARAM;
    info2.lParam = lParam2;
 
    // lParam1과 lParam2에 해당하는 아이템의 row를 얻는다
    // 이걸 바로 row값으로 생각해서 사용하면 sort를 계속 호출하면 처음엔 제대로 되고,
    // 두번째는 이상한 정렬, 그 다음은 정상, ...(반복)
    // 왜냐하면 처음에는 제대로 되지만, 그렇게 sort가 되고 난 후엔
    // row값이 변하기 때문에 다시 SetItemData로 row를 정해주어야 제대로 됩니다.
    // 확인하시려면 FindItem하지 말고 바로 GetItemText에 lParam을 넣어보면
    // 이상하게 작동하는걸 보실 수 있습니다
 
    int irow1 = pListCtrl->FindItem(&info1,-1);
    int irow2 = pListCtrl->FindItem(&info2,-1);
    
    CString strItem1 = pListCtrl->GetItemText(irow1, 2); //lParlam1의 값에 해당하는 row의 성적컬럼 
    CString strItem2 = pListCtrl->GetItemText(irow2, 2); //lParlam2의 값에 해당하는 row의 성적컬럼
     return strcmp(strItem1, strItem2);    
 }

static int CALLBACK CompareFunc1(LPARAM lParam1, LPARAM lParam2, LPARAM lParam)
{
   CString* pStr1 = (CString*)lParam1; // ItemData을 문자열로 형 변환
   CString* pStr2 = (CString*)lParam2;

   pStr1->Remove(',');
   pStr2->Remove(',');
   BOOL bSortAscending = (BOOL)lParam; // 정렬 방식

   int result = atoi(*pStr1) - atoi(*pStr2);//strcmp(*pStr1, *pStr2); // strcmp 로 비교해서

   if (bSortAscending == TRUE) // 오름 차순이면 그냥 리턴
	 return result;
   else // 아니면 반대로 리턴
	 return -result;

   return 0;
}
 




void CanalyPatten::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
   int iColumn = pNMListView->iSubItem; // iSubItem 이 선택된 Column 번호이다.
   if (iColumn == m_nSortColumn) { // m_nSortColumn 은 멤버 변수로 전에 정렬했던 Column 번호이다.
      m_bSortAscending = !m_bSortAscending; // 만약 이 Columns을 오름차순으로 정렬한거면, 내림차순으로 정렬한다.
   }
   else { // 전과 다른 Column이면 오름차순 정렬
      m_bSortAscending = TRUE;
   }
   m_nSortColumn = iColumn; // 정렬한 Column을 멤버변수로 저장.
   // 각 ItemData에 문자열을 넣어줘야 하므로
   int nItem = m_ctrlMS.GetItemCount(); // Item 갯수를 구하고
   CString** arStr = new CString*[nItem]; // 문자열 저장공간을 Item갯수 만큼 만든다.
   for (int i=0; i<nItem; i++) {
      arStr[i] = new CString(m_ctrlMS.GetItemText(i, iColumn)); // 문자열에 각 Item의 내용을 넣어주고
      m_ctrlMS.SetItemData(i, (LPARAM)arStr[i]); // ItemData에 넣준다.
   }
   // 첫 번째 인수는 위에서 만든 함수, 두 번째 인수는 정렬 방식
   m_ctrlMS.SortItems(CompareFunc1, (LPARAM)m_bSortAscending); // 정렬!
   for (int i=0; i<nItem; i++) // 정렬 끝났으면 아까 할당한 문자열 메모리를 다시 삭제
   delete arStr[i];
   delete []arStr;
}

void CanalyPatten::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
   int iColumn = pNMListView->iSubItem; // iSubItem 이 선택된 Column 번호이다.
   if (iColumn == m_nSortColumn) { // m_nSortColumn 은 멤버 변수로 전에 정렬했던 Column 번호이다.
      m_bSortAscending = !m_bSortAscending; // 만약 이 Columns을 오름차순으로 정렬한거면, 내림차순으로 정렬한다.
   }
   else { // 전과 다른 Column이면 오름차순 정렬
      m_bSortAscending = TRUE;
   }
   m_nSortColumn = iColumn; // 정렬한 Column을 멤버변수로 저장.
   // 각 ItemData에 문자열을 넣어줘야 하므로
   int nItem = m_ctrlMD.GetItemCount(); // Item 갯수를 구하고
   CString** arStr = new CString*[nItem]; // 문자열 저장공간을 Item갯수 만큼 만든다.
   for (int i=0; i<nItem; i++) {
      arStr[i] = new CString(m_ctrlMD.GetItemText(i, iColumn)); // 문자열에 각 Item의 내용을 넣어주고
      m_ctrlMD.SetItemData(i, (LPARAM)arStr[i]); // ItemData에 넣준다.
   }
   // 첫 번째 인수는 위에서 만든 함수, 두 번째 인수는 정렬 방식
   m_ctrlMD.SortItems(CompareFunc1, (LPARAM)m_bSortAscending); // 정렬!
   for (int i=0; i<nItem; i++) // 정렬 끝났으면 아까 할당한 문자열 메모리를 다시 삭제
   delete arStr[i];
   delete []arStr;


}

void CanalyPatten::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	CString cs;


	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	switch(pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT :   //그리기 전
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;       

	case CDDS_ITEMPREPAINT:    //아이템 그리기 전
		*pResult = CDRF_NOTIFYSUBITEMDRAW;    
		break;        

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:      //서브 아이템 다시 그려질때                 
		{     
			int nRow = (int)pLVCD->nmcd.dwItemSpec; // 행 인덱스를 가져옴
			int nSub = (int)pLVCD->iSubItem;               // 열 인덱스를 가져옴 

			//if(nRow == 0 && nSub == 2)
			//{
			//	pLVCD->clrTextBk = RGB(0,0,255);   //파랑 바탕
			//	pLVCD->clrText = RGB(255,0,0);     //빨강 글자
			//}			
			pLVCD->clrText = RGB(0,0,0);

			if(nSub == 3){ 
				cs = m_ctrlMS.GetItemText(nRow, nSub);	

				if( atoi(cs) < 0 ) //등락률
				{
					//pLVCD->clrTextBk = RGB(0,255,0);   //녹색 바탕
					pLVCD->clrText = RGB(0,0,255); //파랑 글자
				}
				else if(  atoi(cs) > 0){
					pLVCD->clrText = RGB(255,0,0); //빨강 글자
				}
			}

			*pResult = CDIS_DEFAULT;
			break; 
		}   //밑에 case  문이 하나 더 추가 되면서 괄호{}가 없으면 error C2360이 뜹니다.  
		//저 같은 경우 에러 안뜰 경우에도 case 문 처리할 때는 무조건 가로 쳐 넣음

		//밑에 bReDraw를 다르게 적용하고 테스트 해보십시요(ex: bReDraw를 아예 if문 밖으로 뻬는경우 등등)
		//break을 return으로 교체하고 하는 방법도 있음
	case CDDS_ITEMPOSTPAINT:   //다 그린후

		break;

	}    
}

void CanalyPatten::OnNMCustomdrawList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	CString cs;


	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	switch(pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT :   //그리기 전
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;       

	case CDDS_ITEMPREPAINT:    //아이템 그리기 전
		*pResult = CDRF_NOTIFYSUBITEMDRAW;    
		break;        

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:      //서브 아이템 다시 그려질때                 
		{     
			int nRow = (int)pLVCD->nmcd.dwItemSpec; // 행 인덱스를 가져옴
			int nSub = (int)pLVCD->iSubItem;               // 열 인덱스를 가져옴 

			//if(nRow == 0 && nSub == 2)
			//{
			//	pLVCD->clrTextBk = RGB(0,0,255);   //파랑 바탕
			//	pLVCD->clrText = RGB(255,0,0);     //빨강 글자
			//}			

			pLVCD->clrText = RGB(0,0,0);
			if(nSub == 3){ 
				cs = m_ctrlMS.GetItemText(nRow, nSub);	

				if( atoi(cs) < 0 ) //등락률
				{
					//pLVCD->clrTextBk = RGB(0,255,0);   //녹색 바탕
					pLVCD->clrText = RGB(0,0,255); //파랑 글자
				}
				else if(  atoi(cs) > 0){
					pLVCD->clrText = RGB(255,0,0); //빨강 글자
				}
			}

			*pResult = CDIS_DEFAULT;
			break; 
		}   //밑에 case  문이 하나 더 추가 되면서 괄호{}가 없으면 error C2360이 뜹니다.  
		//저 같은 경우 에러 안뜰 경우에도 case 문 처리할 때는 무조건 가로 쳐 넣음

		//밑에 bReDraw를 다르게 적용하고 테스트 해보십시요(ex: bReDraw를 아예 if문 밖으로 뻬는경우 등등)
		//break을 return으로 교체하고 하는 방법도 있음
	case CDDS_ITEMPOSTPAINT:   //다 그린후

		break;

	}    
}

HBRUSH CanalyPatten::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CString cs, cs1;

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.

	if(m_ctrlMSdata.m_hWnd == pWnd->m_hWnd){
		pDC->SetTextColor(RGB(255, 0, 0));
	}

	if(m_ctrlMDdata.m_hWnd == pWnd->m_hWnd){
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	if(m_ctrlSMSdata.m_hWnd == pWnd->m_hWnd){

		if(m_nSMSval > 0){
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if(m_nSMSval < 0){
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}

	return hbr;
}

void CanalyPatten::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);

	switch(nIDEvent)
	{
	case 7000:
		UpdateAnaly();
		break;

	}
}

int CanalyPatten::UpdateAnaly(void)
{
	int i = 0, j = 0;

	CString csname;
	CString cs;
	COleDateTime dt = COleDateTime::GetCurrentTime();

	m_shcode = ctrlDB.getMatchID(m_hname, 1);
	cs.Format("%s:%s", m_hname, dt.Format("%Y-%m-%d %H:%M:%S"));
	SetWindowText(cs);

	oneanaly.analyst.Empty();
	oneanaly.analyet.Empty();
	oneanaly.itemnumer = 0;

	for(i =  0; i < MAX_JONGMOK; i++){
		for(j = 0; j < MAX_PATTEN; j++){
			oneanaly.data.PATP[j].patten = 0;					
			oneanaly.data.PATP[j].ms = 0;
			oneanaly.data.PATP[j].msval = 0;
			oneanaly.data.chpattenP[j] = 0;

			oneanaly.data.PATM[j].patten = 0;				
			oneanaly.data.PATM[j].ms = 0;
			oneanaly.data.PATM[j].msval = 0;
			oneanaly.data.chpattenM[j] = 0;			
		}
	}
	oneanaly.analymin = m_minval;
	oneanaly.analymax = m_maxval;

	cs.Format("%02d%02d00", dt.GetHour(), dt.GetMinute());
	ctrlDB.calloneanaly(m_shcode, "090000", cs, oneanaly.analymin, oneanaly.analymax, 1);

	CallView(0);
	return 0;
}
