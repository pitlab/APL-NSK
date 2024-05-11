
//#include "stdafx.h"
#include "pch.h"
#include "Errors.h"
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "RS232.h"
#include <atlstr.h>

CPortSzeregowy::CPortSzeregowy() :
	m_bConnected( FALSE ),
	m_hComHandle( NULL ),
	m_iComNr( 1 )
{
	memset( &m_Overlap, 0, sizeof m_Overlap );
}

CPortSzeregowy::~CPortSzeregowy()
{
	Disconnect();
}

////////////////////////////////////////////////////////////////////////////
// W tym pliku zawarte s� procedury obs�ugi warstwy fizycznej transmisji RS232 
// operuj�ce bezposrednio na porcie szeregowym, wysy�aj�ce i odbieraj�ce dane 
// bez wzgl�du na rodzaj informacji jakie one zawieraja.
/////////////////////////////////////////////////////////////////////////////*/


//otw�rz port szeregowy
unsigned char CPortSzeregowy::OpenPort(int ComNr, HANDLE *hComHandle)
{
	CString ComName;

	ComName.Format(_T("\\\\.\\COM%d"), ComNr);
	*hComHandle = CreateFile(ComName,
					GENERIC_READ | GENERIC_WRITE,
					0,						//exclusive access 
					NULL,					//no security attributes 
					OPEN_EXISTING,			//musi by� OPEN_EXISTING dla portu szeregowego
					//NULL,
					FILE_FLAG_OVERLAPPED,	//overlapped
					NULL);					//musi by� NULL dla portu szeregowego

	if (*hComHandle == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError();
		return ERR_OPEN_PORT;
	}

    return ERR_OK;
}




//ustaw numer portu szeregowego
void CPortSzeregowy::SetNrCom(int ComNr)
{
	m_iComNr = ComNr;
}


//zwr�� numer portu szeregowego
int CPortSzeregowy::GetComPortNr()
{
	return m_iComNr;
}


//zwr�c stan po��czenia do portu
BOOL CPortSzeregowy::GetConnectedState()
{
	return m_bConnected;
}



//po��cz do portu szereowego
unsigned char  CPortSzeregowy::Connect(int nComPort, int nBaudRate)
{
	unsigned char chErr;

	if (m_bConnected)
		return ERR_CONNECTED;

	BOOL Result;
	DWORD dwInQueue;	// size of input buffer	
	DWORD dwOutQueue;	// size of output buffer
	DCB dcb;			//device-control block
	COMMTIMEOUTS CommTimeouts;
	m_iComNr = nComPort;

	chErr = OpenPort(nComPort, &m_hComHandle);
	if(chErr == ERR_OK)
	{
		//ustaw rozmiary bufora nadawczego i odbiorczego
		dwInQueue = 16*1024;
		dwOutQueue = 4*1024;
		Result = SetupComm(m_hComHandle, dwInQueue, dwOutQueue);

		Result = GetCommState(m_hComHandle, &dcb);
		dcb.BaudRate	= nBaudRate;
		dcb.fBinary		= TRUE;		//windows nie obs�uguje tryb�w niebinarnych
		dcb.fParity		= FALSE;
		dcb.ByteSize	= 8;			//ilo�� bit�w transmitowanych w ramce
		dcb.Parity		= NOPARITY;		//parzysto��
		dcb.StopBits	= ONESTOPBIT;	//jeden bit stopu
		Result = SetCommState(m_hComHandle, &dcb);

		//definicje timeout�w portu szeregowego. Jednostka: [ms]
		CommTimeouts.ReadIntervalTimeout =	18;	//maksymalny czas mi�dzy przyj�ciem dw�ch znak�w
		CommTimeouts.ReadTotalTimeoutConstant = 50;
		CommTimeouts.ReadTotalTimeoutMultiplier = 5;
		CommTimeouts.WriteTotalTimeoutConstant = 50;
		CommTimeouts.WriteTotalTimeoutMultiplier = 5;
		SetCommTimeouts(m_hComHandle, &CommTimeouts);

		//ustaw mask� zdarze� asynchronicznych
		//SetCommMask(m_hComHandle, EV_RXCHAR | EV_CTS);
		SetCommMask(m_hComHandle, EV_RXCHAR);

		//utw�rz zdarzenie sygnalizuj�ce przyj�cie znaku
		//m_Overlap.hEvent = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
		m_Overlap.hEvent = CreateEvent(NULL, false, false, NULL); // auto-reset event, non-signalled state

		//inicjuj struktur� overlapped do sygnalizacji zdarze� asynchronicznych
		m_Overlap.Pointer = 0;
		m_Overlap.Internal = 0;
		m_Overlap.InternalHigh = 0;
		m_Overlap.Offset = 0;
		m_Overlap.OffsetHigh = 0;
		
		assert(m_Overlap.hEvent);

		m_bConnected = TRUE;
	}
	else
	{
		if (m_hComHandle)
			Result = CloseHandle(m_hComHandle);
		m_bConnected = FALSE;
		chErr = ERR_CANT_CONNECT;
	}

	return chErr;
}



