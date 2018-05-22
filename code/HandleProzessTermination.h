#ifndef HANDLEPROZESSTERMINATION_H
#define HANDLEPROZESSTERMINATION_H

int endProzess(struct tStorage *pStorage);
int searchForProzess(struct tStorage *pStorage, int nPID);
void releaseProzess(struct tStorage *pStorage, int nPos);
void cleanProzessList(struct tStorage *pStorage);
void checkIfMergable(struct tStorage *pStorage);
int endAllProzesses(struct tStorage *pStorage);

#endif
