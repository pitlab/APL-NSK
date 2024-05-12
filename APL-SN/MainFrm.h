
// MainFrm.h: interfejs klasy CMainFrame
//

#pragma once
//#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "GniazdoSieci.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Atrybuty
public:

// Operacje
public:

// Przesłania
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Implementacja
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // osadzone elementy członkowskie paska sterowania
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	//CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// Wygenerowano funkcje mapy komunikatów
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnPolacz();
	afx_msg void OnUpdatePolacz(CCmdUI* pCmdUI);
	afx_msg void OnWyslij();
	afx_msg void OnUpdateWyslij(CCmdUI* pCmdUI);
	afx_msg void OnZamknij();
	afx_msg void OnUpdateZamknij(CCmdUI* pCmdUI);
//	afx_msg void OnSerwerKlient();
//	afx_msg void OnUpdateSerwerKlient(CCmdUI* pCmdUI);

#define SERWER	1
#define KLIENT	2
	int m_iTypPolaczenia = SERWER;
	afx_msg void OnKlient();
	afx_msg void OnUpdateKlient(CCmdUI* pCmdUI);
	afx_msg void OnSerwer();
	afx_msg void OnUpdateSerwer(CCmdUI* pCmdUI);
	void OnAccept();
	void OnConnect();
	CString m_strKomunikatWych;
	BOOL m_bPolaczono;
};


