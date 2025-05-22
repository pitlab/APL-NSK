
// APL-SNDoc.h: interfejs klasy CAPLSNDoc
#include "AnalizatorLogu.h"
//

#pragma once
#define ROZMIAR_BUFORA_ODCZYTU_LOGU		1024

class CAPLSNDoc : public CDocument
{
protected: // utwórz tylko na podstawie serializacji
	CAPLSNDoc() noexcept;
	DECLARE_DYNCREATE(CAPLSNDoc)

// Atrybuty
public:

// Operacje
public:
	uint16_t m_sZdjecie[480*320];
	bool m_bZdjecieGotowe;
	//std::vector <float> dane_telemetryczne;

// Przesłania
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	CAnalizatorLogu m_cAnalizatorLogu;
	std::vector <CAnalizatorLogu::stZmiennaLogu_t> m_vLog;		//zzmienna przechowująca odczytany log
	BOOL m_bOdczytanoLog;


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
public:
};
