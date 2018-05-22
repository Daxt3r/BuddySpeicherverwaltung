#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>

#include "HandleInput.h"
#include "BuddySpV_Prozess.h"
#include "util.h"

/*
	TODO: Programmcode dokumentieren!!
		  Speicher beim Beenden des Programmes wieder freigeben
*/


/*---------------------------------------------------------------------------------------------------
	Die Funktion ist die Obersteebene des Prozesses um einen neuen Prozesses zu starten
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int startProzess(struct tStorage *pStorage)
{
	int nProzessSize = 0;
	int nStorage = 0;
	char cProzessName[PROZESS_NAME_LENGTH];
	int n = 0;

	system("cls");

	if ((nProzessSize = HandleUserInputProzessStorage(pStorage, pStorage->nFreeStorage)) == -1) //Benutzer gibt die gr��e des Prozesses ein
	{
		printf("Es ist ein Fehler in der Funktion \"HandleUserInputProzessStorage\" aufgetreten. . .\n");
		return 1;
	}
	if (nProzessSize == -2)
	{
		printf("Vorgang wird abgebrochen, Sie kehren in das Menue zurueck!\n");
		return 0;
	}
	HandleUserInputProzessName(cProzessName); //Benutzer gibt den Namen des Prozess ein, oder DEFAULT-Name wird gesetzt
	nStorage = storageSize(nProzessSize); //Es wird der passende Speicher f�r den Prozess ermittelt

	if((n = searchInBuddyList(pStorage, nStorage, nProzessSize, cProzessName)) == 1)
	{
		printf("Es ist ein Fehler in der Funktion \"searchInBuddyList\" aufgetreten. . .\n");
		return 1;
	}
	if (n == 0)
	{
		printf("Prozess wurde erfolgreich gestartet. . .\n\n");
		return 0;
	}
	if ((n = splitStorage(pStorage, nStorage, nProzessSize, cProzessName)) == 1)
	{
		return 1;
		printf("Es ist zu einem Fehler in der Funktion \"splitStorage\" gekommen. . .\n");
	}
	else if (n == -1)
	{
		printf("Es konnte kein neuer Prozess gestartet werden!\n\n");
		return 0;
	}

	printf("\nProzess wurde erfolgreich gestartet. . .\n\n");
	return 0;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion bestimmt den "optimalen" Speicher, welcher f�r den Prozess am Besten passt.
	Parameter: nValue = Wert, welcher gepr�ft werden soll
	R�ckgabewert: nStorage = Speicherbereich, der f�r den Prozess am besten passt
--------------------------------------------------------------------------------------------------- */
int storageSize(int nValue)
{
	int nStorage = 1;
	int nBasis = 2;

	while (true)
	{
		if (nStorage < nValue)
			nStorage = nStorage * nBasis;
		else
			return nStorage;
	}
}

/* ---------------------------------------------------------------------------------------------------
	Es wird in der BuddyListe nach einen freien und passenden Speicher f�r den Prozess gesucht.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   nStorage = Speicher, der f�r den Prozess passend ist
			   nProzessSize = Gr��e des Speichers, des Prozesses
			   cProzessName = Name des Prozesses
	R�ckgabewert: 1  = Wenn ein Fehler aufgetreten ist
				  0  = Wenn ein passender Speicher in der Liste gefunden wurde
				  -1 = Wenn kein passender Speicher in der Liste gefunden wurde
--------------------------------------------------------------------------------------------------- */
int searchInBuddyList(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzessName)
{
	int i = 0;

	if (pStorage->pBuddyList != NULL)
	{
		/* Die Schleife durchl�uft die Buddyliste und pr�ft ob bereitsvorhandene Buddys f�r den neuen Prozess geeignet sind.
		   So wird passender Speicher schneller gefunden, anstatt durch die verkettete Liste zu iterieren. */
		for (; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->nStorageSize == nStorage && pStorage->pBuddyList[i]->bFree == true)
				{
					addProzess(pStorage, pStorage->pBuddyList[i], nStorage, nProzessSize, cProzessName);
					if (addToBuddyProzessList(pStorage, pStorage->pBuddyList[i]) == 1)
						return 1;
					return 0;
				}
				if (pStorage->pBuddyList[i]->pBuddy->nStorageSize == nStorage && pStorage->pBuddyList[i]->pBuddy->bFree == true) //Buddy wird gepr�ft
				{
					addProzess(pStorage, pStorage->pBuddyList[i]->pBuddy, nStorage, nProzessSize, cProzessName);
					if (addToBuddyProzessList(pStorage, pStorage->pBuddyList[i]->pBuddy) == 1)
						return 1;
					return 0;
				}
			}
		}
	}
	return -1;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion pr�ft ob der Speicher f�r den Prozess ausreicht. Wenn nicht, wir der Speicher geteilt
	und es wird erneut gepr�ft ob dieser Speicher "optimal" f�r den ben�tigten Prozessspeicher ist.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   nStorage = Speicher, der f�r den Prozess passend ist
			   nProzessSize = Gr��e des Speichers, des Prozesses
			   cProzessName = Name des Prozesses
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
				  -1 = Wenn kein Speicher mehr frei ist
