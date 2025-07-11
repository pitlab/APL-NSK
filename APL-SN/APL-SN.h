﻿
// APL-SN.h: główny plik nagłówkowy aplikacji APL-SN
//
#pragma once

#ifndef __AFXWIN_H__
	#error "dołącz nagłówek „pch.h” przed dołączeniem tego pliku na potrzeby optymalizacji PCH"
#endif

#include "resource.h"       // główne symbole
#include "KonfigPolacz.h"
#include "komunikacja/PortSzeregowy.h"
#include "komunikacja/Protokol.h"
#include "komunikacja/Komunikacja.h"

// CAPLSNApp:
// Aby uzyskać implementację klasy, zobacz APL-SN.cpp
//

class CAPLSNApp : public CWinAppEx
{
public:
	CAPLSNApp() noexcept;


// Przesłania
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CPortSzeregowy& getPortSzeregowy();
	CProtokol& getProtokol();
	CKomunikacja& getKomunikacja();

// Implementacja
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


	private:	
	CPortSzeregowy m_cPortSzeregowy;
	CProtokol m_cProtokol;
	CKomunikacja m_cKomunikacja;

public:
	afx_msg void OnUstawieniaRegulatoryPid();
	afx_msg void OnUpdateUstawieniaRegulatoryPid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorPolacz(CCmdUI* pCmdUI);
	afx_msg void OnUstawieniaDefinicjewrona();
	afx_msg void OnUpdateUstawieniaDefinicjewrona(CCmdUI* pCmdUI);
	afx_msg void OnUstawieniaMikser();
	afx_msg void OnUpdateUstawieniaMikser(CCmdUI* pCmdUI);
	afx_msg void OnUstawieniaOdbiornikirc();
	afx_msg void OnUpdateUstawieniaOdbiornikirc(CCmdUI* pCmdUI);
};

extern CAPLSNApp theApp;
