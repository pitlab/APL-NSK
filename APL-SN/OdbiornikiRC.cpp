// OdbiornikiRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "OdbiornikiRC.h"
#include "afxdialogex.h"


// Okno dialogowe OdbiornikiRC

IMPLEMENT_DYNAMIC(OdbiornikiRC, CDialogEx)

OdbiornikiRC::OdbiornikiRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ODBIORNIKI_RC, pParent)
	, m_bPPM1(FALSE)
	, m_bSBus1(FALSE)
	, m_bPPM2(FALSE)
	, m_bSBus2(FALSE)
{

}

OdbiornikiRC::~OdbiornikiRC()
{
}

void OdbiornikiRC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_PPM1, m_bPPM1);
	DDX_Radio(pDX, IDC_RADIO_SBUS1, m_bSBus1);
	DDX_Radio(pDX, IDC_RADIO_PPM2, m_bPPM2);
	DDX_Radio(pDX, IDC_RADIO_SBUS2, m_bSBus2);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlRC1Kan1);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctlRC1Kan2);
	DDX_Control(pDX, IDC_PROGRESS3, m_ctlRC1Kan3);
	DDX_Control(pDX, IDC_PROGRESS4, m_ctlRC1Kan4);
	DDX_Control(pDX, IDC_PROGRESS5, m_ctlRC1Kan5);
	DDX_Control(pDX, IDC_PROGRESS6, m_ctlRC1Kan6);
	DDX_Control(pDX, IDC_PROGRESS7, m_ctlRC1Kan7);
	DDX_Control(pDX, IDC_PROGRESS8, m_ctlRC1Kan8);
	DDX_Control(pDX, IDC_PROGRESS9, m_ctlRC1Kan9);
	DDX_Control(pDX, IDC_PROGRESS10, m_ctlRC1Kan10);
	DDX_Control(pDX, IDC_PROGRESS11, m_ctlRC1Kan11);
	DDX_Control(pDX, IDC_PROGRESS12, m_ctlRC1Kan12);
	DDX_Control(pDX, IDC_PROGRESS13, m_ctlRC1Kan13);
	DDX_Control(pDX, IDC_PROGRESS14, m_ctlRC1Kan14);
	DDX_Control(pDX, IDC_PROGRESS15, m_ctlRC1Kan15);
	DDX_Control(pDX, IDC_PROGRESS16, m_ctlRC1Kan16);
}


BEGIN_MESSAGE_MAP(OdbiornikiRC, CDialogEx)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OdbiornikiRC


BOOL OdbiornikiRC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Dodaj tutaj dodatkową inicjację

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}
