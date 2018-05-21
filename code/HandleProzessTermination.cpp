#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

#include "HandleInput.h"
#include "HandleProzessTermination.h"
#include "BuddySpV_Prozess.h"


/*---------------------------------------------------------------------------------------------------
	Die Funktion beendet einen laufenden Prozess
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int endProzess(struct tStorage *pStorage)
{
	int nPID = 0;
	int nPos = 0;
	/*
		In der Schleife wird der Benutzer aufgefordert die Prozess ID des zu beendenen Prozess einzugeben.
		Es wird gepr�ft ob es diesen Prozess in der Liste gibt, wenn ja wird die Schleife verlassen und der 
		Prozess wird beendet. */

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
	printf("Prozess wurde beendet. . .\n\n");
	
	checkIfMergable(pStorage); //Es wird gepr�ft, ob Buddys wieder zusammengef�gt werden k�nnen

	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion sucht nach dem Prozess mit der vom Benutzer eingegebenen Prozess ID. Wenn der entsprechende
	Prozess gefunden wurde, wird dessen Position in der Liste zur�ck gegeben.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
			   nPID = ID des Prozesses, der Beendet werden soll
	R�ckgabewert: i = Position in der Prozessliste
				  -1 = Wenn der Prozess nicht in der Liste gefunden wurde
--------------------------------------------------------------------------------------------------- */
int searchForProzess(struct tStorage *pStorage, int nPID)
{
	int i = 0;

	for (; i < pStorage->nCounter_P; i++)
		if (pStorage->pBuddyProzessList[i]->nPID == nPID)
			return i;
	return -1;
}

/*---------------------------------------------------------------------------------------------------
	Gibt setzt die Werte des Prozesses an der Position nPos zur�ck (gibt ihn frei)
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
				nPos = Position in der Prozessliste
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void releaseProzess(struct tStorage *pStorage, int nPos)
{
	pStorage->pBuddyProzessList[nPos]->bFree = true;
	pStorage->pBuddyProzessList[nPos]->nPID = 0;
	pStorage->pBuddyProzessList[nPos]->nProzessSize = 0;
	strcpy_s(pStorage->pBuddyProzessList[nPos]->cProzessName, "");

	pStorage->nFreeStorage += pStorage->pBuddyProzessList[nPos]->nStorageSize; //Freier Speicher wird wieder erh�ht
	pStorage->pBuddyProzessList[nPos] = NULL; //Prozess wird aus der Liste geschmissen
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion pr�ft ob die Buddys wieder zusammengef�gt werden k�nnen, geht nur, wenn beide Frei sind.
	Die Funktion, wird am Besten nach jeder freigabe eines Prozesses aufgerufen.
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void checkIfMergable(struct tStorage *pStorage)
{
	int i = 0;

	//Schleife durchl�uft die BuddyListe von hinten nach vorne (da es Sinn macht, zuerst die kleinen Buddys
	//wieder zusammenzuf�gen, da ansonsten die gro�en nicht zusammengef�gt werden k�nnen)
	//und pr�ft ob Buddys wieder zusammengef�gt werden k�nnen
	for (i = pStorage->nCounter_B-1; i > 0; i--)
	{
		if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->pNextBuddy == NULL && pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->pBuddy->pNextBuddy == NULL)
		{
			//Dar�berliegender Buddy wird wieder als verf�gbarer Speicher markiert
			pStorage->pBuddyList[i]->pPreviousBuddy->nStorageSize = pStorage->pBuddyList[i]->nStorageSize * 2;
			pStorage->pBuddyList[i]->pPreviousBuddy->bFree = true;
			pStorage->pBuddyList[i]->pPreviousBuddy->pNextBuddy = NULL;

			//Speicher f�r die Buddys wird freigegeben
			free(pStorage->pBuddyList[i]->pBuddy);
			free(pStorage->pBuddyList[i]);
			pStorage->pBuddyList[i] = NULL; //Zeiger auf das Element in der Liste wird NULL gesetzt

			pStorage->nCounter_B--; //Anzahl der Buddys in der Liste wird verringert
		}
	}
}
/* ---------------------------------------------------------------------------------------------------
	Die Funktion beendet alle laufenden Prozesse
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int endAllProzesses(struct tStorage *pStorage)
{
	int i = 0;

	if (pStorage->pBuddyList != NULL)
	{
		//Die Schleife durchl�uft alle Buddys und gibt diese frei. Mit ausnahme des ersten Buddys in der Liste!
		for (i = 1; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->pBuddy != NULL)
				{
					free(pStorage->pBuddyList[i]->pBuddy);
					pStorage->pBuddyList[i]->pBuddy = NULL;
				}
				free(pStorage->pBuddyList[i]);
				pStorage->pBuddyList[i] = NULL;
			}
		}
	}
	if (pStorage->pBuddyList != NULL)
	{
		free(pStorage->pBuddyList);
		pStorage->pBuddyList = NULL;
	}
	if (pStorage->pBuddyProzessList != NULL)
	{
		free(pStorage->pBuddyProzessList);
		pStorage->pBuddyProzessList = NULL;
	}
	pStorage->pFirstBuddy->pNextBuddy = NULL;
	pStorage->pFirstBuddy->nStorageSize = pStorage->nMaxStorageSize;
	pStorage->nFreeStorage = pStorage->nMaxStorageSize;
	pStorage->nCounter_B = 0;

	if (pStorage->nCounter_P > 0)
		printf("Alle laufenden Prozesse wurden erfolgreich beendet. . .\n");
	else
		printf("Es mussten keine Prozesse beendet werden. . .\n");
	
	pStorage->nCounter_P = 0;

	return 0;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion gibt die angeforderten Ressourcen wieder frei
	Parameter: *pBuddy
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void release_tBuddy(struct tBuddy *pBuddy)
{

}