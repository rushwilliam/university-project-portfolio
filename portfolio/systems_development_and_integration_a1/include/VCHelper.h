/*
 * Name: William Rush
 * ID: 1267781
 */

#ifndef _CARDHELPER_H
#define _CARDHELPER_H

#include <stdio.h>
#include <string.h>

#include "LinkedListAPI.h"
#include "VCParser.h"
// Open a vcard file
VCardErrorCode openFile(FILE **fptr, char *fileName);
// Parse the line of a vcard file and store the data in a property
VCardErrorCode parseProperty(char *line, Property **prop);
// Read a line of a vcard file
VCardErrorCode readLine(FILE *fptr, char *line);
// Convert a date property into a DateTime struct
VCardErrorCode createDate(Property *prop, DateTime **date);
// Remove the first character of a string
void removeFirstCharacter(char *str);

#endif
