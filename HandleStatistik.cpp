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
	struct tCountBuddys **pCB = NULL;

	if ( (pCB = Init_tCountBuddys(pCB, 0)) == NULL)
	{
		printf("Es ist ein Fehler in der Funktion \"Init_tCountBuddys\" aufgetreten. . .\n");
		return;
	}
	if(countBuddys(pCB, pStorage) == 1)
	{
		printf("Es ist ein Fehler in der Funktion \"countBuddys\" aufgetreten. . .\n");
		return;
	}
	if (pCB[i] != NULL)
	{
		printf("Anzahl freier Speicherbloecke:\n");
		printf("Blockgroesse\t Anzahl\n");
		for(; i < nCounter; i++)
			printf("[%d]\t\t%d\n", pCB[i]->nValue, pCB[i]->nCounter);
	}
	printf("\nBelegter Speicher\t|Freier Speicher\t| Maximaler Speicher\n");
	printf("%d\t\t|%d\t\t|%d\t\t|\n", pStorage->nMaxStorageSize - pStorage->nFreeStorage, pStorage->nFreeStorage, pStorage->nMaxStorageSize);
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion ermittelt wie viele freien Speicher es mit entsprechender gr��e gibt. Also wie viele Buddys z.B
	mit der gr��e 32 / 128 / usw. noch frei sind. 
	Parameter: **pCB = Liste in der die Werte gespeichert werden
			   *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int countBuddys(struct tCountBuddys **pCB, struct tStorage *pStorage)
{
	int i = 0, n = 0;

	if (pStorage->pBuddyList != NULL)
	{
		//Die Schleife l�uft durch die BuddyListe
		for (; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->nStorageSize != 0) //Es wird gepr�ft ob der Buddy frei ist
				{
					if (n == 0) //Erster Wert wird in die Liste tCountBuddys eingef�gt
					{
						pCB[n]->nValue = pStorage->pBuddyList[i]->nStorageSize;
						pCB[n]->nCounter++;
						n++;
						nCounter++; //Z�hler der Elemente in der tCountBuddys-Liste wird erh�ht
					}
					else //Es sind schon Werte in der Liste von tCountBuddys drin
						if (addNewBuddyToList(pCB, pStorage, i, n) == 1)
							return 1;
				}
				if (pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->nStorageSize != 0) //Es wird gepr�ft ob der Buddy des Buddys frei ist
				{
					if (n == 0) //Es ist noch kein Element in der Liste
					{
						pCB[n]->nValue = pStorage->pBuddyList[i]->nStorageSize;
						pCB[n]->nCounter++;
						n++;
						nCounter++; //Z�hler der Elemente in der tCountBuddys-Liste wird erh�ht
					}
					else
						if (addNewBuddyToList(pCB, pStorage, i, n) == 1)
							return 1;
				}
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion durchl�uft die CountBuddys-Liste. Es wird nach einem bereits vorhandenen Element mit der
	gleichen Storage gr��e gesucht. Sollte kein Element mit der gleichen Storage gr��e vorhanden sein,
	wird ein neues Element in die Liste eingef�gt.
	Parameter: **pCB = Liste mit allen freien Buddys einer gewissen Speichergr��e
			   *pStorage = Zeiger auf die Struktur tStorage
			   nPosBuddyListe = Aktuelle Position in der BuddyListe
			   nPosCountBuddys = Aktuelle Position in der CountBuddy Liste (h�chste Position in der Liste)
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int addNewBuddyToList(struct tCountBuddys **pCB, struct tStorage *pStorage, int nPosBuddyList, int nPosCountBuddys)
{
	int e = 0; //Z�hlvariable f�r tCountBuddys-Liste

	//Die Schleife durchl�uft die CountBuddys Liste, in der freie Buddys mit gleich gro�em Storage gelistet werden
	for (; e < nPosCountBuddys; e++)
	{
		if (pCB[e]->nValue == pStorage->pBuddyList[nPosBuddyList]->nStorageSize) //Passende Gruppe wurde gefunden
		{
			pCB[nPosCountBuddys]->nCounter++;
			break;
		}
		else //Es wurde keine passende Gruppe gefunden, darum wird eine neue Gruppe er�ffnet
		{
			if ((pCB = Init_tCountBuddys(pCB, nPosCountBuddys + 1)) == NULL)
				return 1; //Es ist zu einem Fehler gekommen
			pCB[nPosCountBuddys++]->nValue = pStorage->pBuddyList[nPosBuddyList]->nStorageSize;
			pCB[nPosCountBuddys]->nCounter++;
			break;
		}
	}
	nCounter++; //Z�hler der Elemente in der tCountBuddys-Liste wird erh�ht
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion fordert Ressourcen f�r die Struktur tCountBuddys an und initialisert diese
	Parameter: **pCB = Liste f�r die Speicher angeforder, oder erweitert werden soll
			   nPos = Position in der Liste, an welche die Addresse gespeichert werden soll. F�r das erste
				      Element ist 0 zu �bergeben!
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
struct tCountBuddys** Init_tCountBuddys(struct tCountBuddys** pCB, int nPos)
{
	if (pCB == NULL)
	{
		pCB = (struct tCountBuddys**)malloc(sizeof(struct tCountBuddys));
		if (pCB == NULL)
		{
			printf("Es ist konnte kein Speicher fuer \"pCB\" angefordert werden!\n");
			return NULL;
		}
	}
	else
	{
		pCB = (struct tCountBuddys**)realloc(pCB, sizeof(struct tCountBuddys));
		if (pCB == NULL)
		{
			printf("Speicher fuer \"pCB\" konnte nicht erweitert werden!\n");
			return NULL;
		}
	}
	pCB[nPos] = (struct tCountBuddys*)malloc(sizeof(struct tCountBuddys));
	if (pCB[nPos] == NULL)
	{
		printf("Es ist konnte kein Speicher fuer \"pCB[nPos]\" angefordert werden!\n");
		return NULL;
	}
	pCB[nPos]->nValue = 0;
	pCB[nPos]->nCounter = 0;

	return pCB;
}