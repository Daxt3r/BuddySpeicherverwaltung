#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "HandleInput.h"
#include "BuddySpV_Prozess.h"

/*
	TODO: 
*/

/* ---------------------------------------------------------------------------------------------------
	Die Funktion behandelt die Benutzereingabe zur Festlegung des Speichers, der Simuliert werden soll.
	Es wird gepr�ft ob die Benutzereingabe richtig.
	Die Funktion l�uft solange, bis eine richtige Eingabe gemacht wurde, oder das Programm beendet wird
	Parameter: -
	R�ckgabewert: nStorage = Der vom Benutzer eingegeben Wert
--------------------------------------------------------------------------------------------------- */
int HandleUserInputStorage()
{
	char cBuffer[MAX_STORAGE_STRING_LENGTH] = "\0";
	int nStorage = 0;
	int recv = 0;

	while (true) {
		printf("Wie gross soll der zu verwendene Speicher sein? Es sind nur Werte mit der Basis 2^n zulaessig!\n");
		fgets(cBuffer, MAX_STORAGE_STRING_LENGTH, stdin); //Der Men�punkt wird vom Benutzer eingegeben
		SetNewCharacter(cBuffer, '\n', '\0');
		nStorage = CheckInput(cBuffer, MIN_STORAGE, MAX_STORAGE, MAX_STORAGE_STRING_LENGTH);
		if (nStorage == 1) //1 wird nicht als Basis von 2^n erkannt, daher wird dieser Fall so abgefangen
			break;
		if (nStorage == -1) //Wert wird auf Richtigkeit gepr�ft
		{
			printf("\nEingegebener Wert ist ungueltig!\n");
			printf("Der Wert muss folgende Kriterien aufweisen:\n\n - Groesser als %d\n - Kleiner als %d\n - Basis 2^n\n\n", MIN_STORAGE, MAX_STORAGE);
			printf("Bitte Wert erneut eingeben\n");
			system("Pause");
			system("cls");
			nStorage = 0;
		}
		else if ((recv = CheckIfStorageSuitable(nStorage)) == -1) //Wert wird auf Basis 2^n gepr�ft
		{
			printf("\nEingegebener Wert ist ungueltig!\n");
			printf("Eingegebener Wert ist nicht von der Basis 2^n!\n");
			printf("Bitte Wert erneut eingeben\n");
			system("Pause");
			system("cls");
			nStorage = 0;
			recv = 0;
		}
		else
			break; //Wert hat alle Pr�fungen bestanden, Schleife kann verlassen werden
	}
	if (nStorage < DEFAULT_MIN_STORAGE) //Es wird gepr�ft ob die Eingabe kleiner als der DEFAULT_MIN_STORAGE (16) ist
	{
		printf("Da wird aber sparsam mit dem Speicher umgegangen.\n");
		printf("Der Speicher wird auf: %d erhoeht.\n", DEFAULT_MIN_STORAGE);
		nStorage = DEFAULT_MIN_STORAGE;
	}
	return nStorage;
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

	while(true) {
		printf("Waehlen Sie den Menuepunkt anhand der Nummer in den Klammern aus: ");
		fgets(cBuffer, MENUPOINT_LENGTH, stdin); //Der Men�punkt wird vom Benutzer eingegeben
		SetNewCharacter(cBuffer, '\n', '\0');
		if ((i = CheckInput(cBuffer, MIN_MENUPOINT_VALUE, MAX_MENUPOINT_VALUE, MENUPOINT_LENGTH)) != -1) //Der Eingegebene Men�punkt wird gepr�ft //-2 wegen \0 und \n
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
int HandleUserInputProzessStorage(struct tStorage *pStorage, int nFreeStorage)
{
	char cBuffer[MAX_STORAGE_STRING_LENGTH] = "\0";
	int nStorage = -1;
	int recv = 0;
	system("cls");

	printf("Mit \"r\" kommen Sie wieder zum Menue.\n");
	
	if (nFreeStorage > 0) //Es wird gepr�ft ob noch Speicherplatz vorhanden ist
	{
		while (true) {
			printf("Wie viel Speicher soll fuer den Prozess reserviert werden?\n");
			printf("Prozessgroesse: ");
			fgets(cBuffer, MAX_STORAGE_STRING_LENGTH, stdin); //Der Benutzer gibt die gr��e des Prozessspeichers ein
			SetNewCharacter(cBuffer, '\n', '\0');
			if (CheckIfBackToMenu(cBuffer) == 1)
				return -2;
			nStorage = CheckInput(cBuffer, MIN_STORAGE, MAX_STORAGE, MAX_STORAGE_STRING_LENGTH);
			if (nStorage == -1) //Es wird gepr�ft, ob der eingegebene Wert den Vorgaben entspricht
			{
				printf("\nEingegebener Wert ist ungueltig!\n");
				printf("Der Wert muss folgende Kriterien aufweisen:\n\n - Groesser als %d\n - Kleiner als %d\n - Basis 2^n\n\n", MIN_STORAGE, MAX_STORAGE);
				printf("Bitte Wert erneut eingeben\n\n");
				system("Pause");
				system("cls");
				nStorage = 0;
				continue;
			}
			if (nStorage > pStorage->nMaxStorageSize) //Es wird gepr�ft, ob der eingegebene Wert die MaxStorageSize �berschreitet
			{
				printf("\nDie eingegebene Prozessegroesse ueberschreitet die maximale Speichergroesse von %d!\n", pStorage->nMaxStorageSize);
				printf("Geben Sie einen geringeren Wert ein!\n\n");
				system("Pause");
				system("cls");
				nStorage = 0;
				continue;
			}
			if ((recv = CheckFreeStorage(pStorage, nStorage)) == 1) //Es wird gepr�ft ob es einen Speicherblock mit gen�gend zusammenh�ngenden Speicher f�r den neuen Prozess gibt
				return -1;
			else if (recv == 0)
				break; //Schleife kann verlassen werden, Pr�fung des Wertes war erfolgreich -> eingabe korrekt, gen�gend Speicherplatz vorhanden
			else if (recv == -1)
			{
				printf("\nEs ist nicht mehr ausreichend zusammenhaengender Speicher fuer den Prozess vorhanden!\n");
				printf("Beenden Sie Prozesse um Speicherplatz zu schaffen.\n\n");
				return -2; //Benutzer kehrt wieder ins Men� zur�ck
			}
		}
	}
	else //Es ist kein Speicherplatz mehr vorhanden
	{
		printf("\nEs ist kein freier Speicherplatz mehr vorhanden!\n");
		printf("Beenden Sie Prozesse um Speicherplatz zu schaffen.\n\n");
		return -2; //Benutzer kehrt wieder ins Men� zur�ck
	}
	return nStorage; //Die gr��e des Prozesses wird zur�ck gegeben
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
	
	printf("\nSoll dem Prozess ein Name gegeben werden? Wenn nicht, geben Sie \"-\" ein.\n");
	while (true) {
		printf("Prozessname: ");
		fgets(cBuffer, PROZESS_NAME_LENGTH, stdin);
		if (cBuffer[0] == '-') //Es wird gepr�ft ob ein Default Prozessname vergeben werden soll
		{
			sprintf(c, "%s_%d", DEFAULT_PROZESS_NAME, lnr);
			lnr++;
			break;
		}
		else
		{
			SetNewCharacter(cBuffer, '\n', '\0');
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
	char cBuffer[MAX_PROZESSID_LENGTH] = "\0"; //Char-Array, in dem die Benutzereingabe gespeichert wird
	
	system("cls");

	fflush(stdin); //L�scht das \n-Zeichen aus dem Standardeingabepuffer
	printf("Mit \"r\" kommen Sie wieder zum Menue.\n");

	while (true) {
		printf("Geben Sie die Prozess ID des zu beendenden Prozesses ein: ");
		fgets(cBuffer, MAX_PROZESSID_LENGTH, stdin); //Die Prozess ID wird vom Benutzer eingegeben
		SetNewCharacter(cBuffer, '\n', '\0');
		if (CheckIfBackToMenu(cBuffer) == 1)
			return -2;
		if ((CheckInt(cBuffer) == 1)) //Der eingegebene Wert wird bearbeitet
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
	Die Funktion pr�ft ob noch gen�gend Platz f�r den Prozess frei ist
	Parameter: f = Wert der gepr�ft werden soll
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn genug Speicherplatz vorhanden ist
				 -1 = Wenn nicht gen�gend Speicherplatz vorhanden ist
--------------------------------------------------------------------------------------------------- */
int CheckFreeStorage(struct tStorage *pStorage, int nStorage)
{
	struct tBuddy *pTmp = NULL;
	pTmp = searchForFreeStorage(pStorage); //Gr��ter freier Speicher wird angefordert

	if (pTmp != NULL)
	{
		if (pTmp->nStorageSize >= nStorage) //Es wird gepr�ft ob genug Speicherplatz f�r den neuen Prozess vorhanden ist
			return 0; //Genug Platz vorhanden
		else
			return -1; //Nicht genug Platz vorhanden
	}
	printf("Es ist ein Fehler in der Funktion \"CheckFreeStorage\" aufgetreten. . .\n");
	return 1;
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
