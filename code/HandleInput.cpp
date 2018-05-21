#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "HandleInput.h"


/*
	TODO: Fehler in CheckIfStorageSuitable -> Fehlermeldung wird noch nicht richtig angezeigt
	Note: Ein int Wert hat die gr��e von 2 Byte (16 Bit)
		  "\n" wird durch "\0" ersetzt, da es zu Fehlern bei dem n�chsten Aufruf von fgets kommen kann, 
		  wenn die Arraygrenze �berschritten wird und \n im Buffer festh�ngt
*/

/* ---------------------------------------------------------------------------------------------------
	Die Funktion behandelt die Benutzereingabe zur Festlegung des Speichers, der Simuliert werden soll.
	Es wird gepr�ft ob die Benutzereingabe richtig.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter: -
	R�ckgabewert: i = Der vom Benutzer eingegeben Wert
--------------------------------------------------------------------------------------------------- */
int HandleUserInputStorage()
{
	char cBuffer[MAX_STORAGE_STRING_LENGTH] = "\0";
	int i = 0;

	fflush(stdin); //L�scht das \n-Zeichen aus dem Standardeingabepuffer

	while (true) {
		printf("Wie gross soll der zu verwendene Speicher sein? Es sind nur Werte mit der Basis 2^n zulaessig!\n");
		fgets(cBuffer, MAX_STORAGE_STRING_LENGTH, stdin); //Der Men�punkt wird vom Benutzer eingegeben
		i = CheckInput(cBuffer, MIN_STORAGE, MAX_STORAGE, MAX_STORAGE_STRING_LENGTH-2); //-2 wegen \0 und \n
		if (i != -1)
			if (CheckIfStorageSuitable(i) != -1) //Hier ist der Fehler!!
				break;
		if(i == -1) //Bei einer der Pr�funge ist ein Fehler aufgetreten
		{
			printf("Eingegebener Wert ist ungueltig!\n");
			printf("Der Wert muss folgende Kriterien aufweisen:\n\n - Groesser als %d\n - Kleiner als %d\n - Basis 2^n\n\n", MIN_STORAGE, MAX_STORAGE);
			printf("Bitte Wert erneut eingeben\n");
			system("Pause");
			system("cls");
			i = 0;
		}
	}
	if (i < DEFAULT_MIN_STORAGE) //Es wird gepr�ft ob die Eingabe kleiner als der DEFAULT_MIN_STORAGE (16) ist
	{
		printf("Da wird aber sparsam mit dem Speicher umgegangen.\n");
		printf("Wir erhoehen den Speicher mal auf: %d\n", DEFAULT_MIN_STORAGE);
		i = DEFAULT_MIN_STORAGE;
	}
	return i;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion behandelt die Benutzereingabe zur Auswahl des Men�punktes. Der Wert wird auf 
	Richtigkeit (>=1 & <=6) gepr�ft und zur�ckgegeben.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter: - 
	R�ckgabewert: i = Der vom Benutzer eingegeben Wert
--------------------------------------------------------------------------------------------------- */
int HandleUserInputMenu()
{
	char cBuffer[MENUPOINT_LENGTH] = "\0"; //Char-Array, in dem die Benutzereingabe gespeichert wird
	int i = 0;

	fflush(stdin); //L�scht das \n-Zeichen aus dem Standardeingabepuffer

	while(true) {
		printf("Waehlen Sie den Menuepunkt anhand der Nummer in den Klammern aus: ");
		fgets(cBuffer, MENUPOINT_LENGTH, stdin); //Der Men�punkt wird vom Benutzer eingegeben
		if ((i = CheckInput(cBuffer, MIN_MENUPOINT_VALUE, MAX_MENUPOINT_VALUE, MENUPOINT_LENGTH-2)) != -1) //Der Eingegebene Men�punkt wird gepr�ft //-2 wegen \0 und \n
			break;
		else //Pr�fung war nicht erfolgreich
		{
			printf("\nEs ist zu einem Fehler beim Pruefen des Wertes gekommen.\n");
			printf("Bitte Wert erneu eingeben!\n");
			system("Pause");
			system("cls");
			i = 0;
		}
	}
	return i;
}

/* -------------------------------------------------------------------------------------------------- -
	Die Funktion behandelt die Benutzereingabe, zur Definition des Speichers, der f�r einen neuen Prozess
	ben�tigt wird. Der vom Benutzer eingegeben Wert wird auf Richtigkeit gepr�ft.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter : nFreeStorage = Der Speicher, der momentan noch zur Verf�gung steht
	R�ckgabewert : -1 = Wenn ein Fehler aufgetreten ist
				   -2 = Wenn es wieder zur�ck ins Men� gehen soll
				   nStorage = Der Speicher, f�r den Prozess
-------------------------------------------------------------------------------------------------- - */
int HandleUserInputProzessStorage(int nFreeStorage)
{
	char cBuffer[MAX_STORAGE_STRING_LENGTH] = "\0";
	int nStorage = -1;

	fflush(stdin);
	system("cls");
	printf("Mit \"r\" kommen Sie wieder zum Menue.\n");
	while (true) {
		printf("Wie viel Speicher soll fuer den Prozess reserviert werden?\n");
		printf("Prozessgroesse: ");
		fgets(cBuffer, MAX_STORAGE_STRING_LENGTH, stdin); //Der Benutzer gibt die gr��e des Prozessspeichers ein
		if (CheckIfBackToMenu(cBuffer) == 1)
			return -2;
		nStorage = CheckInput(cBuffer, MIN_STORAGE, nFreeStorage, MAX_STORAGE_STRING_LENGTH-2); //-2 wegen \0 und \n
		if (nStorage != -1 && nStorage <= nFreeStorage)
			break;
		else
		{
			if (nStorage != -1 && nStorage > nFreeStorage)
			{
				printf("Es ist nicht mehr ausreichend Speicher fuer diesen Prozess vorhanden!");
				printf("Freier Speicher: %d\n", nFreeStorage);
				printf("Angeforderter Speicher: %d\n", nStorage);
				return -1; //muss evtl. angepasst werden
			}
			printf("Eingegebener Wert ist ungueltig!\n");
			printf("Der Wert muss folgende Kriterien aufweisen:\n\n - Groesser als %d\n - Kleiner als %d\n - Basis 2^n\n\n", MIN_STORAGE, nFreeStorage);
			printf("Bitte Wert erneut eingeben\n");
			system("Pause");
			system("cls");
			nStorage = 0;
		}
	}
	return nStorage;
}

/* -------------------------------------------------------------------------------------------------- -
	Die Funktion behandelt die Benutzereingabe, zur Definition des Prozessnamen.
	Der vom Benutzer eingegeben Wert wird auf Richtigkeit gepr�ft.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter : *c = Pointer, in dem der eingegebene Prozessname gespeichert werden soll
-------------------------------------------------------------------------------------------------- - */
void HandleUserInputProzessName(char *c)
{
	char cBuffer[PROZESS_NAME_LENGTH] = "\0";
	
	printf("Soll dem Prozess ein Name gegeben werden? Wenn nicht, geben Sie \"-\" ein.\n");
	while (true) {
		printf("Prozessname: ");
		fgets(cBuffer, PROZESS_NAME_LENGTH, stdin);
		if (cBuffer[0] == '-')
		{
			strcpy(c, DEFAULT_PROZESS_NAME);
			break;
		}
		else
		{
			cBuffer[strlen(cBuffer)-1] = '\0'; //"\n" wird durch "\0" ersetzt, da \n zu Fehler in der Darstellung gef�hrt hat
			strcpy(c, cBuffer);
			break;
		}
	}
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion behandelt die Benutzereingabe zur Auswahl des Men�punktes. Der Wert wird auf
	Richtigkeit gepr�ft und zur�ckgegeben.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter: -
	R�ckgabewert: -1 = Wenn ein Fehler aufgetreten ist, ansonsten dem vom Benutzer eingegebenen Wert
				  -2 = Wenn es wieder zur�ck ins Men� gehen soll
--------------------------------------------------------------------------------------------------- */
int HandleUserInputEndProzess()
{
	char cBuffer[MENUPOINT_LENGTH] = "\0"; //Char-Array, in dem die Benutzereingabe gespeichert wird
	
	system("cls");

	fflush(stdin); //L�scht das \n-Zeichen aus dem Standardeingabepuffer
	printf("Mit \"r\" kommen Sie wieder zum Menue.\n");

	while (true) {
		printf("Geben Sie die Prozess ID des zu beendenden Prozesses ein: ");
		fgets(cBuffer, MENUPOINT_LENGTH, stdin); //Der Men�punkt wird vom Benutzer eingegeben
		if (CheckIfBackToMenu(cBuffer) == 1)
			return -2;
		if ((CheckInt(cBuffer) == 1)) //Der Eingegebene Men�punkt wird gepr�ft
			return atoi(cBuffer);
		else //Pr�fung war nicht erfolgreich
		{
			printf("\nEingabe war ungueltig. Folgende Fehler koennten vorliegen: - Keinen ganzzahliger Wert\n - Eingabe eines Buchstabens\n");
			printf("Bitte Wert erneut eingeben!\n");
			system("Pause");
			system("cls");
			cBuffer[0] = '\0';
		}
	}
	return -1;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion pr�ft die Benutzereingabe auf Richtigkeit und verarbeitet diese
	Parameter: *c = String der gepr�ft werden soll
				nMin = Mindestwert, den *c haben darf 
				nMax = H�chsterwert, den *c haben darf
				nLength = L�nge, die *c h�chstens haben darf
	R�ckgabewert: -1  = Wenn ein Fehler aufgetreten ist
				   i  = Der gepr�ft Wert als integer
--------------------------------------------------------------------------------------------------- */
int CheckInput(char *c, int nMin, int nMax, int nLength)
{
	int i = -1;

	if (c == NULL)
		return -1;
	c[nLength] = '\0'; //"\n" wird durch "\0" ersetzt

	if (strlen(c) <= nLength) //Die L�nge des Wertes wird gepr�ft (-1 wegen \n bzw. \0)
	{
		i = atoi(c); //Der String wird zum Integer Wert umgewandelt
		if (i >= nMin && i <= nMax) //Es wird gepr�ft ob der Wert die angegebenen Grenzen �ber oder unterschreitet
			return i; //Wert hat alle Pr�fungen bestanden
	}
	return -1; //Es ist ein Fehler aufgetreten
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion pr�ft ob die Eingabe eine Basis von 2^n.
	Parameter: f = Wert der gepr�ft werden soll
	R�ckgabewert: -1  = Wenn ein Fehler aufgetreten ist
				   0  = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int CheckIfStorageSuitable(float f)
{
	float fRest = 0;

	while (f >= 2)
	{
		fRest = f / 2;
		f = fRest;
	}

	if (fRest != 1)
		return -1;

	return 0;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion pr�ft ob der Benutzer zur�ck zum Men� m�chte
	Parameter: *c = String der gepr�ft werden soll
	R�ckgabewert: 0 = Nicht zur�ck zum Men�
				  1 = Zur�ck zum Men�
--------------------------------------------------------------------------------------------------- */
int CheckIfBackToMenu(char *c)
{
	if (c[0] == 'r' || c[0] == 'R')
		return 1;
	return 0;
}
