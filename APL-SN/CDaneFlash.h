#pragma once
#include "Komunikacja.h"

// Okno dialogowe CDaneFlash

class CDaneFlash : public CDialogEx
{
	DECLARE_DYNAMIC(CDaneFlash)
//#define ROZMIAR_SPISU_KOMUNIKATOW	256
#define ROZMIAR_SPISU_KOMUNIKATOW	8
#define ADRES_POCZATKU_KOMUNIKATOW	0x68020000
//#define ADRES_POCZATKU_KOMUNIKATOW	0x68030000
#define LICZBA_SEKTOROW_KOMUNIKATOW	6			//sektor to 128kB

	struct _SpisTresci
	{
		uint32_t nAdres;
		uint32_t nRozmiar;
	};
	struct _PlikWav
	{
		uint8_t chRiff[4];				//0..3		CHUNK ID "FIFF"
		uint32_t nRozmiarPliku;			//4..7		(file size) - 8
		uint8_t chWave[4];				//8..11		RIFF Type "WAVE"
		uint8_t chfmt[4];				//12..15	CHUNK ID "fmt "
		uint32_t nRozmiarFormatuDanych;	//16..19	16 + extra format bytes
		uint16_t sTypFormatu;			//20..21	compression code 1=uncompressed
		uint16_t sLiczbaKanalow;		//22..23
		uint32_t nCzestProbkowania;		//24..27
		uint32_t nBajtowNaSek;			//28..31
		uint16_t sBajtowNaProbke;		//32..33
		uint16_t sBitowNaProbke;		//34..35
		uint8_t chData[4];				//36..39
		uint32_t nLiczbaDanych;			//40..43	extra format bytes
	};

	union _uWav
	{
		_PlikWav strPlikWav;
		uint8_t chBufor[44];
	};

public:
	CDaneFlash(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CDaneFlash();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZAPISZ_FLASH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	CKomunikacja cKomunikacja;
	std::vector <uint16_t> m_vPamiecKomunikatow;	//przechowuje zawartosć jaka ma być wgrana do Flash
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButCzytajPlik();
	CListCtrl m_ctlListaKomunikatow;
	afx_msg void OnBnClickedButZapiszFlash();
	_SpisTresci m_SpisKomunikatow[ROZMIAR_SPISU_KOMUNIKATOW];
	_uWav m_uNaglowekWav;
	afx_msg void OnBnClickedButKasujFlash();
	CProgressCtrl m_ctlPasekPostepu;
};
