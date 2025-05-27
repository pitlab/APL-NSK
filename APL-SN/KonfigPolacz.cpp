// OknoPolacz.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPolacz.h"
#include "afxdialogex.h"
#include <setupapi.h>
#include <devguid.h>
#include <cfgmgr32.h>   //CM_Get_Parent
#include "polecenia_komunikacyjne.h"


// Okno dialogowe OknoPolacz
static SP_CLASSIMAGELIST_DATA ild;
IMPLEMENT_DYNAMIC(CKonfigPolacz, CDialogEx)

CKonfigPolacz::CKonfigPolacz(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PORT, pParent)
	, m_strAdresIP(_T("127.0.0.1"))
	, m_strPortETH(_T("4001"))
	, m_bTypPolaczenia(FALSE)
{
	
}

CKonfigPolacz::~CKonfigPolacz()
{
}

void CKonfigPolacz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT_COM, m_cPortCom);
	DDX_Control(pDX, IDC_COMBO_PREDKOSC, m_cPredkoscCom);
	DDX_Text(pDX, IDC_EDIT_ADRES, m_strAdresIP);
	DDX_Text(pDX, IDC_EDIT_PORT_ETH, m_strPortETH);
	DDX_Radio(pDX, IDC_RAD_UART, m_bTypPolaczenia);
}


BEGIN_MESSAGE_MAP(CKonfigPolacz, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigPolacz::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RAD_UART, &CKonfigPolacz::OnBnClickedRadUart)
	ON_BN_CLICKED(IDC_RAD_ETH, &CKonfigPolacz::OnBnClickedRadEth)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OknoPolacz

