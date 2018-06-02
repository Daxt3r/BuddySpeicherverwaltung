#ifndef BUDDYSPV_PROZESS_H
#define BUDDYSPV_PROZESS_H

#define MAXLENGTH 101

static int nCounter_PID = 1; //Counter f�r die Prozess ID
//---------------------------------------------------------------------------------------------------

struct tBuddy {
	int nPID;						//ID des Prozesses um nach ihm zu suchen
	int nStorageSizeIntern;			//Gr��e des Speicherbereiches, wird aber nicht auf 0 gesetzt, wenn der Buddy in verwendung ist
	int nStorageSize;				//Gr��e des Speicherbereiches
	int nProzessSize;				//Gr��e des Prozesses
	char cProzessName[MAXLENGTH];	//Name des Prozesses
	struct tBuddy *pBuddy;			//Adresse des Buddies
	struct tBuddy *pNextBuddy;		//Adresse auf den darunter liegenden Speicherbereich - NULL wenn es der kleinste Speicherbereich (1) ist
	struct tBuddy *pPreviousBuddy;	//Adresse auf den dar�berliegenden Speicher - wird beim zusammenf�gen des Speichers ben�tigt
	bool bFree;						//Gibt an, ob der Speicher noch frei ist oder nicht
									//true  = frei
									//false = belegt
};

struct tStorage {
	int nMaxStorageSize;				//Maximaler Speicher, der zur Verf�gung steht
	int nFreeStorage;					//Gibt an, wie viel freier Speicherplatz noch zur Verf�gung steht
	int nCounter_P;						//Counter f�r die Prozesse in der Liste
	int nCounter_B;						//Counter f�r die Buddys in der Liste
	struct tBuddy *pFirstBuddy;			//Zeiger auf das erste Element in der Liste
	struct tBuddy **pBuddyProzessList;	//Liste mit allen Prozessen, die zurzeit Laufen
	struct tBuddy **pBuddyList;			//Liste mit alle Buddys, die erstellt wurden
};

//---------------------------------------------------------------------------------------------------

int startProzess(struct tStorage *pStorage);
int storageSize(int nValue);
int searchInBuddyList(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzessName);
int splitStorage(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzesName);
struct tBuddy* searchForFreeStorage(struct tStorage *pStorage, int nStorage);
int createBuddy(struct tBuddy *pTmpBuddy);
void addProzess(struct tStorage *pStorage, struct tBuddy *pBuddy, int nStorage, int nProzessSize, char *cProzessName);
int addToBuddyProzessList(struct tStorage *pStorage, struct tBuddy *pBuddy);
int addToBuddyList(struct tStorage *pStorage, struct tBuddy *pBuddy);
struct tBuddy* Init_tBuddy();

//---------------------------------------------------------------------------------------------------

#endif
