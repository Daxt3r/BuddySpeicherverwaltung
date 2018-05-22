#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

/* ---------------------------------------------------------------------------------------------------
	Die Funkin ermittelt die Länge eines Integerwertes
	Parameter: nValue = Der Wert, dessen Länge ermittelt werden soll
	Rückgabewert: nZaehler = Länge des Wertes
--------------------------------------------------------------------------------------------------- */
int strlen_int(int nValue)
{
	int n = 9;
	int nZaehler = 0; //Zählt die Stellen der Zahl

	while (true) {
		if (nValue <= n)
		{
			nZaehler++;
			break;
		}
		else
		{
			nZaehler++;
			n = n * 10 + 9;
		}
	}
	return nZaehler;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------
	Die Funktion berechnet alle Werte von 1 bis zum maximalen Wert
	Parameter: - nMax = Wert bis zu dem die Potenz berechnet werden soll
	Rückgabewert: -
-----------------------------------------------------------------------------------------------------------------------------------------------*/
void pow(int nMax)
{
	int i = 1;
	while (i <= nMax)
	{
		printf("[%d] ", i);
		i = i * 2;
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------------------
	Funktion: Prüft ob in einer Zeichenkette(*cBuf) nur nummerische Zeichen enthalten sind
	Parameter: - *cString = Zeichenkette welche geprüft werden soll
	           - cSearch = Das zu suchende Zeichen, welches ersetzt werden soll
			   - cNew = Das neue Zeichen
	Rückgabewert: -
-----------------------------------------------------------------------------------------------------------------------------------------------*/
void SetNewCharacter(char *cString, char cSearch, char cNew)
{
	while (*cString != '\0')
	{
		if (*cString == cSearch)
			*cString = cNew;
		cString++;
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------------------
	Funktion: Prüft ob in einer Zeichenkette(*cBuf) nur nummerische Zeichen enthalten sind
	Parameter: - char *cString = Zeichenkette welche geprüft werden soll
	Rückgabewert: 0 = Nicht nur nummerische Zeichen 
	              1 = Nur nummerische Zeichen 
-----------------------------------------------------------------------------------------------------------------------------------------------*/
int CheckInt(char *cString)
{
	if (cString == NULL)
		return 0;
	if (cString[strlen(cString) - 1] == '\n')
		cString[strlen(cString) - 1] = '\0';

	while (*cString != '\0')
	{
		if ((*cString < '0') || (*cString > '9'))
			return 0;
		cString++;
	}
	return 1;
}