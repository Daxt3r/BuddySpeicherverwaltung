#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

/* ---------------------------------------------------------------------------------------------------
	Die Funkin ermittelt die L�nge eines Integerwertes
	Parameter: nValue = Der Wert, dessen L�nge ermittelt werden soll
	R�ckgabewert: nZaehler = L�nge des Wertes
--------------------------------------------------------------------------------------------------- */
int strlen_int(int nValue)
{
	int n = 9;
	int nZaehler = 0; //Z�hlt die Stellen der Zahl

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
	Funktion: Pr�ft ob in einer Zeichenkette(*cBuf) nur nummerische Zeichen enthalten sind
	Parameter: - char *cString = Zeichenkette welche gepr�ft werden soll
	R�ckgabewert: 0 = Nicht nur nummerische Zeichen 
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