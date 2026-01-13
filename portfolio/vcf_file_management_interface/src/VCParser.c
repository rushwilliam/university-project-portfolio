/*
 * Name: William Rush
 * ID: 1267781
 */

#include "VCParser.h"
#include "VCHelper.h"

#define MAX_LINE_SIZE 500
#define NUM_PROPERTIES 38

void deleteCard(Card* obj) {
    if (obj == NULL) {
        return;
    }
    Card *card = (Card *)obj;
    if (card->fn != NULL) {
        deleteProperty(card->fn);
    }
    clearList(card->optionalProperties);
    free(card->optionalProperties);
    if (card->birthday != NULL) {
        deleteDate(card->birthday);
    }
    if (card->anniversary != NULL) {
        deleteDate(card->anniversary);
    }
    free(obj);
}

char* cardToString(const Card* obj) {
    if (obj == NULL) {
        return NULL;
    }
    Card *card = (Card *)obj;
    // ------------\n{fn}------------\n\0
    char *temp = propertyToString(card->fn);
    int length = strlen(temp) + 27;
    free(temp);
    // BDAY\n_{birthday}
    if (card->birthday != NULL) {
        temp = dateToString(card->birthday);
        length += strlen(temp) + 6;
        free(temp);
    }
    // ANNIVERSARY\n_{anniversary}
    if (card->anniversary != NULL) {
        temp = dateToString(card->anniversary);
        length += strlen(temp) + 13;
        free(temp);
    }
    ListIterator propertyIterator = createIterator(card->optionalProperties);
    void *element;
    while ((element = nextElement(&propertyIterator)) != NULL) {
        temp = card->optionalProperties->printData(element);
        length += strlen(temp);
        free(temp);
    }
    // String to be returned
    char *propString = (char *)malloc(sizeof(char) * length);
    propString[0] = '\0';
    // Temporary string for formatting (theoretical max size)
    temp = propertyToString(card->fn);
    char *toConcat = (char *)malloc(sizeof(char) * strlen(temp) + 27);
    // Header for fn
    sprintf(toConcat, "------------\n%s------------\n", temp);
    free(temp);
    strcat(propString, toConcat);
    // Add dates
    if (card->birthday != NULL) {
        temp = dateToString(card->birthday);
        sprintf(toConcat, "BDAY\n %s", temp);
        free(temp);
        strcat(propString, toConcat);
    }
    if (card->anniversary != NULL) {
        temp = dateToString(card->anniversary);
        sprintf(toConcat, "ANNIVERSARY\n %s", temp);
        free(temp);
        strcat(propString, toConcat);
    }
    // Add properties
    propertyIterator = createIterator(card->optionalProperties);
    while ((element = nextElement(&propertyIterator)) != NULL) {
        char *temp = card->optionalProperties->printData(element);
        strcat(propString, temp);
        free(temp);
    }
    free(toConcat);
    return propString;
}

char* errorToString(VCardErrorCode err) {
    switch (err) {
        case OK:
            return "Ok";
        case INV_FILE:
            return "Invalid file";
        case INV_CARD:
            return "Invalid card";
        case INV_PROP:
            return "Invalid prop";
        case INV_DT:
            return "Invalid date";
        case WRITE_ERROR:
            return "Write error";
        case OTHER_ERROR:
            return "Other error";
    }
    return "Error error";
}

void deleteProperty(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    Property *prop = (Property *)toBeDeleted;
    if (prop->name != NULL) {
        free(prop->name);
    }
    if (prop->group != NULL) {
        free(prop->group);
    }
    clearList(prop->parameters);
    free(prop->parameters);
    clearList(prop->values);
    free(prop->values);
    free(prop);
}

int compareProperties(const void* first,const void* second) {
    return 0;
}

