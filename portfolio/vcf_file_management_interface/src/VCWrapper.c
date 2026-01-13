/*
 * Name: William Rush
 * ID: 1267781
 */

#include "VCParser.h"
#include "VCWrapper.h"

// Get a newline seperated string of all valid vcards in a ceratin folder
char *getCardList() {
    DIR *cardDir = opendir("cards");
    if (cardDir == NULL) {
        printf("Error opening card folder\n");
        exit(0);
    }
    char *cardList = (char *)malloc(sizeof(char) * 1000);
    cardList[0] = '\0';
    struct dirent *file;
    while ((file = readdir(cardDir)) != NULL) {
        if (file->d_name[0] == '.') {
            continue;
        }
        Card *card;
        VCardErrorCode error;
        char path[100] = "cards/";
        strcat(path, file->d_name);
        error = createCard(path, &card);
        if (error != OK) {
            continue;
        }
        error = validateCard(card);
        if (error != OK) {
            continue;
        }
        strcat(cardList, file->d_name);
        strcat(cardList, "\n");
    }
    closedir(cardDir);
    return cardList;
}

// Get a newline seperated string of important values for SQL
char *getContactInfo(char *filename) {
    Card *card;
    char path[100] = "cards/";
    strcat(path, filename);
    createCard(path , &card);
    char *contactInfo = (char *)malloc(sizeof(char) * 500);
    contactInfo[0] = '\0';

    strcat(contactInfo, getFromFront(card->fn->values));
    strcat(contactInfo, "\n");

    char *date = formatDateSQL(card->birthday);
    if (date != NULL) {
        strcat(contactInfo, date);
        free(date);
    }
    strcat(contactInfo, "\n");
    
    date = formatDateSQL(card->anniversary);
    if (date != NULL) {
        strcat(contactInfo, date);
        free(date);
    }
    strcat(contactInfo, "\n");
    return contactInfo;
}
    
// Format a date so that it can be entered into SQL
char *formatDateSQL(DateTime *date) {
    if (date == NULL
        || date->isText
        || date->date == NULL
        || date->time == NULL) {
        return NULL;
    }
    // 2025-03-24 14:30:00
    char *formattedDate = (char *)malloc(sizeof(char) * 20);
    formattedDate[0] = date->date[0];
    formattedDate[1] = date->date[1];
    formattedDate[2] = date->date[2];
    formattedDate[3] = date->date[3];
    formattedDate[4] = '-';
    formattedDate[5] = date->date[4];
    formattedDate[6] = date->date[5];
    formattedDate[7] = '-';
    formattedDate[8] = date->date[6];
    formattedDate[9] = date->date[7];
    formattedDate[10] = ' ';
    formattedDate[11] = date->time[0];
    formattedDate[12] = date->time[1];
    formattedDate[13] = ':';
    formattedDate[14] = date->time[2];
    formattedDate[15] = date->time[3];
    formattedDate[16] = ':';
    formattedDate[17] = date->time[4];
    formattedDate[18] = date->time[5];
    formattedDate[19] = '\0';
    return formattedDate;
}

// Format a date for output
char *formatDate(DateTime *date) {
    if (date->isText) {
        return date->text;
    }
    int length = 1;
    if (strlen(date->date) != 0) {
        // Date:_{}_
        length += 7 + strlen(date->date);
    }
    if (strlen(date->time) != 0) {
        //Time:_{}_
        length += 7 + strlen(date->time);
    }
    if (date->UTC) {
        // (UTC)
        length += 5;
    }
    char *output = (char *)malloc(sizeof(char) * length);
    output[0] = '\0';
    if (strlen(date->date) != 0) {
        strcat(output, "Date: ");
        strcat(output, date->date);
        strcat(output, " ");
    }
    if (strlen(date->time) != 0) {
        strcat(output, "Time: ");
        strcat(output, date->time);
        strcat(output, " ");
    }
    if (date->UTC) {
        strcat(output, "(UTC)");
    }
    return output;
}

// Get output formatted dates for a specific file
char *getFormattedDates(char *filename) {
    Card *card;
    char path[100] = "cards/";
    strcat(path, filename);
    createCard(path , &card);
    char *birthday;
    if (card->birthday != NULL) {
        birthday = formatDate(card->birthday);
    } else {
        birthday = (char *)malloc(sizeof(char));
        birthday[0] = '\0';
    }
    char *anniversary;
    if (card->anniversary != NULL) {
        anniversary = formatDate(card->anniversary);
    } else {
        anniversary = (char *)malloc(sizeof(char));
        anniversary[0] = '\0';
    }
    //{}\n{}\n\0
    char *dates = (char *)malloc(sizeof(char) * (strlen(birthday) + strlen(anniversary) + 3));
    dates[0] = '\0';
    strcat(dates, birthday);
    strcat(dates, "\n");
    strcat(dates, anniversary);
    strcat(dates, "\n");
    return dates;
}

// Write a new card with just a fn values
void writeNewCard(char *filename, char *fn) {
    char path[100] = "cards/";
    strcat(path, filename);
    Card *card = (Card *)malloc(sizeof(Card));
    card->fn = (Property *)malloc(sizeof(Property));
    card->fn->name = (char *)malloc(sizeof(char) * 3);
    strcpy(card->fn->name, "FN");
    card->fn->group = (char *)malloc(sizeof(char));
    card->fn->group[0] = '\0';
    card->fn->parameters = initializeList(&parameterToString, &deleteParameter, &compareParameters);
    card->fn->values = initializeList(&valueToString, &deleteValue, &compareValues);
    char *value = (char *)malloc(sizeof(char) * (strlen(fn) + 1));
    strcpy(value, fn);
    insertFront(card->fn->values, (void *)value);
    card->optionalProperties = initializeList(&propertyToString, &deleteProperty, &compareProperties);
    card->birthday = NULL;
    card->anniversary = NULL;
    writeCard(path, card);
}

// Update the fn value in a file
void updateContact(char *filename, char *fn) {
    Card *card;
    char path[100] = "cards/";
    strcat(path, filename);
    createCard(path , &card);
    clearList(card->fn->values);
    char *value = (char *)malloc(sizeof(char) * (strlen(fn) + 1));
    strcpy(value, fn);
    insertFront(card->fn->values, (void *)value);
    writeCard(path, card);
}

// Count the number of properties in a file
int countOptionalProperties(char *filename) {
    Card *card;
    char path[100] = "cards/";
    strcat(path, filename);
    createCard(path, &card);
    int count = 0;
    ListIterator propIterator = createIterator(card->optionalProperties);
    Property *prop;
    while ((prop = (Property *)nextElement(&propIterator)) != NULL) {
        count++;
    }
    return count;
}