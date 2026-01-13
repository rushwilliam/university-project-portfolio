/*
 * Name: William Rush
 * ID: 1267781
 */

#include "VCHelper.h"

#define MAX_LINE_SIZE 500

VCardErrorCode openFile(FILE **fptr, char *fileName) {
    if (fileName == NULL || strlen(fileName) == 0) {
        return INV_FILE;
    }
    // File name should not be edited in case it neads to be used later
    char *temp = (char *)malloc(strlen(fileName) + 1);
    strcpy(temp, fileName);
    strtok(temp, ".");
    // Get just the extension
    char *extension = strtok(NULL, ".");
    if (extension == NULL) {
        free(temp);
        return INV_FILE;
    }
    // Check if the extension is either .vcf or .vcard
    if (!(strcmp(extension, "vcf") == 0) && !(strcmp(extension, "vcard") == 0)) {
        free(temp);
        return INV_FILE;
    }
    free(temp);
    *fptr = fopen(fileName, "r");
    if (*fptr == NULL) {
        return INV_FILE;
    }
    return OK;
}

VCardErrorCode parseProperty(char *line, Property **prop) {
    (*prop)->name = NULL;
    (*prop)->group = NULL;
    (*prop)->parameters = NULL;
    (*prop)->values = NULL;
    char *colon = strchr(line, ':');
    if (colon == NULL) {
        return INV_PROP;
    }
    // Split line into two strings
    // This changes the value of line, which is just a temporary value and will be written over after parsing
    *colon = '\0';
    if (strlen(line) == 0 || strlen(colon + 1) == 0) {
        return INV_PROP;
    }
    // Turn line into just the name
    char *nextSemicolon = strchr(line, ';');
    if (nextSemicolon != NULL) {
        *nextSemicolon = '\0';
    }
    
    char *period = strchr(line, '.');
    if (period == NULL) {
        // If there is no group
        (*prop)->name = (char *)malloc(sizeof(char) * strlen(line) + 1);
        strcpy((*prop)->name, line);
        // Set group to empty string
        (*prop)->group = (char *)malloc(sizeof(char));
        (*prop)->group[0] = '\0';
    } else {
        // If there is a group
        *period = '\0';
        (*prop)->name = (char *)malloc(sizeof(char) * strlen(period + 1) + 1);
        strcpy((*prop)->name, period + 1);
        // Will copy until the period, which has been replaced by a newline
        (*prop)->group = (char *)malloc(sizeof(char) * strlen(line) + 1);
        strcpy((*prop)->group, line);
    }

    // List must be created even if there are no parameters
    (*prop)->parameters = initializeList(&parameterToString, &deleteParameter, &compareParameters);
    // Handle the case that there are no parameters, set everything to NULL so the while loop does not run
    line = nextSemicolon == NULL ? NULL : nextSemicolon + 1;
    nextSemicolon = line == NULL ? NULL : strchr(line, ';');

    while (line != NULL) {
        if (nextSemicolon != NULL) {
            *nextSemicolon = '\0';
        }
        char *equals = strchr(line, '=');
        // Check if there is an equals in parameter
        if (equals == NULL) {
            return INV_PROP;
        }
        // Split string by equals
        *equals = '\0';
        if (strlen(line) == 0 || strlen(equals + 1) == 0) {
            return INV_PROP;
        }
        Parameter *param = (Parameter *)malloc(sizeof(Parameter));
        // Copy and store strings
        param->name = (char *)malloc(sizeof(char) * strlen(line) + 1);
        strcpy(param->name, line);
        param->value = (char *)malloc(sizeof(char) * strlen(equals + 1) + 1);
        strcpy(param->value, equals + 1);
        insertBack((*prop)->parameters, (void *)param);
        line = nextSemicolon == NULL ? NULL : nextSemicolon + 1;
        nextSemicolon = line == NULL ? NULL : strchr(line, ';');
    }

    // List must be created even if there are no values
    (*prop)->values = initializeList(&valueToString, &deleteValue, &compareValues);
    line = colon + 1;
    nextSemicolon = strchr(line, ';');
    while (line != NULL) {
        if (nextSemicolon != NULL) {
            *nextSemicolon = '\0';
        }
        char *temp = (char *)malloc(sizeof(char) * strlen(line) + 1);
        strcpy(temp, line);
        insertBack((*prop)->values, (void *)temp);
        line = nextSemicolon == NULL ? NULL : nextSemicolon + 1;
        nextSemicolon = line == NULL ? NULL : strchr(line, ';');
    }
    return OK;
}

VCardErrorCode readLine(FILE *fptr, char *line) {
    if (fgets(line, MAX_LINE_SIZE, fptr) == NULL) {
        // Error specifics should be checked by the calling function (Should card have ended here?)
        return OTHER_ERROR;
    }
    // If this isnt checked, the program could crash in the next if statement. More thorough checking is done when the line is parsed
    if (strlen(line) < 2) {
        return INV_CARD;
    }
    // Check for CRLF
    if (line[strlen(line) - 2] != '\r' || line[strlen(line) - 1] != '\n') {
        return INV_CARD;
    }
    // Remove CRLF
    line[strlen(line) - 2] = '\0';
    return OK;
}