char* propertyToString(void* prop) {
    if (prop == NULL) {
        return NULL;
    }
    Property *property = (Property *)prop;
    // Minimum space: {name}\n_PARAMS\n_VALUES\n\0
    int length = strlen(property->name) + 18;
    // {group}:_
    if (strlen(property->group) != 0) {
        length += strlen(property->group) + 2;
    }
    char *temp;
    // Count parameters
    ListIterator parameterIterator = createIterator(property->parameters);
    void *element;
    while ((element = nextElement(&parameterIterator)) != NULL) {
        // __{param}\n
        temp = property->parameters->printData(element);
        length += strlen(temp) + 3;
        free(temp);
    }
    // Count values
    ListIterator valueIterator = createIterator(property->values);
    while ((element = nextElement(&valueIterator)) != NULL) {
        // __{value}\n
        temp = property->values->printData(element);
        length += strlen(temp) + 3;
        free(temp);
    }
    // String to be returned
    char *propString = (char *)malloc(sizeof(char) * length);
    propString[0] = '\0';
    // Temporarsy string for formatting (theoretical max size)
    char *toConcat = (char *)malloc(sizeof(char) * length);
    // Add group
    if (strlen(property->group) != 0) {
        sprintf(toConcat, "%s: ", property->group);
        strcat(propString, toConcat);
    }
    // Add parameters
    sprintf(toConcat, "%s\n PARAMS\n", property->name);
    strcat(propString, toConcat);
    parameterIterator = createIterator(property->parameters);
    while ((element = nextElement(&parameterIterator)) != NULL) {
        char *temp = property->parameters->printData(element);
        sprintf(toConcat, "  %s\n", temp);
        strcat(propString, toConcat);
        free(temp);
    }
    // Add values
    strcat(propString, " VALUES\n");
    valueIterator = createIterator(property->values);
    while ((element = nextElement(&valueIterator)) != NULL) {
        sprintf(toConcat, "  %s\n", (char *)element);
        strcat(propString, toConcat);
    }
    free(toConcat);
    return propString;
}

void deleteParameter(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    Parameter *param = (Parameter *)toBeDeleted;
    if (param->name != NULL) {
        free(param->name);
    }
    if (param->value != NULL) {
        free(param->value);
    }
    free(param);
}

int compareParameters(const void* first,const void* second) {
    return 0;
}

char* parameterToString(void* param) {
    if (param == NULL) {
        return NULL;
    }
    Parameter *temp = (Parameter *)param;
    // {name}: {value}\n
    int length = strlen(temp->name) + strlen(temp->value) + 3;
    char *paramString = (char *)malloc(sizeof(char) * length);
    // Set format data
    sprintf(paramString, "%s: %s", temp->name, temp->value);
    return paramString;
}

void deleteValue(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    free(toBeDeleted);
}

int compareValues(const void* first, const void* second) {
    return 0;
}

char* valueToString(void* val) {
    if (val == NULL) {
        return NULL;
    }
    char *value = (char *)val;
    char *valueString = (char *)malloc(sizeof(char) * strlen(value) + 1);
    strcpy(valueString, value);
    return valueString;
}

void deleteDate(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    DateTime *date = (DateTime *)toBeDeleted;
    if (date->date != NULL) {
        free(date->date);
    }
    if (date->time != NULL) {
        free(date->time);
    }
    if (date->text != NULL) {
        free(date->text);
    }
    free(date);
}

int compareDates(const void* first,const void* second) {
    return 0;
}

char* dateToString(void* date) {
    if (date == NULL) {
        return NULL;
    }
    DateTime *temp = (DateTime *)date;
    // If date is text, return texts
    if (temp->isText) {
        char *dateString = (char *)malloc(sizeof(char) * strlen(temp->text) + 1);
        strcpy(dateString, temp->text);
        return dateString;
    }
    // UTC\n\0
    int length = 5;
    // DATE:_{date}_
    if (strlen(temp->date) != 0) {
        length += 7 + strlen(temp->date);
    }
    // TIME:_{time}_
    if (strlen(temp->time) != 0) {
        length += 7 + strlen(temp->time);
    }
    // Extra time for "LOCAL"
    if (!temp->UTC) {
        length += 2;
    }
    // String to be returned
    char *dateString = (char *)malloc(sizeof(char) * length);
    dateString[0] = '\0';
    // Temporary string for formatting
    char *toConcat = (char *)malloc(sizeof(char) * length);
    // Add date and time values
    if (strlen(temp->date) != 0) {
        sprintf(toConcat, "DATE: %s ", temp->date);
        strcat(dateString, toConcat);
    }
    if (strlen(temp->time) != 0) {
        sprintf(toConcat, "TIME: %s ", temp->time);
        strcat(dateString, toConcat);
    }
    // Add time zone
    sprintf(toConcat, "%s\n", temp->UTC ? "UTC" : "LOCAL");
    strcat(dateString, toConcat);
    free(toConcat);
    return dateString;
}

