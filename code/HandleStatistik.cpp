#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

#include "HandleStatistik.h"
#include "BuddySpV_Prozess.h"

/*---------------------------------------------------------------------------------------------------
	Top-Level Funktion, in der alle anderen Funktionen aufgerufen werden, die Informationen ausgeben
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int statistik(struct tStorage *pStorage)
{
	system("cls");

	displayProzess(pStorage);
	displayStorage(pStorage);
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion gibt eine Tabelle zu allen laufenden Prozessen aus
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void displayProzess(struct tStorage *pStorage)
{
	int i = 0;
	if (pStorage->pBuddyProzessList != NULL && pStorage->nCounter_P > 0)
	{
		printf("PID\t|Prozessname\t\t|Prozessgroesse\t|Bereitgestellter Speicher\n");
		
		for (; i < pStorage->nCounter_P; i++)
		{
			if (pStorage->pBuddyProzessList[i] != NULL)
			{
				printf("%d\t|", pStorage->pBuddyProzessList[i]->nPID);
				if (strlen(pStorage->pBuddyProzessList[i]->cProzessName) <= 6)
					printf("%s\t\t\t", pStorage->pBuddyProzessList[i]->cProzessName);
				else if(strlen(pStorage->pBuddyProzessList[i]->cProzessName) < 15)
					printf("%s\t\t", pStorage->pBuddyProzessList[i]->cProzessName);
				else if(strlen(pStorage->pBuddyProzessList[i]->cProzessName) < 23)
					printf("%s\t", pStorage->pBuddyProzessList[i]->cProzessName);
				else
					printf("%s", pStorage->pBuddyProzessList[i]->cProzessName);
				printf("|%d\t\t|%d\n", pStorage->pBuddyProzessList[i]->nProzessSize, pStorage->pBuddyProzessList[i]->nStorageSize);
			}
		}
		printf("--------------------------------------------------------------------------\n\n");
	}
	else
		printf("Es sind noch keine Prozesse in der Liste vorhanden!\n\n");
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion gibt Informationen über den belegten-/ freien Speicher aus
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: -
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
		printf("Anzahl freier Speicherbloecke\n");
		printf("-----------------------------\n");
		printf("Blockgroesse\tAnzahl\n");
		
		//Die Schleife durchläuft die Liste und gibt alle Speicherblöcke mit ihrer Anzahl aus
		while (pCB != NULL)
		{
			if (pCB->nCounter != 0)
				printf("[%d]\t\t%d\n", pCB->nValue, pCB->nCounter);
			pCB = pCB->pNextCB;
		}
	}
	printf("\nBelegter Speicher\t|Freier Speicher\t|Maximaler Speicher\n");
	printf("%d\t\t\t|%d\t\t\t|%d\n\n", pStorage->nMaxStorageSize - pStorage->nFreeStorage, pStorage->nFreeStorage, pStorage->nMaxStorageSize);

	release(pFirst);
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion ermittelt wie viele freien Speicher es mit entsprechender größe gibt. Also wie viele Buddys z.B
	mit der größe 32 / 128 / usw. noch frei sind. 
	Parameter: *pCB = Liste in der die Werte gespeichert werden
			   *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int countBuddys(struct tCountBuddys *pCB, struct tStorage *pStorage)
{
	int i = 0;
	
	struct tCountBuddys *pFirst = pCB;

	if (pStorage->pBuddyList != NULL)
	{
		//Die Schleife läuft durch die BuddyListe
		for (; i < pStorage->nCounter_B; i++)
		{
			if (pStorage->pBuddyList[i] != NULL)
			{
				if (pStorage->pBuddyList[i]->bFree == true && pStorage->pBuddyList[i]->nStorageSize != 0) //Es wird geprüft ob der Buddy frei ist
						if (addNewBuddyToList(pCB, pStorage->pBuddyList[i]->nStorageSize) == 1) //Es wird geprüft ob es bereits einen bestehenden Speicherblock mit der größe gibt, oder ob ein neues Element hinzugefügt wird
							return 1;
				if (pStorage->pBuddyList[i]->pBuddy->bFree == true && pStorage->pBuddyList[i]->pBuddy->nStorageSize != 0) //Es wird geprüft ob der Buddy des Buddys frei ist
						if (addNewBuddyToList(pCB, pStorage->pBuddyList[i]->pBuddy->nStorageSize) == 1)  //Es wird geprüft ob es bereits einen bestehenden Speicherblock mit der größe gibt, oder ob ein neues Element hinzugefügt wird
							return 1;
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion durchläuft die CountBuddys-Liste. Es wird nach einem bereits vorhandenen Element mit der
	gleichen Storage größe gesucht. Sollte kein Element mit der gleichen Storage größe vorhanden sein,
	wird ein neues Element in die Liste eingefügt.
	Parameter: *pCB = Liste mit allen freien Buddys einer gewissen Speichergröße
			   nStorageSize = Speichergröße, nach der gesucht wird
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int addNewBuddyToList(struct tCountBuddys *pCB, int nStorageSize)
{	
	struct tCountBuddys *pTmpOld;

	//Schleife durchläuft die CountBuddy Liste. Sollte kein passendes Element gefunden werden, ist pCB automatisch schon das letzte Element
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

	//Es wurde keine passende Gruppe gefunden, darum wird eine neue Gruppe eröffnet
	if ((pCB = Init_tCountBuddys()) == NULL) //Es wird Speicher für einen neuen Speicherblock angefordert
		return 1; //Es ist zu einem Fehler gekommen
	pCB->nValue = nStorageSize;
	pCB->nCounter++;
	pTmpOld->pNextCB = pCB;

	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion fordert Ressourcen für die Struktur tCountBuddys an und initialisert diese
	Parameter: *pCB = Liste für die Speicher angeforder, oder erweitert werden soll
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
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
	Gibt die angeforderten Ressourcen für tCountBuddys wieder frei
	Parameter: *pCB = Liste für die Speicher angeforder, oder erweitert werden soll
	Rückgabewert: -
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