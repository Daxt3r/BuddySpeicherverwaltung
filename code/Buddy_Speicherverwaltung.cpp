// Buddy_Speicherverwaltung.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
// Autor: Marcel Crone
// MarNr: 1221008

#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>

#include "HandleInput.h"
#include "HandleProzessTermination.h"
#include "HandleStatistik.h"
#include "HandleStorageChanges.h"
#include "BuddySpV_Prozess.h"
#include "util.h"

/*
	TODO: -
*/

//forward declaration
//---------------------------------------------------------------------------------------------------

struct tStorage* Init_tStorage();
int DefineStorage(struct tStorage *pStorage);
void DisplayMenu();
void SelectMenupoint(struct tStorage *pStorage, int i);
void release_tStorage(struct tStorage *pStorage);

//---------------------------------------------------------------------------------------------------

/*---------------------------------------------------------------------------------------------------
	Einsteigspunkt für das Programm. Top-Level Funktion
	Die Funktion gibt das Menü in der Konsole aus
	Parameter: -
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
int main()
{
	int i = 0;
	struct tStorage *pStorage;
	
	pStorage = Init_tStorage(); //Structur tBuddy wird initialisiert
	if (DefineStorage(pStorage) == 1) //Speicher für die Simulation wird vom Benutzer eingegebn
	{
		printf("Es konnte kein Speicher fuer das erste Element in der Liste angefordert werden!\n");
		printf("Fehler in der Funktion \"DefineStorage\". . .\n");
		return 0;
	}

	while (true)
	{
		DisplayMenu(); //Menü wird ausgegeben
		if ((i = HandleUserInputMenu()) == -1) //Eingabe vom Benutzer wird behandelt
		{
			printf("Es ist ein Fehler in der Funktion \"HandleUserInput\" aufgetreten. . .\n");
			break;
		}
		if (i == 5) //Benutzer hat das Programm beendet
		{
			system("cls"); //Inhalt der Konsole wird gelöscht
			printf("Evtl. laufende Prozesse werden beendet. . .\n");
			endAllProzesses(pStorage);
			printf("Die Simulation der Buddy-Speicherverwaltung wurde beendet. . .\n\n");
			break;
		}
		SelectMenupoint(pStorage, i); //Die Eingabe vom Benutzer wird verarbeitet

		system("Pause");
		system("cls"); //Inhalt der Konsole wird gelöscht
		i = 0;
	}
	
	release_tStorage(pStorage); //Angeforderte Ressourcen für pStorage werden wieder freigegeben
	system("Pause");
    return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion begrüßt den Benutzer und fordert ihn zur Eingabe des Speicherbereiches auf.
	Zusätzlich werden dadurch einige Werte der Struktur tStorage initialisert und das erste Element in der Liste erstellt
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	Rückgabewert: 1 = Wenn ein Fehler aufgetreten ist
				  0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int DefineStorage(struct tStorage *pStorage)
{
	printf("Willkommen zur Buddy Speicherverwaltung!\n\n");
	printf("Zum Beginn muss der zu simulierende Speicher festgelegt werden.\n");
	printf("Moegliche Werte sind: ");
	pow(MAX_STORAGE); //Gibt alle möglichen Werte aus, die als Speicher angegeben werden dürfen

	pStorage->nMaxStorageSize = HandleUserInputStorage(); //Größe des Speichers wird in pStorage->nMaxStorageSize gespeichert
	pStorage->nFreeStorage = pStorage->nMaxStorageSize;	  //Freier Speicher wird auf pStorage->nMaxStorageSize gesetzt
	
	pStorage->pFirstBuddy = Init_tBuddy(); //Erste Element in der Liste wird erstellt <-- Erstspart einiges an Arbeit
	if (pStorage->pFirstBuddy == NULL)
		return 1;
	pStorage->pFirstBuddy->nStorageSize = pStorage->nMaxStorageSize; //Anfangsspeicher wird gesetzt
	pStorage->pFirstBuddy->nStorageSizeIntern = pStorage->nMaxStorageSize; //Interner Speicher wird gesetzt, macht das debuggen einfacher

	pStorage->pFirstBuddy->pBuddy = Init_tBuddy(); //Buddy für das erste Element in der Liste wird erstellt, damit das Konzept nicht inkonsistent ist. außerdem löst das ein Problem, beim der Buddy-Such-Schleife
	if (pStorage->pFirstBuddy->pBuddy == NULL)
		return 1;

	if (addToBuddyList(pStorage, pStorage->pFirstBuddy) == 1) //Es muss nur ein Buddy in die Liste eingefügt werden, da diese aufeinander referenzieren
		return 1;
	return 0;
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion gibt das Menü in der Konsole aus
	Parameter: -
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void DisplayMenu()
{
	printf("\n(1) Einen Prozess starten\n");
	printf("(2) Einen Prozess beenden\n");
	printf("(3) Statistik ausgeben lassen\n");
	printf("(4) Alle Prozesse beenden\n");
	printf("(5) Programm beenden\n\n");
}

/*---------------------------------------------------------------------------------------------------
	Die Funktion wählt anhand der Eingabe den entsprechenden Menüpunkt aus
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
					   i = Menüpunkt, der Ausgewählt wurde
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void SelectMenupoint(struct tStorage *pStorage, int i)
{
	switch(i)
	{
		case 1: startProzess(pStorage);
			break;
		case 2: endProzess(pStorage);
			break;
		case 3: statistik(pStorage);
			break;
		case 4: endAllProzesses(pStorage);
			break;
		default: printf("Unbekannter Wert: %d\n", i);
			break;
	}
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion fordert Speicher für die Struktur tStorage und initialisiert die Werte in der Struktur
	Parameter: -
	Rückgabewert: NULL = Wenn ein Fehler aufgetreten ist
				  pStorage = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
struct tStorage* Init_tStorage()
{
	struct tStorage *pStorage;
	pStorage = (struct tStorage*)malloc(sizeof(struct tStorage));
	if (&pStorage == NULL)
	{
		printf("Es konnte kein Speicher fuer die Struktur \"tStorage\" angefordert werden");
		return NULL;
	}
	pStorage->nMaxStorageSize = 0;
	pStorage->nFreeStorage = 0;
	pStorage->nCounter_P = 0; 
	pStorage->nCounter_B = 0; 
	pStorage->pFirstBuddy = NULL;
	pStorage->pBuddyProzessList = NULL;
	pStorage->pBuddyList = NULL;

	return pStorage;
}

/* ---------------------------------------------------------------------------------------------------
	Die Funktion gibt die angeforderten Ressourcen wieder frei
	Parameter: *pStorage
	Rückgabewert: -
--------------------------------------------------------------------------------------------------- */
void release_tStorage(struct tStorage *pStorage)
{
	if (pStorage != NULL)
	{
		pStorage->nMaxStorageSize = 0;
		pStorage->nFreeStorage = 0;

		if (pStorage->pBuddyProzessList != NULL)
		{
			free(pStorage->pBuddyProzessList);
			pStorage->pBuddyProzessList = NULL;
		}
		if (pStorage->pFirstBuddy != NULL)
		{
			free(pStorage->pFirstBuddy);
			pStorage->pFirstBuddy = NULL;
		}
		free(pStorage);
		pStorage = NULL;
	}
}

