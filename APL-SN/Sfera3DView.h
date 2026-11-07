#pragma once


#define MAX_ZMIENNOSC	0.005
#define M_PI 	3.14159265358979323846


#include <afxwin.h>
class CSfera3DView :    public CView
{
    DECLARE_DYNCREATE(CSfera3DView)

protected:
    CSfera3DView();
    virtual ~CSfera3DView();
    DECLARE_MESSAGE_MAP()
    double m_dZoom = 0.5;
    CD2DSolidColorBrush* m_pBrushBlack;
    CD2DSolidColorBrush* m_pBrushGray;
    CD2DSolidColorBrush* m_pBrushR;
    CD2DSolidColorBrush* m_pBrushG;
    CD2DSolidColorBrush* m_pBrushB;
    CD2DTextFormat* m_pTextFormat;
    CD2DTextFormat* m_pTextFormatSkalaY;
    CD2DLinearGradientBrush* m_pLinearGradientBrush;

    BOOL m_bKoniecWatkuOdswiezaniaTelemtrii;
    BOOL m_bOknoGotowe;
    BOOL m_bObracanieMyszk¹;
    CWinThread* pWskWatkuOdswiezaniaSfery3D;
    CPoint m_ptPocz¹tkowaPozycjaMyszy;
    CPoint m_ptZmianaPozycjiMyszy;

public:
    //CBryla3DDoc* GetDocument() const;
    virtual void OnDraw(CDC* pDC);
    
protected:
    afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);
    static uint8_t WatekInvalidujStanSfery(LPVOID pParam);
    uint8_t WlasciwyWatekInvalidujStanSfery();
    void Obrot_wektora(float *wekt_we, float *wekt_wy, float *kat_obr);
    D2D1_COLOR_F UstawKolor(float fD³ugoœæWektora);


public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


