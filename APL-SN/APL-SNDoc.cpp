
// APL-SNDoc.cpp: implementacja klasy CAPLSNDoc
//

#include "pch.h"
#include "framework.h"
// Element SHARED_HANDLERS można zdefiniować w projekcie ATL z implementacją podglądu, miniaturze
// procedury obsługi serializacji i filtrów wyszukiwania oraz umożliwia udostępnianie kodu dokumentu w tym projekcie.
#ifndef SHARED_HANDLERS
#include "APL-SN.h"
#endif

#include "APL-SNDoc.h"
#include "APL-SNView.h"
#include "Errors.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAPLSNDoc

IMPLEMENT_DYNCREATE(CAPLSNDoc, CDocument)

BEGIN_MESSAGE_MAP(CAPLSNDoc, CDocument)
END_MESSAGE_MAP()


// Tworzenie/niszczenie obiektu CAPLSNDoc

CAPLSNDoc::CAPLSNDoc() noexcept
	: m_bZdjecieGotowe(FALSE)
	, m_bFFTGotowe(FALSE)
	, m_bOdczytanaMapaNMT(FALSE)
{
	// TODO: tutaj dodaj jednorazowy kod konstruowania
	//m_sZdjecie[0] = 0;

}

CAPLSNDoc::~CAPLSNDoc()
{
}

BOOL CAPLSNDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Dodaj tutaj kod ponownego inicjowania
	// (Dokumenty SDI będą ponownie używać tego dokumentu)

	return TRUE;
}




// Serializacja klasy CAPLSNDoc

void CAPLSNDoc::Serialize(CArchive& ar)
{
	CFile file;
	CHAR szBuf[ROZMIAR_BUFORA_ODCZYTU_LOGU];
	uint8_t chRead[ROZMIAR_BUFORA_ODCZYTU_LOGU];
	CString string, strNazwaPliku;
	CFileException ex;
	UINT nOdczytano;
	for (int n = 0; n < 512; n++)
		szBuf[n] = n;

	if (ar.IsStoring())
	{
		// TODO: W tym miejscu dodaj kod przechowywania
		if (file.Open(_T("CArchive__test__file.txt"), CFile::modeCreate | CFile::modeWrite))
		{
			CArchive ar(&file, CArchive::store, 512, szBuf);
			// Write a string to the archive.
			ar.WriteString(_T("My string."));
			if (ar.m_pDocument != NULL)
				ar.m_pDocument->Serialize(ar);
			// Flush all of the data to the file.
			ar.Flush();
			ar.Close();
		}
		
	}
	else
	{
		if (file.Open(ar.m_strFileName, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			//trzeba znaleźć metodę rozróżniajacą plik logu od pliku mapy i wywołać odpowiednią funkcję
			CString strNazwaPliku = file.GetFileName();
			int nPozyjaKropki = strNazwaPliku.Find(_T("asc"), 0);
			if (strNazwaPliku.Find(_T("asc"), 0) > 0)
			{
				//obsługa pliku mapy
				m_cMapaWysokosciowa.m_bAnalizaNaglowka = TRUE;
				m_cMapaWysokosciowa.m_bPierwszeWażneDane = TRUE;
				m_cMapaWysokosciowa.m_cIndeksZmiennej = 0;
				CArchive ar(&file, CArchive::load, ROZMIAR_BUFORA_ODCZYTU_LOGU, szBuf);				
				if (strNazwaPliku.Find(_T("asc"), 0) > 0)
					do {
						nOdczytano = ar.Read(chRead, ROZMIAR_BUFORA_ODCZYTU_LOGU);
						m_cMapaWysokosciowa.Analizuj(chRead, nOdczytano, &m_stNMT);
					} while (nOdczytano);
					ar.Close();
					m_bOdczytanaMapaNMT = TRUE;
			}

			if (strNazwaPliku.Find(_T("log"), 0) > 0)
			{
				//obsługa pliku logu
				CArchive ar(&file, CArchive::load, ROZMIAR_BUFORA_ODCZYTU_LOGU, szBuf);
				if (strNazwaPliku.Find(_T("asc"), 0) > 0)
					do {
						nOdczytano = ar.Read(chRead, ROZMIAR_BUFORA_ODCZYTU_LOGU);
						m_cAnalizatorLogu.Analizuj(chRead, nOdczytano, m_vLog);
					} while (nOdczytano);
					ar.Close();
					m_bOdczytanoLog = TRUE;
			}
			UpdateAllViews(NULL);
		}
	}
}

#ifdef SHARED_HANDLERS

// Obsługa miniatur
void CAPLSNDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Zmodyfikuj ten kod pod kątem rysowania danych dokumentu
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Obsługa procedur obsługi wyszukiwania
void CAPLSNDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Ustaw zawartość wyszukiwania na podstawie danych dokumentu.
	// Części zawartości powinny być rozdzielone znakami „;”

	// Na przykład: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAPLSNDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Diagnostyka klasy CAPLSNDoc

#ifdef _DEBUG
void CAPLSNDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAPLSNDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Polecenia CAPLSNDoc


