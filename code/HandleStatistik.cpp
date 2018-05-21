#include "stdafx.h"
#include <stdlib.h>

#include "HandleStatistik.h"
#include "BuddySpV_Prozess.h"

/*---------------------------------------------------------------------------------------------------
	Top-Level Funktion, in der alle anderen Funktionen aufgerufen werden, die Informationen ausgeben
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int statistik(struct tStorage *pStorage)
{
	displayProzess(pStorage);
	displayStorage(pStorage);
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion gibt eine Tabelle zu allen laufenden Prozessen aus
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void displayProzess(struct tStorage *pStorage)
{
	int i = 0;
	if (pStorage->pBuddyProzessList != NULL)
	{
		printf("PID\t|Prozessname\t|Prozessgroesse\t|Bereitgestellter Speicher\n");
		for (; i < pStorage->nCounter_P; i++)
		{
			if (pStorage->pBuddyProzessList[i] != NULL)
				printf("%d\t|%s\t|%d\t|%d\n", pStorage->pBuddyProzessList[i]->nPID, pStorage->pBuddyProzessList[i]->cProzessName, pStorage->pBuddyProzessList[i]->nProzessSize, pStorage->pBuddyProzessList[i]->nStorageSize);
		}
	}
	else
		printf("Es sind noch keine Prozesse in der Liste vorhanden!\n\n");
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion gibt Informationen �ber den belegten-/ freien Speicher aus
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void displayStorage(struct tStorage *pStorage)
{
	int i = 0;
	struct tCountBuddys *pCB = Init_tCountBuddys();
	if (pCB == NULL)
		return;

	struct tCountBuddys *pFirst = pCB; //Adresse auf den Anfang der Liste wird gespeichert

	if(countBuddys(pCB, pStorage) == 1)
	{
		printf("Es ist ein Fehler in der Funktion \"countBuddys\" aufgetreten. . .\n");
		return;
	}
	if (pCB != NULL)
	{
		printf("\nAnzahl freier Speicherbloecke:\n");
		printf("Blockgroesse\tAnzahl\n");

		//Die Schleife durchl�uft die Liste und gibt alle Speicherbl�cke mit ihrer Anzahl aus
		while (pCB != NULL)
		{
			if (pCB->nCounter != 0)
				printf("[%d]\t\t%d\n", pCB->nValue, pCB->nCounter);
			pCB = pCB->pNextCB;
		}
	}
	printf("\nBelegter Speicher|Freier Speicher\t| Maximaler Speicher\n");
	printf("%d\t\t|%d\t\t|%d\t\t|\n", pStorage->nMaxStorageSize - pStorage->nFreeStorage, pStorage->nFreeStorage, pStorage->nMaxStorageSize);

	release(pFirst);
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion ermittelt wie viele freien Speicher es mit entsprechender gr��e gibt. Also wie viele Buddys z.B
	mit der gr��e 32 / 128 / usw. noch frei sind. 
	Parameter: *pCB = Liste in der die Werte gespeichert werden
			   *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int countBuddys(struct tCountBuddys *pCB, struct tStorage *pStorage)
{
	int i = 0;
	
	struct tCountBuddys *pFirst = pCB;

	if (pStorage->pBuddyList != NULL)
	{
		//Die Schleife l�uft durch die BuddyListe
		for (; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->nStorageSize != 0) //Es wird gepr�ft ob der Buddy frei ist
						if (addNewBuddyToList(pCB, pStorage->pBuddyList[i]->nStorageSize) == 1) //Es wird gepr�ft ob es bereits einen bestehenden Speicherblock mit der gr��e gibt, oder ob ein neues Element hinzugef�gt wird
							return 1;
				if (pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->pBuddy->nStorageSize != 0) //Es wird gepr�ft ob der Buddy des Buddys frei ist
						if (addNewBuddyToList(pCB, pStorage->pBuddyList[i]->pBuddy->nStorageSize) == 1)  //Es wird gepr�ft ob es bereits einen bestehenden Speicherblock mit der gr��e gibt, oder ob ein neues Element hinzugef�gt wird
							return 1;
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion durchl�uft die CountBuddys-Liste. Es wird nach einem bereits vorhandenen Element mit der
	gleichen Storage gr��e gesucht. Sollte kein Element mit der gleichen Storage gr��e vorhanden sein,
	wird ein neues Element in die Liste eingef�gt.
	Parameter: *pCB = Liste mit allen freien Buddys einer gewissen Speichergr��e
			   nStorageSize = Speichergr��e, nach der gesucht wird
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int addNewBuddyToList(struct tCountBuddys *pCB, int nStorageSize)
{	
	struct tCountBuddys *pTmpOld;

	//Schleife durchl�uft die CountBuddy Liste. Sollte kein passendes Element gefunden werden, ist pCB automatisch schon das letzte Element
	while (pCB != NULL)
	{
		if (pCB->nValue == nStorageSize) //Passende Gruppe wurde gefunden
		{
			pCB->nCounter++;
			return 0;
		}
		pTmpOld = pCB;
		pCB = pCB->pNextCB;
	}

	//Es wurde keine passende Gruppe gefunden, darum wird eine neue Gruppe er�ffnet
	if ((pCB = Init_tCountBuddys()) == NULL) //Es wird Speicher f�r einen neuen Speicherblock angefordert
		return 1; //Es ist zu einem Fehler gekommen
	pCB->nValue = nStorageSize;
	pCB->nCounter++;
	pTmpOld->pNextCB = pCB;

	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion fordert Ressourcen f�r die Struktur tCountBuddys an und initialisert diese
	Parameter: *pCB = Liste f�r die Speicher angeforder, oder erweitert werden soll
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
struct tCountBuddys* Init_tCountBuddys()
{
	struct tCountBuddys *pTmp = NULL;

	pTmp = (struct tCountBuddys*)malloc(sizeof(struct tCountBuddys));
	if (pTmp == NULL)
	{
		printf("Es ist konnte kein Speicher fuer \"pTmp\" angefordert werden!\n");
		return NULL;
	}
	pTmp->nValue = 0;
	pTmp->nCounter = 0;
	pTmp->pNextCB = NULL;

	return pTmp;
}

/*---------------------------------------------------------------------------------------------------
	Gibt die angeforderten Ressourcen f�r tCountBuddys wieder frei
	Parameter: *pCB = Liste f�r die Speicher angeforder, oder erweitert werden soll
	R�ckgabewert: -
--------------------------------------------------------------------------------------------------- */
void release(struct tCountBuddys *pCB)
{
	struct tCountBuddys *pTmp = pCB;

	while (pTmp != NULL)
	{
		pCB = pTmp;
		pTmp = pCB->pNextCB;
		free(pCB);
	}
}