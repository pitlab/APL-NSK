
// APL-SNView.h: interfejs klasy CAPLSNView
//

#pragma once
#include "Protokol.h"
#include "Komunikacja.h"
#include "CDaneFlash.h"
#include "Rejestr.h"
#include "DekoderTelemetrii.h"

class CAPLSNView : public CView
{
protected: // utwórz tylko na podstawie serializacji
	CAPLSNView() noexcept;
	DECLARE_DYNCREATE(CAPLSNView)

// Atrybuty
public:
	CAPLSNDoc* GetDocument() const;
	static uint8_t WatekRysujPasekPostepu(LPVOID pParam);
	static uint8_t WatekInvalidujWytkresTelemetrii(LPVOID pParam);
	uint8_t WlasciwyWatekRysujPasekPostepu();
	uint8_t WlasciwyWatekInvalidujWytkresTelemetrii();

// Operacje
public:

// Przesłania
public:
	virtual void OnDraw(CDC* pDC);  // przesłonięty, aby narysować ten widok
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	CWinThread* pWskWatkuPaskaPostepu;
	CWinThread* pWskWatkuOdswiezaniaTelemetrii;

// Implementacja
public:
	virtual ~CAPLSNView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CKomunikacja m_cKomunikacja;
	CKonfigPolacz m_cKonfigPolacz;
	CDaneFlash m_cDaneFlash;
	CProtokol m_cProtokol;
	CDekoderTelemetrii m_cDekoderTelemetrii;
	BOOL m_bPolaczono;
	BOOL m_bKoniecWatkuPaskaPostepu;
	BOOL m_bKoniecWatkuOdswiezaniaTelemtrii;
	BOOL m_bRysujPasekPostepu;
	BOOL m_bRysujTelemetrie;
	uint16_t m_sLiczbaFragmentowPaskaPostepu;
	uint16_t m_sBiezacyStanPaskaPostepu;
	uint8_t m_chAdresAutopilota;
	CObslugaRejestru m_cObslugaRejestru;

public:
	uint32_t m_nNumerPortuCom;
	uint32_t m_nTypPolaczenia;	//0=UART, 1=ETH
	uint8_t m_chNumerIP[4];	
	uint32_t m_nPredkoscPortuCom;
	uint32_t m_nNumerPortuEth;

// Wygenerowano funkcje mapy komunikatów
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnKonfigPort();
	afx_msg void OnUpdateKonfigPort(CCmdUI* pCmdUI);
	afx_msg void OnPolaczCom();
	afx_msg void OnUpdatePolaczCom(CCmdUI* pCmdUI);
	afx_msg void OnZrobZdjecie();
	afx_msg void OnUpdateZrobZdjecie(CCmdUI* pCmdUI);
	afx_msg void OnZapiszPamiec();
	afx_msg void OnUpdateZapiszPamiec(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debuguj wersję w elemencie APL-SNView.cpp
inline CAPLSNDoc* CAPLSNView::GetDocument() const
   { return reinterpret_cast<CAPLSNDoc*>(m_pDocument); }
#endif