BOOL CKonfigPolacz::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ild.cbSize = sizeof ild;
	SetupDiGetClassImageList(&ild);

	HANDLE devs = SetupDiGetClassDevsW((LPGUID)&GUID_DEVCLASS_PORTS, 0, 0, DIGCF_PRESENT);

	SP_DEVINFO_DATA devInfo;
	devInfo.cbSize = sizeof devInfo;
	TCHAR s[80];
	//TCHAR str[96];
	COMBOBOXEXITEM cbei;
	cbei.mask = CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	cbei.pszText = s;

	for (DWORD i = 0; SetupDiEnumDeviceInfo(devs, i, &devInfo); i++)
	{
		HKEY hKey = SetupDiOpenDevRegKey(devs, &devInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
		if (hKey == INVALID_HANDLE_VALUE) continue;
		TCHAR t[16];    // The COM port name will be placed here
		*t = 0;
		DWORD len = sizeof(t);
		RegQueryValueExW(hKey, L"PortName", 0, 0, (LPBYTE)t, &len);
		RegCloseKey(hKey);
		if (*t != 'C') continue;  // bail out on errors and LPT%u
		//m_cPortCom.AddString(t);
		//HANDLE h = myOpen((UINT)cbei.lParam);
		//if (h) CloseHandle(h);
		cbei.lParam = StrToInt(t + 3) - 1;    // I use zero-based numbering
		//cbei.iOverlay = h ? 0 : 2;  // 2 is the "not available" overlay.
		DEVINST parent;  // Graying text would require ownerdrawn combobox and much more code
		const GUID* pguid = &GUID_DEVCLASS_PORTS;
		// Show class icon for parent(!) device class, so get a clue where your COM port is connected (USB or onboard, or somewhere else)
		if (!CM_Get_Parent(&parent, devInfo.DevInst, 0)) {
			ULONG l = sizeof s;
			if (!CM_Get_DevNode_Registry_Property(parent, CM_DRP_CLASSGUID, 0, s, &l, 0)) {
				GUID guid;
				if (!CLSIDFromString(s, &guid)) pguid = &guid; // change pointer on success
			}
		}
		SetupDiGetClassImageIndex(&ild, pguid, &cbei.iImage);
		cbei.iSelectedImage = cbei.iImage;
		// Show descriptive string, not only COM%u
		SetupDiGetDeviceRegistryProperty(devs, &devInfo, SPDRP_FRIENDLYNAME, 0, (PBYTE)s, sizeof s, 0);   // ab Windows 2k?


	// The COM number should bei included by help of device driver. If not, append it
		if (!StrStr(s, t))
		{ // Caution! StrStr(Haystack,Needle) vs. strstr(needle,haystack)
			int l = lstrlen(s);
			//wnsprintf(s + l, elemof(s) - l, T(" (%s)"), t);
			wnsprintf(s, 80, (L"%s (%s)"), s, t);

		}
		m_cPortCom.AddString(s);
	}
	SetupDiDestroyDeviceInfoList(devs);

	int32_t x, y, nPos;
	CString Str;
	wchar_t chNapis[200];
	uint32_t nValue;


	//Przeszukaj wykryte porty i ustaw aktywny ten który jest w rejestrze
	for (y = 0; y < m_cPortCom.GetCount(); y++)
	{
		m_cPortCom.GetLBText(y, Str);
		nPos = Str.Find(_T("(COM"));
		for (x = 0; x < 3; x++)
			chNapis[x] = Str.GetAt(nPos + 4 + x);
		chNapis[x] = 0;
		nValue = _wtoi((const wchar_t*)chNapis);
		if (nValue == m_chNumerPortuCom)
		{
			m_cPortCom.SetCurSel(y);
			break;
		}
	}

	//odczytaj rejestr i ustaw pozycje z odczytaną prędkością portu COM
	m_cPredkoscCom.AddString(L"9600");
	m_cPredkoscCom.AddString(L"19200");
	m_cPredkoscCom.AddString(L"38400");
	m_cPredkoscCom.AddString(L"57600");
	m_cPredkoscCom.AddString(L"115200");
	m_cPredkoscCom.AddString(L"172800");
	m_cPredkoscCom.AddString(L"230400");

	//Przeszukaj wykryte prędkości i ustaw aktywny tą która jest w rejestrze
	for (y = 0; y < m_cPredkoscCom.GetCount(); y++)
	{
		m_cPredkoscCom.GetLBText(y, Str);
		for (x = 0; x < Str.GetLength(); x++)
			chNapis[x] = Str.GetAt(x);
		chNapis[x] = 0;
		nValue = _wtoi((const wchar_t*)chNapis);
		if (nValue == m_nPredkoscPortuCom)
		{
			m_cPredkoscCom.SetCurSel(y);
			break;
		}
	}

	//Przepisz adres do stringa
	m_strAdresIP.Format(L"%d.%d.%d.%d", m_chAdresIP[0], m_chAdresIP[1], m_chAdresIP[2], m_chAdresIP[3]);
	m_strPortETH.Format(L"% d", m_chNumerPortuEth);

	//ustaw widoczność aktywnych kontrolek
	m_chTypPolaczenia = UART;
	UaktywnijPola(m_chTypPolaczenia);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przycisk OK. Zapisuje ustawienia do rejestru
// Parametry:
//  chTyp - rodzaj połączenia
// zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigPolacz::OnBnClickedOk()
{
	int x, nPos;
	DWORD dwWartosc;
	CString Str;
	wchar_t chNapis[90];
	HKEY hKey;
	LONG lErr;
	DWORD dwResult;

	UpdateData(TRUE);
	nPos = m_cPortCom.GetCurSel();
	m_cPortCom.GetLBText(nPos, Str);
	nPos = Str.Find(_T("(COM"));
	for (x = 0; x < 3; x++)
		chNapis[x] = Str.GetAt(nPos + 4 + x);
	chNapis[x] = 0;
	dwWartosc = _wtoi((const wchar_t*)chNapis);

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegSetValueExW(hKey, L"NrPortuCOM", 0, REG_DWORD, (LPBYTE)&dwWartosc, 4);
		RegCloseKey(hKey);
	}
	if (lErr != ERROR_SUCCESS)
		MessageBoxW(L"Nie udało się utworzyć pozycji", L"Ojojoj!", MB_ICONWARNING);
	UpdateData(FALSE);


	nPos = m_cPredkoscCom.GetCurSel();
	m_cPredkoscCom.GetLBText(nPos, Str);
	for (x = 0; x < Str.GetLength(); x++)
		chNapis[x] = Str.GetAt(x);
	chNapis[x] = 0;
	dwWartosc = _wtoi((const wchar_t*)chNapis);

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegSetValueExW(hKey, L"PredkoscCOM", 0, REG_DWORD, (LPBYTE)&dwWartosc, 4);
		RegCloseKey(hKey);
	}
	if (lErr != ERROR_SUCCESS)
		MessageBoxW(L"Nie udało się utworzyć pozycji", L"Ojojoj!", MB_ICONWARNING);


	for (x = 0; x < m_strAdresIP.GetLength(); x++)
		chNapis[x] = m_strAdresIP.GetAt(x);
	chNapis[x] = 0;

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegSetValueExW(hKey, L"AdresIP", 0, REG_SZ, (LPBYTE)&chNapis, 2 * m_strAdresIP.GetLength() + 1);
		RegCloseKey(hKey);
	}
	if (lErr != ERROR_SUCCESS)
		MessageBoxW(L"Nie udało się utworzyć pozycji", L"Ojojoj!", MB_ICONWARNING);

	for (x = 0; x < m_strPortETH.GetLength(); x++)
		chNapis[x] = m_strPortETH.GetAt(x);
	chNapis[x] = 0;
	dwWartosc = _wtoi((const wchar_t*)chNapis);

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegSetValueExW(hKey, L"PortETH", 0, REG_DWORD, (LPBYTE)&dwWartosc, 4);
		RegCloseKey(hKey);
	}
	if (lErr != ERROR_SUCCESS)
		MessageBoxW(L"Nie udało się utworzyć pozycji", L"Ojojoj!", MB_ICONWARNING);

	dwWartosc = m_chTypPolaczenia;

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegSetValueExW(hKey, L"TypPortu", 0, REG_DWORD, (LPBYTE)&dwWartosc, 4);
		RegCloseKey(hKey);
	}
	if (lErr != ERROR_SUCCESS)
		MessageBoxW(L"Nie udało się utworzyć pozycji", L"Ojojoj!", MB_ICONWARNING);

	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia widoczność pól konfiguracji w zależnosci od rodzaju wybranego portu
// Parametry:
//  chTyp - rodzaj połączenia
// zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigPolacz::UaktywnijPola(uint8_t chTyp)
{
	if (chTyp == UART)
	{
		GetDlgItem(IDC_COMBO_PORT_COM)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PREDKOSC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ADRES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT_ETH)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PORT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PREDKOSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADRES)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT_ETH)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Nacisnięto wybór portu UART
// Parametry:
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigPolacz::OnBnClickedRadUart()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_chTypPolaczenia = UART;
	UaktywnijPola(m_chTypPolaczenia);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Nacisnięto wybór portu Ethernet serwer
// Parametry:
// zwraca: nic
//////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigPolacz::OnBnClickedRadEth()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_chTypPolaczenia = ETHS;
	UaktywnijPola(m_chTypPolaczenia);
}
