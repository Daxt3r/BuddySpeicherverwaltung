#include "stdafx.h"
#include <stdlib.h>

#include "HandleStorageChanges.h"
#include "BuddySpV_Prozess.h"


/*---------------------------------------------------------------------------------------------------
	Die Funktion erh�ht den vorhandenen Speicher. Speicher kann nur verdoppelt werden.
	Aus 16 kann nur 32, bzw aus 1024 -> 2048 werden. Damit alle Buddys am Ende wieder zusammengef�gt werden k�nnen
	Parameter: *pStorage = Zeiger auf die Struktur tStorage
	R�ckgabewert: 1 = Wenn ein Fehler aufgetreten ist
			      0 = Wenn kein Fehler aufgetreten ist
--------------------------------------------------------------------------------------------------- */
int raise_storage(struct tStorage *pStorage)
{
	printf("raise_storage\n");
	return 0;
}