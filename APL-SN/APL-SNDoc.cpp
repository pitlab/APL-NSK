
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
{
	// TODO: tutaj dodaj jednorazowy kod konstruowania

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
	if (ar.IsStoring())
	{
		// TODO: W tym miejscu dodaj kod przechowywania
	}
	else
	{
		// TODO: W tym miejscu dodaj kod ładujący
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
