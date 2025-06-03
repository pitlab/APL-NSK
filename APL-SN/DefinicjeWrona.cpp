// DefinicjeWrona.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "DefinicjeWrona.h"
#include "afxdialogex.h"
#include "komunikacja/Komunikacja.h"

// Okno dialogowe DefinicjeWrona

IMPLEMENT_DYNAMIC(DefinicjeWrona, CDialogEx)

DefinicjeWrona::DefinicjeWrona(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DEFINICJE_WRONA, pParent)
	, m_strNumer(_T(""))
	, m_strNazwa(_T(""))
	, m_dNumIP(0)
{

}

DefinicjeWrona::~DefinicjeWrona()
{
}

void DefinicjeWrona::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUMER, m_strNumer);
	DDX_Text(pDX, IDC_EDIT_NAZWA, m_strNazwa);
	DDX_IPAddress(pDX, IDC_IPADDRESS_DEFINICJE, m_dNumIP);
}


BEGIN_MESSAGE_MAP(DefinicjeWrona, CDialogEx)
	ON_BN_CLICKED(IDOK, &DefinicjeWrona::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_NUMER, &DefinicjeWrona::OnEnChangeEditNumer)
	ON_EN_CHANGE(IDC_EDIT_NAZWA, &DefinicjeWrona::OnEnChangeEditNazwa)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_DEFINICJE, &DefinicjeWrona::OnIpnFieldchangedIpaddressDefinicje)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów DefinicjeWrona




BOOL DefinicjeWrona::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int nIndeksWrona;
	int chNumIP[4];

	// TODO:  Dodaj tutaj dodatkową inicjację
	nIndeksWrona = getKomunikacja().m_cRoj.m_nIndeksWrona;
	m_strNumer.Format(_T("%d"), getKomunikacja().m_cRoj.vWron[nIndeksWrona].m_chAdres);
	m_strNazwa = getKomunikacja().m_cRoj.vWron[nIndeksWrona].m_strNazwa;

	m_dNumIP = 0;
	for (int n = 0; n < 4; n++)
	{
		chNumIP[n] = getKomunikacja().m_cRoj.vWron[nIndeksWrona].m_chAdresIP[n];
		m_dNumIP += chNumIP[n] << (3-n)*8;
 	}
	m_bZmieniono = FALSE;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



void DefinicjeWrona::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	if (m_bZmieniono)
	{
		int n, nIndeksWrona = getKomunikacja().m_cRoj.m_nIndeksWrona;
		uint8_t chAdresWrona = getKomunikacja().m_cRoj.vWron[nIndeksWrona].m_chAdres;
		uint8_t chNumIP[4];
		uint8_t chNazwa[DLUGOSC_NAZWY];
		int nDlugosc = m_strNazwa.GetLength();
		if (nDlugosc > DLUGOSC_NAZWY)
			nDlugosc = DLUGOSC_NAZWY;

		for (n = 0; n < 4; n++)
			chNumIP[n] = getKomunikacja().m_cRoj.vWron[nIndeksWrona].m_chAdresIP[n];
		for (n = 0; n < nDlugosc; n++)
			chNazwa[n] = (char)m_strNazwa.GetAt(n);
		for (; n < DLUGOSC_NAZWY; n++)		//dopełnij nazwę zerami
			chNazwa[n] = 0;

		getKomunikacja().UstawBSP(chAdresWrona, chNazwa, chNumIP);
	}
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki adresu
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DefinicjeWrona::OnEnChangeEditNumer()
{
	uint8_t chNumer;

	UpdateData(TRUE);
	chNumer = (uint8_t)_wtoi(m_strNumer);
	getKomunikacja().m_cRoj.vWron[getKomunikacja().m_cRoj.m_nIndeksWrona].m_chAdres = chNumer;
	m_bZmieniono = TRUE;
}


void DefinicjeWrona::OnEnChangeEditNazwa()
{
	UpdateData(TRUE);
	getKomunikacja().m_cRoj.vWron[getKomunikacja().m_cRoj.m_nIndeksWrona].m_strNazwa = m_strNazwa;
	m_bZmieniono = TRUE;
}


void DefinicjeWrona::OnIpnFieldchangedIpaddressDefinicje(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	getKomunikacja().m_cRoj.vWron[getKomunikacja().m_cRoj.m_nIndeksWrona].m_chAdresIP[pIPAddr->iField] = pIPAddr->iValue;
	m_bZmieniono = TRUE;
	*pResult = 0;
}
