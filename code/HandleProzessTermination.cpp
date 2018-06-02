#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

#include "HandleInput.h"
#include "HandleProzessTermination.h"
#include "BuddySpV_Prozess.h"
#include "HandleStatistik.h"


/*---------------------------------------------------------------------------------------------------
	Die Funktion beendet einen laufenden Prozess
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int endProzess(struct tStorage *pStorage)
{
	int nPID = 0;
	int nPos = 0;
	
	system("cls");

	if (pStorage->nCounter_P > 0)
	{
		displayProzess(pStorage); //Laufende Prozesse werden angezeigt

		nPID = HandleUserInputEndProzess();
		if (nPID == -2)
		{
			printf("Vorgang wird abgebrochen, Sie kehren in das Menue zurueck!\n");
			return 0;
		}
		nPos = searchForProzess(pStorage, nPID);
		if (nPos == -1)
		{
			printf("Prozess mit der PID: %d wurde nicht gefunden!\n", nPID);
			return 0;
		}
		releaseProzess(pStorage, nPos); //Prozess wird beendet
		cleanProzessList(pStorage); //Der gelöschte Prozess hinterlässt eine Lücke, die evtl. gefüllt werden muss
		printf("Prozess wurde beendet. . .\n\n");

		checkIfMergable(pStorage); //Es wird geprüft, ob Buddys wieder zusammengefügt werden können
	}
	else
		printf("Es befinden sich noch keine Prozesse in der Liste, die geloescht werden koennen!\n\n");

	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion sucht nach dem Prozess mit der vom Benutzer eingegebenen Prozess ID. Wenn der entsprechende
	Prozess gefunden wurde, wird dessen Position in der Liste zurück gegeben.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   nPID = ID des Prozesses, der Beendet werden soll
	Rückgabewert: i = Position in der Prozessliste
				  -1 = Wenn der Prozess nicht in der Liste gefunden wurde
--------------------------------------------------------------------------------------------------- */
int searchForProzess(struct tStorage *pStorage, int nPID)
{
	int i = 0;

	for (; i < pStorage->nCounter_P; i++)
	{
		if(pStorage->pBuddyProzessList[i] != NULL)
			if (pStorage->pBuddyProzessList[i]->nPID == nPID)
				return i;
	}
	return -1;
}

/*---------------------------------------------------------------------------------------------------
	Gibt setzt die Werte des Prozesses an der Position nPos zurück (gibt ihn frei)
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
				nPos = Position in der Prozessliste
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void releaseProzess(struct tStorage *pStorage, int nPos)
{
	pStorage->pBuddyProzessList[nPos]->bFree = true;
	pStorage->pBuddyProzessList[nPos]->nPID = 0;
	pStorage->pBuddyProzessList[nPos]->nProzessSize = 0;
	strcpy_s(pStorage->pBuddyProzessList[nPos]->cProzessName, "");

	pStorage->nFreeStorage += pStorage->pBuddyProzessList[nPos]->nStorageSize; //Freier Speicher wird wieder erhöht
	pStorage->pBuddyProzessList[nPos] = NULL; //Prozess wird aus der Liste geschmissen
	pStorage->nCounter_P--; //Anzahl der Prozesse wird verringert
}

/*---------------------------------------------------------------------------------------------------
	Nachdem ein Wert aus der ProzessListe gelöscht wurde, der nicht der letzte in dieser war, 
	enthält diese einen NULL Wert, welche zu einem Fehler führen kann. Darum wird die Liste aufgeräumt 
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
void cleanProzessList(struct tStorage *pStorage)
{
	int i = 0;

	while (pStorage->pBuddyProzessList != NULL)
	{
		if (pStorage->pBuddyProzessList[i] == NULL && pStorage->pBuddyProzessList[i + 1] != NULL)
		{
			pStorage->pBuddyProzessList[i] = pStorage->pBuddyProzessList[i + 1];
			pStorage->pBuddyProzessList[i + 1] = NULL;
		}
		i++;
		if (i >= pStorage->nCounter_P)
			break;
	}
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion prüft ob die Buddys wieder zusammengefügt werden können, geht nur, wenn beide Frei sind.
	Die Funktion, wird am Besten nach jeder freigabe eines Prozesses aufgerufen.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void checkIfMergable(struct tStorage *pStorage)
{
	int i = 0;

	//Schleife durchläuft die BuddyListe von hinten nach vorne (da es Sinn macht, zuerst die kleinen Buddys
	//wieder zusammenzufügen, da ansonsten die großen nicht zusammengefügt werden können)
	//und prüft ob Buddys wieder zusammengefügt werden können
	for (i = pStorage->nCounter_B-1; i > 0; i--)
	{
		if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->pNextBuddy == NULL && pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->pBuddy->pNextBuddy == NULL)
		{
			//Darüberliegender Buddy wird wieder als verfügbarer Speicher markiert
			pStorage->pBuddyList[i]->pPreviousBuddy->nStorageSize = pStorage->pBuddyList[i]->nStorageSize * 2;
			pStorage->pBuddyList[i]->pPreviousBuddy->bFree = true;
			pStorage->pBuddyList[i]->pPreviousBuddy->pNextBuddy = NULL;

			//Speicher für die Buddys wird freigegeben
			free(pStorage->pBuddyList[i]->pBuddy);
			free(pStorage->pBuddyList[i]);
			pStorage->pBuddyList[i] = NULL; //Zeiger auf das Element in der Liste wird NULL gesetzt

			pStorage->nCounter_B--; //Anzahl der Buddys in der Liste wird verringert
		}
	}
}
/* ---------------------------------------------------------------------------------------------------
	Die Funktion beendet alle laufenden Prozesse, es werden alle Werte wieder auf den ursprungs Zustand
	zurück gesetzt.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int endAllProzesses(struct tStorage *pStorage)
{
	int i = 0;

	if (pStorage->pBuddyList != NULL)
	{
		//Die Schleife durchläuft alle Buddys und gibt diese frei. Mit ausnahme des ersten Buddys in der Liste!
		for (i = 1; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->pBuddy != NULL)
				{
					free(pStorage->pBuddyList[i]->pBuddy); //Zweiter Buddy wird freigegeben
					pStorage->pBuddyList[i]->pBuddy = NULL;
				}
				free(pStorage->pBuddyList[i]); //Erster Buddy wird freigegeben
				pStorage->pBuddyList[i] = NULL;
			}
		}
	}
	pStorage->nCounter_B = 0;

	if (pStorage->pBuddyList != NULL)
	{
		free(pStorage->pBuddyList);
		pStorage->pBuddyList = NULL;

		if (addToBuddyList(pStorage, pStorage->pFirstBuddy) == 1) //Erster Buddy wird wieder in die Liste eingefügt
			return 1;
	}
	if (pStorage->pBuddyProzessList != NULL)
	{
		free(pStorage->pBuddyProzessList);
		pStorage->pBuddyProzessList = NULL;
	}
	pStorage->pFirstBuddy->pNextBuddy = NULL;
	pStorage->pFirstBuddy->bFree = true;
	pStorage->pFirstBuddy->nStorageSize = pStorage->nMaxStorageSize;
	strcpy_s(pStorage->pFirstBuddy->cProzessName, "\0");
	pStorage->nFreeStorage = pStorage->nMaxStorageSize;
	
	if (pStorage->nCounter_P > 0)
		printf("Alle laufenden Prozesse wurden erfolgreich beendet. . .\n");
	else
		printf("Es mussten keine Prozesse beendet werden. . .\n");
	
	pStorage->nCounter_P = 0;

	return 0;
}