VCardErrorCode createCard(char* fileName, Card** obj) {
    *obj = (Card *)malloc(sizeof(Card));
    // Set everything to NULL incase we need to delete it before a value is initalized
    (*obj)->fn = NULL;
    (*obj)->optionalProperties = NULL;
    (*obj)->birthday = NULL;
    (*obj)->anniversary = NULL;
    FILE *fptr = NULL;
    VCardErrorCode errorCode = openFile(&fptr, fileName);
    if (errorCode != OK) {
        deleteCard(*obj);
        *obj = NULL;
        return errorCode;
    }
    // Buffer for reading in data from the file
    char *line = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);
    fgets(line, MAX_LINE_SIZE, fptr);
    // Try to read the begin statement
    if (strcmp(line, "BEGIN:VCARD\r\n") != 0 && 0) {
        free(line);
        deleteCard(*obj);
        *obj = NULL;
        fclose(fptr);
        return INV_CARD;
    }
    // Try to read the version statement
    fgets(line, MAX_LINE_SIZE, fptr);
    if (strcmp(line, "VERSION:4.0\r\n") != 0) {
        free(line);
        deleteCard(*obj);
        *obj = NULL;
        fclose(fptr);
        return INV_CARD;
    }
    (*obj)->optionalProperties = initializeList(&propertyToString, &deleteProperty, &compareProperties);
    // Line will be concatonated to, so it should start as an empty string
    line[0] = '\0';
    char *nextLine = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);
    while (true) {
        errorCode = readLine(fptr, nextLine);
        if (errorCode == OTHER_ERROR) {
            // When the end of the file is reached, check for the end statement
            if (strcmp(line, "END:VCARD") == 0) {
                break;
            } else {
                free(line);
                free(nextLine);
                deleteCard(*obj);
                *obj = NULL;
                fclose(fptr);
                return INV_CARD;
            }
        // Another error is found when reading from the file
        } else if (errorCode != OK) {
            free(line);
            free(nextLine);
            deleteCard(*obj);
            *obj = NULL;
            fclose(fptr);
            return errorCode;
        }
        // Check for line wrap
        if (nextLine[0] ==  ' ' || nextLine[0] == '\t') {
            // Remove line wrap character
            removeFirstCharacter(nextLine);
            strcat(line, nextLine);
        } else {
            // Unless this is the first line
            if (strlen(line) != 0) {
                Property *prop = (Property *)malloc(sizeof(Property));
                errorCode = parseProperty(line, &prop);
                if (errorCode != OK) {
                    free(line);
                    free(nextLine);
                    deleteProperty(prop);
                    deleteCard(*obj);
                    *obj = NULL;
                    fclose(fptr);
                    return errorCode;
                }
                // Check for specific special properties
                if (strcmp(prop->name, "END") == 0) {
                    deleteProperty(prop);
                    break;
                } else if (strcmp(prop->name, "FN") == 0 && (*obj)->fn == NULL) {
                    (*obj)->fn = prop;
                } else if (strcmp(prop->name, "BDAY") == 0 && (*obj)->birthday == NULL) {
                    (*obj)->birthday = (DateTime *)malloc(sizeof(DateTime));
                    createDate(prop, &(*obj)->birthday);
                    deleteProperty(prop);
                } else if (strcmp(prop->name, "ANNIVERSARY") == 0 && (*obj)->anniversary == NULL) {
                    (*obj)->anniversary = (DateTime *)malloc(sizeof(DateTime));
                    createDate(prop, &(*obj)->anniversary);
                    deleteProperty(prop);
                } else {
                    insertBack((*obj)->optionalProperties, (void *)prop);
                }
            }
            strcpy(line, nextLine);
        }
    }
    free(line);
    free(nextLine);
    fclose(fptr);
    // No fn property, invalid vcard
    if  ((*obj)->fn == NULL) {
        deleteCard(*obj);
        *obj = NULL;
        return INV_CARD;
    }

    return OK;
}

