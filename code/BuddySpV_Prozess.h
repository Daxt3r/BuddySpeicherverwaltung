#ifndef BUDDYSPV_PROZESS_H
#define BUDDYSPV_PROZESS_H

#define MAXLENGTH 101

static int nCounter_PID = 1; //Counter für die Prozess ID
//---------------------------------------------------------------------------------------------------

struct tBuddy {
	int nPID;						//ID des Prozesses um nach ihm zu suchen
	int nStorageSize;				//Größe des Speicherbereiches
	int nProzessSize;				//Größe des Prozesses
	char cProzessName[MAXLENGTH];	//Name des Prozesses
	struct tBuddy *pBuddy;			//Adresse des Buddies
	struct tBuddy *pNextBuddy;		//Adresse auf den darunter liegenden Speicherbereich - NULL wenn es der kleinste Speicherbereich (1) ist
	struct tBuddy *pPreviousBuddy;	//Adresse auf den darüberliegenden Speicher - wird beim zusammenfügen des Speichers benötigt
	bool bFree;						//Gibt an, ob der Speicher noch frei ist oder nicht
									//true  = frei
									//false = belegt
};

struct tStorage {
	int nMaxStorageSize;				//Maximaler Speicher, der zur Verfügung steht
	int nFreeStorage;					//Gibt an, wie viel freier Speicherplatz noch zur Verfügung steht
	int nCounter_P;						//Counter für die Prozesse in der Liste
	int nCounter_B;						//Counter für die Buddys in der Liste
	struct tBuddy *pFirstBuddy;			//Zeiger auf das erste Element in der Liste
	struct tBuddy **pBuddyProzessList;	//Liste mit allen Prozessen, die zurzeit Laufen
	struct tBuddy **pBuddyList;			//Liste mit alle Buddys, die erstellt wurden
};

//---------------------------------------------------------------------------------------------------

int startProzess(struct tStorage *pStorage);
int storageSize(int nValue);
int searchInBuddyList(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzessName);
int splitStorage(struct tStorage *pStorage, int nStorage, int nProzessSize, char *cProzesName);
struct tBuddy* searchForFreeStorage(struct tStorage *pStorage);
int createBuddy(struct tBuddy *pTmpBuddy);
void addProzess(struct tStorage *pStorage, struct tBuddy *pBuddy, int nStorage, int nProzessSize, char *cProzessName);
int addToBuddyProzessList(struct tStorage *pStorage, struct tBuddy *pBuddy);
int addToBuddyList(struct tStorage *pStorage, struct tBuddy *pBuddy);
struct tBuddy* Init_tBuddy();

//---------------------------------------------------------------------------------------------------

#endif
