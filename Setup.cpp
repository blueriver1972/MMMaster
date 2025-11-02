// Setup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"

#include "Setup.h"


// CSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetup, CDialog)

CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)

{

}

CSetup::~CSetup()
{
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


}


BEGIN_MESSAGE_MAP(CSetup, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSetup::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSetup::OnBnClickedOk)
//	ON_WM_INITMENU()

ON_EN_CHANGE(IDC_EDIT_JM_DIFF, &CSetup::OnEnChangeEditJmDiff)
END_MESSAGE_MAP()


// CSetup 메시지 처리기입니다.

void CSetup::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CSetup::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csindex, csval, csrate, cslevel, cssunik, csmaxsunik, cssonjel, csmaxsonjel;
	CString csquery;

	GetDlgItem(IDC_EDIT0)->GetWindowTextA(csindex);

	GetDlgItem(IDC_EDIT1)->GetWindowTextA(m_csCount);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(m_csCntPw);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(m_csID);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(m_csIdPw);
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(m_csPubVerify);


	GetDlgItem(IDC_EDIT_JM_VAL)->GetWindowTextA(csval);
	GetDlgItem(IDC_EDIT_JM_RATE)->GetWindowTextA(csrate);
	GetDlgItem(IDC_EDIT_JM_LEVEL)->GetWindowTextA(cslevel);


	GetDlgItem(IDC_EDIT_JM_DIFF)->GetWindowTextA(cssunik);
	GetDlgItem(IDC_EDIT_JM_DIFF2)->GetWindowTextA(csmaxsunik);
	GetDlgItem(IDC_EDIT_JM_DIFF3)->GetWindowTextA(cssonjel);
	GetDlgItem(IDC_EDIT_JM_DIFF4)->GetWindowTextA(csmaxsonjel);


	csquery.Format("REPLACE INTO cfg_tbl(idindex, countid, countpw, loginid, loginpw , verifypw, msvalue, msrate, mdlevel, sunik, maxsunik, sonjel, maxsonjel, allcount) VALUES ('%s', '%s', AES_ENCRYPT('%s', '%s'),\
				   '%s', AES_ENCRYPT('%s', '%s'),  AES_ENCRYPT('%s', '%s'), '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", csindex, m_csCount, m_csCntPw, "12345678",
				    m_csID, m_csIdPw, "23456781", m_csPubVerify, "34567812",  csval, csrate, cslevel, cssunik, csmaxsunik, cssonjel, csmaxsonjel, "3");

	ctrlDB.db_query_cmd(csquery);
	OnOK();
}

BOOL CSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_EDIT0)->SetWindowTextA("0");
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT3)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT4)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT5)->SetWindowTextA("");


	GetDlgItem(IDC_EDIT_JM_VAL)->SetWindowTextA("1000000");
	GetDlgItem(IDC_EDIT_JM_RATE)->SetWindowTextA("80");
	GetDlgItem(IDC_EDIT_JM_DIFF)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT_JM_LEVEL)->SetWindowTextA("3");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSetup::OnEnChangeEditJmDiff()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