VCardErrorCode writeCard(const char* fileName, const Card *obj) {
    FILE *fptr = fopen(fileName, "w");
    // Check for write errors
    if (fptr == NULL || obj == NULL) {
        return WRITE_ERROR;
    }
    // Print header
    fprintf(fptr, "BEGIN:VCARD\r\n");
    fprintf(fptr, "VERSION:4.0\r\n");
    // Print fn property
    writeProperty(fptr, obj->fn);
    // Print birthday if it exists
    if (obj->birthday != NULL) {
        fprintf(fptr, "BDAY");
        if (obj->birthday->isText) {
            fprintf(fptr, ";VALUE=text");
        }
        fprintf(fptr, ":");
        writeDateValue(fptr, obj->birthday);
    }
    // Print anniversary if it exists
    if (obj->anniversary != NULL) {
        fprintf(fptr, "ANNIVERSARY");
        if (obj->anniversary->isText) {
            fprintf(fptr, ";VALUE=text");
        }
        fprintf(fptr, ":");
        writeDateValue(fptr, obj->anniversary);
    }
    // Iterator for other properties
    ListIterator propIterator = createIterator(obj->optionalProperties);
    Property *prop;
    // Print all other properties
    while ((prop = (Property *)nextElement(&propIterator)) != NULL) {
        writeProperty(fptr, prop);
    }
    fprintf(fptr, "END:VCARD\r\n");
    fclose(fptr);
    return OK;
}

