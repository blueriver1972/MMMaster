// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__77729D94_E2DC_465F_9AE1_F1E41C2A0175__INCLUDED_)
#define AFX_STDAFX_H__77729D94_E2DC_465F_9AE1_F1E41C2A0175__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// Visual Studio 2022 호환성을 위한 헤더
// afx.h가 new.h를 찾으려 하므로 _INC_NEW 매크로를 먼저 정의
#define _INC_NEW            // Visual Studio 2022: new.h 대신 <new> 사용
#include <new>              // new.h 대신 사용 (VS2022)
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions


//------------------------------------------------------------------------------------
#include "IXingAPI.h"
#include "XLEzAutomation.h"

#include "resource.h"		// main symbols

extern IXingAPI		g_iXingAPI;
extern	BOOL		g_bShowRaw;

#define WMU_SHOW_TR		( WM_USER + 1000 )
#define WMU_SET_MESSAGE	( WM_USER + 1001 )

enum
{
	DATA_TYPE_STRING = 0,		// ���ڿ�
	DATA_TYPE_LONG,				// ����
	DATA_TYPE_FLOAT,			// �Ǽ�
	DATA_TYPE_FLOAT_DOT,		// �Ǽ�( �Ҽ����� ������ ���� )
};
#define DATA_TYPE_DOUBLE	DATA_TYPE_FLOAT
#define DATA_TYPE_DOT		DATA_TYPE_FLOAT_DOT

typedef struct
{
	char			szMenu[3][50];
	char			szTrCode[ 11];
	CRuntimeClass*	pRuntimeClass;
	UINT			uDlgID;
	char			szDesc	[101];
} TR_WINDOW_TABLE, *LPTR_WINDOW_TABLE;
int					GetTRWindowTableCount();
LPTR_WINDOW_TABLE	GetTRWindowTableData();

CString GetDispData			( char* psData, int nSize, int nType, int nDotPos=0 );
void	SetPacketData		( char* psData, int nSize, LPCTSTR pszSrc, int nType, int nDotPos=0 );
void	SetPacketData		( char* psData, int nSize, CWnd* pWnd, int nID, int nType, int nDotPos=0 );
void	InsertColumn4List	( CWnd* pDlg, UINT nID, int nCol, LPCTSTR pszTitle, BOOL bString, double fLen );
CString GetDlgItemText		( CWnd* pWnd, UINT nID );
void	SetCtrlAccountList	( CComboBox* pCtrl, LPCTSTR pszGoods );


#include "./Packet/CSPAT00600.h"//����
#include "./Packet/CSPAT00700.h"//����
#include "./Packet/CSPAT00800.h"//���
#include "./Packet/CSPBQ00200.h"//���ɼ� ��ȸ
#include "./Packet/CSPAQ03700.h"//�ֹ�ü�� ���� ��ȸ
#include "./Packet/CSPAQ02300.h"//�������� �ܰ����� ��ȸ
#include "./Packet/CSPAQ02200.h"//�������� �ܰ����� ��ȸ

#include "./Packet/t0424.h"//�ܰ�
#include "./Packet/t0167.h"//time

#include "./Packet/t0425.h"//ü��/��ü��
#include "./Packet/t1102.h"//���簡(�ü�) ��ȸ
#include "./Packet/t1532.h"//�����׸�

#include "./Packet/t8430.h"//���� ��ȸ
#include "./Packet/t1825.h"//����QŬ���˻�
#include "./Packet/t1826.h"//����QŬ���˻�����Ʈ

#include "./Packet/t1302.h"//������ȸ ��ȸ
#include "./Packet/t1305.h"//�к��ְ� ��ȸ
#include "./Packet/t1301.h"//�ð��뺰
#include "./Packet/t1702.h"//����/������� ����
#include "./Packet/t1442.h"//�ýŰ�����
#include "./Packet/t1444.h"//�ð��Ѿ�
#include "./Packet/t1602.h"//�����ں� �Ÿŵ���
#include "./Packet/t1636.h"//���α׷�
#include "./Packet/t1637.h"//�������α׷�

//------------------------------------------------------------------------------------

#if 1
#define STR_ID				"k8096"
#define STR_PASSWORD_ID		"kim1972"
#define STR_PASSWORD_CERTI	"kim-1923717"
#define ACCOUNT_NUM			"20080800101"
#define ACCOUNT_NUM1		"20131719901"
#define ACCOUNT_SONG		"20134486801"

//#define MOEU_NUM			"55501016667" //k8096 / danbi0131 
//#define MOEU_NUM			"55501016665" // -������
#else
#define STR_ID				"kim0131"
#define STR_PASSWORD_ID		"kim1972"
#define STR_PASSWORD_CERTI	"kim-1923717"
#define ACCOUNT_NUM			"20115767801"//danbi
#endif

#define   K8096	    1


#define PASSWORD_NUM		"1972"
#define PASSWORD_SONG		"2698"


#define _CSPAT00600	_T("CSPAT00600")//�ֹ�
#define _CSPAT00700	_T("CSPAT00700")//����
#define _CSPAT00800	_T("CSPAT00800")//���
#define _CSPBQ00200	_T("CSPBQ00200")//���ɼ� ��ȸ
#define _CSPAQ03700	_T("CSPAQ03700")//�ֹ�ü�� ���� ��ȸ
#define _CSPAQ12200	_T("CSPAQ12200")

#define _T0424		_T("t0424") //�ܰ�
#define _T0425		_T("t0425") //ü��/��ü��
#define _T1102		_T("t1102") //���簡(����) ��ȸ
#define _T8430		_T("t8430") //������ȸ ��ȸ
#define _T1302		_T("t1302") //�к��ְ� ��ȸ
#define _T1305		_T("t1305") //�Ǻ�
#define _T1301		_T("t1301") //�ð��� �ְ�
#define _T1636		_T("t1636") //���α׷� ����
#define _T1602		_T("t1602") //�����ں� �Ÿŵ���

#define _T1702		_T("t1702") //�ܱ��α���ż�
#define _T1442		_T("t1442") //�Ű�����
#define _T1532		_T("t1532") //�׸�

#define _T1444		_T("t1444") //�ð��Ѿ�
#define _T1825		_T("t1825") //����QŬ���˻�
#define _T1826		_T("t1826") //����QŬ���˻�����Ʈ

#define _T1637		_T("t1637") //���� ���α׷�
#define _T0167		_T("t0167") //time

#define ST_TP_ALL	"0"
#define ST_TP_KSP	"1"
#define ST_TP_ASD	"2"


extern int m_gnLogonFlag;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_STDAFX_H__77729D94_E2DC_465F_9AE1_F1E41C2A0175__INCLUDED_)
