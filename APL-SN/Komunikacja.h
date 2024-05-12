#pragma once

#include "Protokol.h"

class CKomunikacja
{
public:
	CKomunikacja();	//noexcept;
	virtual ~CKomunikacja();


	void UstawTypPolaczenia(uint8_t chTyp) { m_chTypPolaczenia = chTyp; }
	uint8_t Polacz(CView* pWnd);
	uint8_t Rozlacz();
	void AkceptujPolaczenieETH();
	void OdebranoDaneETH();
	void WyslanoDaneETH();


	void UstawNumerPortuETH(uint32_t iNumer) { m_iNumerPortuETH = iNumer; }
	uint32_t PobierzNumerPortuETH() { return m_iNumerPortuETH; }
	void UstawAdresPortuETH(CString strAdres) { m_strAdresPortuETH = strAdres; }
	CString PobierzAdresPortuETH() { return m_strAdresPortuETH; }
	void UstawNumerPortuUART(uint32_t iNumer) { m_iNumerPortuUART = iNumer; }
	uint32_t PobierzNumerPortuUART() { return m_iNumerPortuUART; }
	void UstawPredkoscPortuUART(uint32_t iPredkosc) { m_iPredkoscUART = iPredkosc; }
	uint32_t PobierzPredkoscPortuUART() { return m_iPredkoscUART; }

	uint8_t WyslijSwojaNazwe(CString strNazwa);


private:
	CProtokol m_cProto;
	BOOL m_bPolaczono = FALSE;
	uint8_t m_chTypPolaczenia = ETHS;
	uint32_t m_iNumerPortuETH;
	CString m_strAdresPortuETH;
	uint32_t m_iNumerPortuUART;
	uint32_t m_iPredkoscUART;

	//pola publiczne z odbieranymi danymi
public:
	CString m_strNazwa;
};