VCardErrorCode validateCard(const Card *obj) {
    // Check card requirements
    if (obj == NULL
        || obj->fn == NULL
        || obj->optionalProperties == NULL) {
        return INV_CARD;
    }
    // Cardinality of a property
    typedef enum Cardinality {
        NONE, // Properties that cannot be in optional properties
        ONE_MUST, // 1
        ONE_MAY, // *1
        ONE_OR_MORE_MUST, // 1*
        ONE_OR_MORE_MAY // *
    } Cardinality;
    // Requirements to be checked for each property
    typedef struct PropRequirements {
        char name[100];
        Cardinality cardinality;
        int minNumValues;
        int maxNumValues;
    } PropRequirements;
    // All requirements for each property
    PropRequirements propRequirements[NUM_PROPERTIES] = {
        {"BEGIN", NONE, 1, 1},
        {"END", NONE, 1, 1},  
        {"SOURCE", ONE_OR_MORE_MAY, 1, 1},  
        {"KIND", ONE_MAY, 1, 1},  
        {"XML", ONE_OR_MORE_MAY, 1, 1},  
        {"FN", NONE, 1, 1},  
        {"N", ONE_MAY, 5, 5},  
        {"NICKNAME", ONE_OR_MORE_MAY, 1, 1},  
        {"PHOTO", ONE_OR_MORE_MAY, 1, 1},
        {"BDAY", NONE, 1, 1},
        {"ANNIVERSARY", NONE, 1, 1},
        {"GENDER", ONE_MAY, 1, 2},
        {"ADR", ONE_OR_MORE_MAY, 7, 7},
        // TODO:
        // In the ABNF structure, TEL apparently only has one value, but it can have semicolons in it
        // Pretty sure the test harness treats it as two different values
        {"TEL", ONE_OR_MORE_MAY, 1, 2}, 
        {"EMAIL", ONE_OR_MORE_MAY, 1, 1}, 
        {"IMPP", ONE_OR_MORE_MAY, 1, 1}, 
        {"LANG", ONE_OR_MORE_MAY, 1, 1}, 
        {"TZ", ONE_OR_MORE_MAY, 1, 1}, 
        {"GEO", ONE_OR_MORE_MAY, 1, 1}, 
        {"TITLE", ONE_OR_MORE_MAY, 1, 1}, 
        {"ROLE", ONE_OR_MORE_MAY, 1, 1}, 
        {"LOGO", ONE_OR_MORE_MAY, 1, 1}, 
        {"ORG", ONE_OR_MORE_MAY, 1, -1}, 
        {"MEMBER", ONE_OR_MORE_MAY, 1, 1}, 
        {"RELATED", ONE_OR_MORE_MAY, 1, 1}, 
        {"CATEGORIES", ONE_OR_MORE_MAY, 1, 1}, 
        {"NOTE", ONE_OR_MORE_MAY, 1, 1}, 
        {"PRODID", ONE_MAY, 1, 1}, 
        {"REV", ONE_MAY, 1, 1}, 
        {"SOUND", ONE_OR_MORE_MAY, 1, 1}, 
        {"UID", ONE_MAY, 1, 1}, 
        {"CLIENTPIDMAP", ONE_OR_MORE_MAY, 2, 2}, 
        {"URL", ONE_OR_MORE_MAY, 1, 1}, 
        {"VERSION", NONE, 1, 1}, 
        {"KEY", ONE_OR_MORE_MAY, 1, 1}, 
        {"FBURL", ONE_OR_MORE_MAY, 1, 1}, 
        {"CALADRURI", ONE_OR_MORE_MAY, 1, 1}, 
        {"CALURI", ONE_OR_MORE_MAY, 1, 1} 
    };
    // Keep track of how often each property appears in the card
    int propCount[NUM_PROPERTIES];
    for (int i = 0; i < NUM_PROPERTIES; i++) {
        propCount[i] = 0;
    }
    // Check fn details
    if (obj->fn->name == NULL
        || strlen(obj->fn->name) == 0
        || obj->fn->group == NULL
        || obj->fn->parameters == NULL
        || obj->fn->values == NULL) {
        return INV_PROP;
    }
    // Fn should have one value
    if (getLength(obj->fn->values) != 1) {
        return INV_PROP;
    }
    // Iterator for fn parameters
    ListIterator fnParamIterator = createIterator(obj->fn->parameters);
    Parameter *param;
    // Check fn parameter requirements
    while ((param = (Parameter *)nextElement(&fnParamIterator)) != NULL) {
        if (param->name == NULL
            || strlen(param->name) == 0
            || param->value == NULL
            || strlen(param->value) == 0) {
            return INV_PROP;
        }
    }
    // Iterator for optional properties
    ListIterator propIterator = createIterator(obj->optionalProperties);
    Property *prop;
    while ((prop = (Property *)nextElement(&propIterator)) != NULL) {
        // Check property requirements
        if (prop->name == NULL
            || strlen(prop->name) == 0
            || prop->group == NULL
            || prop->parameters == NULL
            || prop->values == NULL) {
            return INV_PROP;
        }
        // The property index in the database
        int propNum = -1;
        for (int i = 0; i < NUM_PROPERTIES; i++) {
            if (strcmp(prop->name, propRequirements[i].name) == 0) {
                propNum = i;
            }
        }
        // Invalid property name
        if (propNum == -1) {
            return INV_PROP;
        }
        // Structure properties in optional properties
        if (propNum == 0 || propNum == 1 || propNum == 33) {
            return INV_CARD;
        }
        // BDAY or ANNIVERSARY in optional properties
        if (propNum == 9 || propNum == 10) {
            return INV_DT;
        }
        int length = getLength(prop->values);
        // Invalid number of values in property
        if (length < propRequirements[propNum].minNumValues
            || (propRequirements[propNum].maxNumValues != -1
                && length > propRequirements[propNum].maxNumValues)) {
            return INV_PROP;
        }
        // Iterator for parameters
        ListIterator paramIterator = createIterator(prop->parameters);
        Parameter *param;
        // Check all parameter requirements
        while ((param = (Parameter *)nextElement(&paramIterator)) != NULL) {
            if (param->name == NULL
                || strlen(param->name) == 0
                || param->value == NULL
                || strlen(param->value) == 0) {
                return INV_PROP;
            }
        }
        // Add to the property count
        propCount[propNum] += 1;
    }
    // Verify the cardinality of each property
    for (int i = 0; i < NUM_PROPERTIES; i++) {
        switch (propRequirements[i].cardinality) {
            case NONE:
                if (propCount[i] != 0) {
                    return INV_PROP;
                }
                break;
            case ONE_MUST:
                if (propCount[i] != 1) {
                    return INV_PROP;
                }
                break;
            case ONE_MAY:
                if (propCount[i] > 1) {
                    return INV_PROP;
                }
                break;
            case ONE_OR_MORE_MUST:
                if (propCount[i] == 0) {
                    return INV_PROP;
                }
                break;
            case ONE_OR_MORE_MAY:
                // No requirement
                break;
        }
    }
    // Verify dates
    if ((obj->birthday != NULL && validateDate(obj->birthday) != OK)
        || (obj->anniversary != NULL && validateDate(obj->anniversary) != OK)) {
        return INV_DT;
    }
    return OK;
}