VCardErrorCode createDate(Property *prop, DateTime **date) {
    // (*date)->date = NULL;
    // (*date)->time = NULL;
    // (*date)->text = NULL;
    // A date should have one value
    char *value = getFromFront(prop->values);
    if (value == NULL) {
        return INV_DT;
    }
    Parameter *tempParam = (Parameter *)getFromFront(prop->parameters);
    // If the parameter is text
    if (tempParam != NULL && strcmp(tempParam->name, "VALUE") == 0 && strcmp(tempParam->value, "text") == 0) {
        (*date)->UTC = false;
        (*date)->isText = true;
        // Set date and time to empty strings
        (*date)->date = (char *)malloc(sizeof(char));
        (*date)->date[0] = '\0';
        (*date)->time = (char *)malloc(sizeof(char));
        (*date)->time[0] = '\0';
        // Store value in text
        (*date)->text = (char *)malloc(sizeof(char) * strlen(value) + 1);
        strcpy((*date)->text, value);
        return OK;
    }
    // Check for time zone
    (*date)->UTC = value[strlen(value) - 1] == 'Z';
    if ((*date)->UTC) {
        value[strlen(value) - 1] = '\0';
    }
    (*date)->isText = false;
    (*date)->text = (char *)malloc(sizeof(char));
    (*date)->text[0] = '\0';
    // Look for captial T
    int timeDesignatorIndex = 0;
    while (timeDesignatorIndex < strlen(value) && value[timeDesignatorIndex] != 'T') {
        timeDesignatorIndex++;
    }
    // If value is in date format
    if (timeDesignatorIndex == strlen(value)) {
        (*date)->date = (char *)malloc(sizeof(char) * strlen(value) + 1);
        strcpy((*date)->date, value);
        (*date)->time = (char *)malloc(sizeof(char));
        (*date)->time[0] = '\0';
        return OK;
    }
    // If value is in time format
    if (timeDesignatorIndex == 0) {
        removeFirstCharacter(value);
        (*date)->date = (char *)malloc(sizeof(char));
        (*date)->date[0] = '\0';
        (*date)->time = (char *)malloc(sizeof(char) * strlen(value) + 1);
        strcpy((*date)->time, value);
        return OK;
    }
    // Value is in date-time format
    char *dateString = strtok(value, "T");
    char *timeString = strtok(NULL, "T");
    (*date)->date = (char *)malloc(sizeof(char) * strlen(dateString) + 1);
    strcpy((*date)->date, dateString);
    (*date)->time = (char *)malloc(sizeof(char) * strlen(timeString) + 1);
    strcpy((*date)->time, timeString);
    return OK;
}

void removeFirstCharacter(char *str) {
    char *temp = (char *)malloc(sizeof(char) * strlen(str));
    // Copy characters offset by one
    for (int i = 0; i < strlen(str); i++) {
        temp[i] = str[i + 1];
    }
    strcpy(str, temp);
    free(temp);
}

// Write an individual property to a file
void writeProperty(FILE *fptr, Property *prop) {
    // Write the group if it exists
    if (strlen(prop->group) != 0) {
        fprintf(fptr, "%s.", prop->group);
    }
    // Print the property name
    fprintf(fptr, "%s", prop->name);
    // Iterator for property parameters
    ListIterator paramIterator = createIterator(prop->parameters);
    Parameter *param;
    // Write all parameters
    while ((param = (Parameter *)nextElement(&paramIterator)) != NULL) {
        fprintf(fptr, ";%s=%s", param->name, param->value);
    }
    fprintf(fptr, ":");
    // Iterator for property values
    ListIterator valueIterator = createIterator(prop->values);
    char *value;
    // Print the first value with no semicolon (if it exists)
    if ((value = (char *)nextElement(&valueIterator)) != NULL) {
        fprintf(fptr, "%s", value);
    }
    // Print all other values with a semicolon
    while ((value = (char *)nextElement(&valueIterator)) != NULL) {
        fprintf(fptr, ";%s", value);
    }
    fprintf(fptr, "\r\n");
}

// Write the value of a date to a file (not the name or parameters)
void writeDateValue(FILE *fptr, DateTime *date) {
    if (date->isText) {
        // If date is text, just print text
        fprintf(fptr, "%s", date->text);
    } else {
        // Print date information if it exists
        if (strlen(date->date)) {
            fprintf(fptr, "%s", date->date);
        }
        // Print time information if it exists
        if (strlen(date->time)) {
            fprintf(fptr, "T%s", date->time);
        }
    }
    if (date->UTC) {
        // Add the UTC indicator
        fprintf(fptr, "Z");
    }
    fprintf(fptr, "\r\n");
}

// Check for errors in a DateTime
VCardErrorCode validateDate(DateTime *date) {
    // Check struct requirements
    if (date->date == NULL || date->time == NULL || date->text == NULL) {
        return INV_DT;
    }
    if (date->isText) {
        // If the date is text
        if (date->UTC) {
            return INV_DT;
        }
        if (strlen(date->date) != 0 || strlen(date->time) != 0) {
            return INV_DT;
        }
    } else {
        // If the date is date-and-or-time
        if (strlen(date->text) != 0) {
            return INV_DT;
        }
    }
    return OK;
}