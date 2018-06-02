#ifndef HANDLESTATISTIK_H
#define HANDLESTATISTIK_H

	struct tCountBuddys {
		int nValue;
		int nCounter;
		struct tCountBuddys *pNextCB;
	};

int statistik(struct tStorage *pStorage);
void displayProzess(struct tStorage *pStorage);
void displayStorage(struct tStorage *pStorage);
int countBuddys(struct tCountBuddys *pCB, struct tStorage *pStorage);
int addNewBuddyToList(struct tCountBuddys *pCB, int nStorageSize);
struct tCountBuddys* Init_tCountBuddys();
void release(struct tCountBuddys *pCB);

#endif
