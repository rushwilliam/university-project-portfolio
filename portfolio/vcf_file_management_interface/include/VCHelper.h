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
// Write an individual property to a file
void writeProperty(FILE *fptr, Property *prop);
// Write the value of a date to a file (not the name or parameters)
void writeDateValue(FILE *fptr, DateTime *date);
// Check for errors in a DateTime
VCardErrorCode validateDate(DateTime *date);

#endif
