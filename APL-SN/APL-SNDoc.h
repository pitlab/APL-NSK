
// APL-SNDoc.h: interfejs klasy CAPLSNDoc
//


#pragma once


class CAPLSNDoc : public CDocument
{
protected: // utwórz tylko na podstawie serializacji
	CAPLSNDoc() noexcept;
	DECLARE_DYNCREATE(CAPLSNDoc)

// Atrybuty
public:

// Operacje
public:
	uint16_t m_sZdjecie[320*240];
	bool m_bZdjecieGotowe;

// Przesłania
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementacja
public:
	virtual ~CAPLSNDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Wygenerowano funkcje mapy komunikatów
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Funkcja pomocnicza, która ustawia zawartość wyszukiwania dla procedury obsługi wyszukiwania
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
