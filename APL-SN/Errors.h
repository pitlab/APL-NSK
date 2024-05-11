//kody b��d�w
const int ERR_OK				= 0;	//bez b��du
const int ERR_NOT_CONNECTED		= 1;	//nie po��czono z portem szeregowym
const int ERR_CANT_CONNECT		= 2;	//nie mozna po��czy� z portem 
const int ERR_INVALID_DATA_SIZE	= 3;	//niew�a�ciwy rozmiar danych
const int ERR_INVALID_COMMAND	= 4;	//niew�asciwe polecenie
const int ERR_SEND_DATA			= 5;	//b��d wysy�ania danych
const int ERR_RECEIVE_FILE		= 6;	//problem z odbiorem danych
const int ERR_ZLY_NAGLOWEK		= 7;	//zdekodowano z�y nag��wek ramki
const int ERR_ZLY_ADRESAT		= 8;	//ramka jest przeznaczona dla kogo� innego
const int ERR_ZLE_POLECENIE		= 9;	//zdekodowano z�e polecenie
const int ERR_CRC16				= 10;	//zdekodowano z�� sum� kontroln�
const int ERR_ZLY_STAN_PROT		= 11;	//wykryto z�y stan protoko�u

const int ERR_NO_FILE			= 12;	//nie znaleziono takiego pliku
const int ERR_FILE_READ			= 13;	//problem z odczytem pliku
const int ERR_SEEK_FILE			= 14;	//problem z ustawieniem pozycji w pliku
const int ERR_TIMEOUT			= 15;	//timeout oczekiwania na przyjscie danych
const int ERR_ZLE_DANE			= 16;	//niew�asciwe dane
const int ERR_NO_DATA_RECEIVED	= 17;	//nie odebrano �adnych danych
const int ERR_OPEN_PORT			= 18;	//nie mozna otworzy� portu
const int ERR_CONNECTED			= 19;	//jest ju� po��czony
const int ERR_INVALID_ADDRESS	= 20;	//niew�a�ciwy adres danych logera
const int ERR_FILE_FORMAT		= 21;	//niew�a�ciwy format pliku
const int ERR_PLIK_ZA_DLUGI		= 22;	//plik zbyt d�ugi
const int ERR_NR_ZMIENNEJ_EE	= 23;	//odczytano niew�a�ciwy numer zmiennej EEPROM
const int ERR_WERSJA_CPU		= 24;	//nieprawid�owa wersja CPU
const int ERR_KASUJ_FLASH_CPU	= 25;	//b��d kasowania pami�ci flash kontrolera
const int ERR_PROGR_FLASH_CPU	= 26;	//b��d programowania pami�ci flash kontrolera