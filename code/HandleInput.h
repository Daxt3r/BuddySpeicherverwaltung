#ifndef HANDLEINPUT_H
#define HANDLEINPUT_H

#define MIN_MENUPOINT_VALUE 1
#define MAX_MENUPOINT_VALUE 5
#define MENUPOINT_LENGTH 3 //"\n" und "\0" nicht vergessen!

#define MAX_PROZESSID_LENGTH 6

#define MIN_STORAGE 1
#define MAX_STORAGE 8192
#define MAX_STORAGE_STRING_LENGTH 22 //Stringlänge ist so groß, da bei einer zu geringen größe Werte noch im Eingangsstream waren und zu Fehlern geführt haben

#define DEFAULT_MIN_STORAGE 16

#define PROZESS_NAME_LENGTH 52

#define DEFAULT_PROZESS_NAME "defaultProzess"

static int lnr = 0;

//---------------------------------------------------------------------------------------------------

int HandleUserInputStorage();
int HandleUserInputMenu();
int HandleUserInputProzessStorage(struct tStorage *pStorage, int nFreeStorage);
void HandleUserInputProzessName(char *c);
int HandleUserInputEndProzess();
int CheckInput(char *c, int nMin, int nMax, int nLength);
int CheckIfStorageSuitable(float f);
int CheckFreeStorage(struct tStorage *pStorage, int nStorage);
int CheckIfBackToMenu(char *c);

//---------------------------------------------------------------------------------------------------

#endif