--------------------------------------------------------------------------------------------------- */
int splitStorage(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzessName)
{
	struct tBuddy *pTmpBuddyOld = NULL;
	struct tBuddy *pTmpBuddy = NULL;
	int recv = 0;

	pTmpBuddy = searchForFreeStorage(pStorage); //Gr��ter freier Speicher wird ermittelt und zur�ck gegeben

	while (pTmpBuddy != NULL)
	{
		if (pTmpBuddy->nStorageSize == nStorage && pTmpBuddy->bFree == true) //Erster Buddy wird gepr�ft
		{
			addProzess(pStorage, pTmpBuddy, nStorage, nProzessSize, cProzessName);
			if (addToBuddyProzessList(pStorage, pTmpBuddy) == 1)
			{
				printf("Es ist zu einem Fehler in der Funktion \"addToBuddyProzessList\" aufgetreten. . .\n");
				return 1;
			}
			return 0;
		}
		else if (pTmpBuddy->pBuddy->nStorageSize == nStorage && pTmpBuddy->pBuddy->bFree == true) //Buddy wird gepr�ft
		{
			addProzess(pStorage, pTmpBuddy->pBuddy , nStorage, nProzessSize, cProzessName);
			if (addToBuddyProzessList(pStorage, pTmpBuddy) == 1)
			{
				printf("Es ist zu einem Fehler in der Funktion \"addToBuddyProzessList\" aufgetreten. . .\n");
				return 1;
			}
			return 0;
		}
		else if (pTmpBuddy->pNextBuddy != NULL) //Es wird auf den n�chsten Buddy gezeigt, da die beiden oberliegenden entweder zu gro� oder nicht frei sind
			pTmpBuddy = pTmpBuddy->pNextBuddy;
		else //Neuer Buddy wird erstellt, da aktueller Speicher nicht optimal f�r den Prozess ist
		{
			if ((recv = createBuddy(pTmpBuddy)) == 1)
			{
				printf("Es ist ein Fehler in der Funktion \"createBuddy\" aufgetreten. . .\n");
				return 1;
			}
			if (addToBuddyList(pStorage, pTmpBuddy->pNextBuddy) == 1)
			{
				printf("Es ist zu einem Fehler in der Funktion \"addToBuddyProzessList\" aufgetreten. . .\n");
				return 1;
			}
		}
		
	}		
	printf("Es wurde kein Speicher in der BuddyListe gefunden!\n");
	return 1;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion sucht den gr��ten freien Speicher und gibt diesen zur�ck. Sollte noch kein Buddy in der Liste
	sein, wird der erste Buddy zur�ck gegeben.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewerte:  pBiggestFreeBuddy = Gr��ter freier Speicher
				    NULL = Wenn kein freier Speicher gefunden wurde
--------------------------------------------------------------------------------------------------- */
struct tBuddy* searchForFreeStorage(struct tStorage *pStorage)
{
	int i = 0;
	struct tBuddy *pBiggestFreeBuddy = NULL;

	if (pStorage->pBuddyList != NULL)
	{
		/* Die Schleife durchl�uft die Buddyliste und sucht nach dem gr��ten freien Speicher */
		for (; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->nStorageSize > 0)
				{
					if (pBiggestFreeBuddy == NULL)
						pBiggestFreeBuddy = pStorage->pBuddyList[i];
					else if (pBiggestFreeBuddy->nStorageSize < pStorage->pBuddyList[i]->nStorageSize)
						pBiggestFreeBuddy = pStorage->pBuddyList[i];
				}
				else if (pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->pBuddy->nStorageSize > 0)
				{
					if (pBiggestFreeBuddy == NULL)
						pBiggestFreeBuddy = pStorage->pBuddyList[i]->pBuddy;
					else if (pBiggestFreeBuddy->nStorageSize < pStorage->pBuddyList[i]->pBuddy->nStorageSize)
						pBiggestFreeBuddy = pStorage->pBuddyList[i]->pBuddy;

				}
			}
		}
	}

	return pBiggestFreeBuddy;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion erstellt neue Buddys
	Parameter: *pTmpBuddy = Der aktuelle Buddy, dessen Speicher f�r den Prozess zu gro� ist und nun aufgeteilt werden soll
	R�ckgabewerte: 1 = Wenn ein Fehler aufgetreten ist
				   0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int createBuddy(struct tBuddy *pTmpBuddy)
{
	struct tBuddy *pTmpBuddyNew = NULL;

	//Erster Buddy wird erstellt
	if ((pTmpBuddyNew = Init_tBuddy()) == NULL)
	{
		printf("Es ist zu einem Fehler in der Funktion \"Init_tBuddy\" gekommen. . .\n");
		return 1;
	}
	pTmpBuddyNew->nStorageSize = pTmpBuddy->nStorageSize / 2; //Speichergr��e des Buddys wird gesetzt (H�lfte des vorherigen)

	pTmpBuddy->pNextBuddy = pTmpBuddyNew; //Verweis auf den neuen Buddy wird gesetzt (darunterliegenden)
	pTmpBuddyNew->pPreviousBuddy = pTmpBuddy; //Verweis auf den vorherigen Buddy wird gesetzt (dar�berliegend)

	//Zweiter Buddy wird erstellt
	if ((pTmpBuddyNew = Init_tBuddy()) == NULL)
	{
		printf("Es ist zu einem Fehler in der Funktion \"Init_tBuddy\" gekommen. . .\n");
		return 1;
	}
	pTmpBuddyNew->nStorageSize = pTmpBuddy->nStorageSize / 2; //Speichergr��e des Buddys wird gesetzt (H�lfte des vorherigen)
	
	//Verweis der neuen Buddys untereinander werden gesetzt 
	pTmpBuddy->pNextBuddy->pBuddy = pTmpBuddyNew; 
	pTmpBuddyNew->pBuddy = pTmpBuddy->pNextBuddy->pBuddy;

	pTmpBuddy->bFree = false; //(alter) Buddy wird als belegt gekennzeichnet, da sein Speicher aufgeteilt wurde
	pTmpBuddy->nStorageSize = 0; //Speicher wird auf 0 gesetzt, da dieser aufgeteilt wurde und theoretisch nicht mehr vorhanden ist

	return 0;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion f�gt Werte in die Struktur ein, auf die pBuddy referenziert
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   *pBuddy = Zeiger auf die Struktur, in die, die Werte eingef�gt werden sollen
			   nStorage = Speicher, der f�r den Prozess passend ist
			   nProzessSize = Gr��e des Speichers, des Prozesses
			   cProzessName = Name des Prozesses
--------------------------------------------------------------------------------------------------- */
void addProzess(struct tStorage *pStorage, struct tBuddy *pBuddy, int nStorage, int nProzessSize, char *cProzessName)
{
	pBuddy->nPID = nCounter_PID++;
	pBuddy->bFree = false;
	pBuddy->nStorageSize = nStorage; //Kann eventuell entfernt werden
	pBuddy->nProzessSize = nProzessSize;
	strcpy_s(pBuddy->cProzessName, cProzessName);

	pStorage->nFreeStorage -= nStorage; //Aktueller freier Speicher wird aktualisiert
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion f�gt den �bergebenen Pointer in die ProzessListe ein
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   *Buddy = Zeiger auf die Struktur, welche in die Liste gespeichert werden soll
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
			      0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int addToBuddyProzessList(struct tStorage *pStorage, struct tBuddy *pBuddy)
{
	//Es wird gepr�ft ob schon Elemente in pStorage->pBuddyProzessList vorhanden sind
	if (pStorage->pBuddyProzessList == NULL) //Wenn == NULL sind noch keine ELemente vorhanden
	{
		pStorage->pBuddyProzessList = (struct tBuddy**)malloc(sizeof(struct tBuddy)); //Speicher f�r das erste Element wird angefordert
		if (pStorage->pBuddyProzessList == NULL)
		{
			printf("Es konnte kein Speicher fuer \"pStorage->pBuddyProzessList\" angefordert werden. . .\n");
			return 1;
		}
	}
	else //Es sind schon Elemente in der Liste, speicher wird f�r die Liste erweitert
	{
		pStorage->pBuddyProzessList = (struct tBuddy**)realloc(pStorage->pBuddyProzessList, sizeof(struct tBuddy));
		if(pStorage->pBuddyProzessList == NULL)
		{
			printf("Es konnte kein Speicher fuer \"pStorage->pBuddyProzessList\" angefordert werden. . .\n");
			return 1;
		}
	}
	pStorage->pBuddyProzessList[pStorage->nCounter_P++] = pBuddy; //Element wird in Liste gespeichert
	return 0;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion f�gt den �bergebenen Pointer in die BuddyListe ein
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   *Buddy = Zeiger auf die Struktur, welche in die Liste gespeichert werden soll
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int addToBuddyList(struct tStorage *pStorage,  struct tBuddy *pBuddy)
{
	//Es wird gepr�ft ob schon Elemente in pStorage->pBuddyProzessList vorhanden sind
	if (pStorage->pBuddyList == NULL) //Wenn == NULL sind noch keine ELemente vorhanden
	{
		pStorage->pBuddyList = (struct tBuddy**)malloc(sizeof(struct tBuddy)); //Speicher f�r das erste Element wird angefordert
		if (pStorage->pBuddyList == NULL)
		{
			printf("Es konnte kein Speicher fuer \"pStorage->pBuddyProzessList\" angefordert werden. . .\n");
			return 1;
		}
	}
	else //Es sind schon Elemente in der Liste, speicher wird f�r die Liste erweitert
	{
		pStorage->pBuddyList = (struct tBuddy**)realloc(pStorage->pBuddyList, sizeof(struct tBuddy));
		if (pStorage->pBuddyList == NULL)
		{
			printf("Es konnte kein Speicher fuer \"pStorage->pBuddyProzessList\" angefordert werden. . .\n");
			return 1;
		}
	}
	pStorage->pBuddyList[pStorage->nCounter_B++] = pBuddy; //Element wird in Liste gespeichert
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion initialisiert die Struktur tBuddy
	Parameter: nStorage = Speicher, der f�r den Prozess passend ist
			   nProzessSize = Gr��e des Speichers, des Prozesses
			   cProzessName = Name des Prozesses
	           bFree		= Ob Speicher frei oder belegt
	R�ckgabewert: NULL = Wenn ein Fehler aufgetreten ist
			      pBuddy = Pointer auf die Struktur, f�r die Speicher angefordert wurde
--------------------------------------------------------------------------------------------------- */
struct tBuddy* Init_tBuddy()
{
	struct tBuddy *pBuddy;

	pBuddy = (struct tBuddy*)malloc(sizeof(struct tBuddy));
	if (pBuddy == NULL)
		return NULL;

	pBuddy->nPID = 0;
	pBuddy->nStorageSize = 0;
	pBuddy->nProzessSize = 0;
	strcpy_s(pBuddy->cProzessName, "");
	pBuddy->bFree = true;
	pBuddy->pBuddy = NULL;
	pBuddy->pNextBuddy = NULL;
	pBuddy->pPreviousBuddy = NULL;

	return pBuddy;
}

