/*
 * Name: William Rush
 * ID: 1267781
 */

#ifndef _CARDWRAPPER_H
#define _CARDWRAPPER_H

// #include <stdio.h>
// #include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
// #include "LinkedListAPI.h"
// #include "VCParser.h"

char *getCardList();
char *getContactInfo(char *filename);
char *formatDateSQL(DateTime *date);
char *formatDate(DateTime *date);
char *getFormattedDates(char *filename);
void writeNewCard(char *filename, char *fn);
void updateContact(char *filename, char *fn);
int countOptionalProperties(char *filename);
#endif