unsigned char  CPortSzeregowy::Disconnect()
{
	if (m_bConnected)
	{
		if (m_hComHandle)
		{
			//zamknij dost�p do portu
			BOOL Result = CloseHandle(m_hComHandle);			
			m_bConnected = FALSE;
			return ERR_OK;
		}
	}
	else
		return ERR_NOT_CONNECTED;

	return ERR_OK;
}



//wyslij ramk� danych do portu szeregowego
unsigned char  CPortSzeregowy::SendFrame(unsigned char *WskFrame, unsigned char chSize)
{
	DWORD dwSizeToSend, dwSizeSend;	
	DWORD dwRes;

	if (!m_bConnected)
		return ERR_NOT_CONNECTED;
	
	dwSizeToSend = chSize;
	if (WriteFile(m_hComHandle, WskFrame, dwSizeToSend, &dwSizeSend, &m_Overlap) == 0)
	{
		//je�eli wystapi� b��d
		dwRes = GetLastError();
		if (dwRes != ERROR_IO_PENDING)	//The GetLastError code ERROR_IO_PENDING is not a failure; it designates the write operation is pending completion asynchronously. 
			return ERR_SEND_DATA;
	}
	return ERR_OK;
}


//wyslij ramk� danych do portu szeregowego
unsigned char  CPortSzeregowy::SendData(unsigned char *chFrame, unsigned char chLen)
{
	DWORD dwSizeToSend, dwSizeSend;	
	DWORD dwRes;

	if (!m_bConnected)
		return ERR_NOT_CONNECTED;
	
	dwSizeToSend = chLen;
	
	if (WriteFile(m_hComHandle, chFrame, dwSizeToSend, &dwSizeSend, &m_Overlap) == 0)
	{
		//je�eli wystapi� b��d
		dwRes = GetLastError();
		if (dwRes != ERROR_IO_PENDING)
			return ERR_SEND_DATA;
		else
			dwRes = WaitForSingleObject(m_Overlap.hEvent, 20);	//czekaj a� wy�le
	}
	return ERR_OK;
}


//odczytaj dane z portu szeregowego
unsigned char  CPortSzeregowy::ReceiveData(unsigned char *Data, unsigned int *Len, unsigned int nMaxLen)
{
	int iRes;
	COMSTAT comStat;
	DWORD dwErrors;
	int ReqSize;
	DWORD dwSize, dwRes;
	DWORD dwEvtMask = 0;

	*Len = 0;

	dwRes = WaitForSingleObject(m_Overlap.hEvent, 1);
	

	//odczytaj ilo�� danych w buforze portu
	ClearCommError(m_hComHandle, &dwErrors, &comStat);
	if (comStat.cbInQue < nMaxLen)
		ReqSize = comStat.cbInQue;
	else
		ReqSize = nMaxLen;

	if (ReqSize > 0)
	{
		iRes = ReadFile(m_hComHandle, Data, ReqSize, &dwSize, &m_Overlap);
		*Len = dwSize;
		if (iRes == 0)
			return ERR_RECEIVE_FILE;
	}
	return ERR_OK;
}


//czekaj na opr�nienie si� bufora
int CPortSzeregowy::InBufSize(void)
{
	COMSTAT comStat;
	DWORD dwErrors;

	//odczytaj ilo�� danych w buforze portu
	ClearCommError(m_hComHandle, &dwErrors, &comStat);

	return comStat.cbInQue;
}

void CPortSzeregowy::PurgeIncomingData(void)
{
	PurgeComm(m_hComHandle, PURGE_RXCLEAR);
}
