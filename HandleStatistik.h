#ifndef HANDLESTATISTIK_H
#define HANDLESTATISTIK_H

struct tCountBuddys {
	int nValue;
	int nCounter;
};

static int nCounter = 0;

int statistik(struct tStorage *pStorage);
void displayProzess(struct tStorage *pStorage);
void displayStorage(struct tStorage *pStorage);
int countBuddys(struct tCountBuddys **pCB, struct tStorage *pStorage);
int addNewBuddyToList(struct tCountBuddys **pCB, struct tStorage *pStorage, int nPosBuddyList, int nPosCountBuddys);
struct tCountBuddys** Init_tCountBuddys(struct tCountBuddys** pCB, int nPos);

#endif
