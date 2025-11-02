// Serach_LiST.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Serach_LiST.h"

#include "ETrade_Order_Sample.h"

#include <afxdb.h>
#include <atlstr.h>
#include <stdio.h>
// CSerach_LiST 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSerach_LiST, CDialog)

CSerach_LiST::CSerach_LiST(CWnd* pParent /*=NULL*/)
	: CDialog(CSerach_LiST::IDD, pParent)
{

}

CSerach_LiST::~CSerach_LiST()
{
}

void CSerach_LiST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSerach_LiST, CDialog)
END_MESSAGE_MAP()


// CSerach_LiST 메시지 처리기입니다.
