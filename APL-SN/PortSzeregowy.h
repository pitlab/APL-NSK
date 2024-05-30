#pragma once

class CPortSzeregowy
{
public:
	CPortSzeregowy();
	virtual ~CPortSzeregowy();

	unsigned char  ReceiveData(unsigned char *Data, unsigned int *Len, unsigned int nMaxLen);
	unsigned char  SendFrame(unsigned char* WskFrame, unsigned char chSize);
	unsigned char  SendData(unsigned char *chFrame, unsigned char chLen);
	unsigned char  Disconnect();
	unsigned char  Connect(int nComPort, int nBaudRate);
	//unsigned char  Err;
	BOOL GetConnectedState();
	int GetComPortNr();
	void SetNrCom(int ComNr);
	unsigned char  OpenPort(int ComNr, HANDLE *hComHandle);
	int InBufSize(void);
	void PurgeIncomingData(void);

private:
	BOOL m_bConnected;
	int m_iComNr;
	HANDLE m_hComHandle;
	int m_iIloscInstancji;
	OVERLAPPED m_Overlap;

};

CPortSzeregowy& getPortSzeregowy();



