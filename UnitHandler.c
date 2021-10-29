#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "UnitHandler.h"


/* Unit related functions */

Unit *makeUnit(const char *input_name, const UNIT_TYPE input_type) {
    Unit *item = (Unit *)malloc(sizeof(Unit));

    // Initialize name
    item->name = (char *)malloc((strlen(input_name) + 1) * sizeof(char));
    strcpy(item->name, input_name);

    // Initialize type
    item->type = input_type;

    // Initialize link
    item->link = NULL;

    // Initialize data
    switch (item->type)
    {
    case SCALAR:
        printf("Enter a number: "); scanf("%lf", &item->data.scalarData);
        break;

    case VECTOR:
        initVector(&item->data.vectData);
        break;

    case MATRIX:
        initMatrix(&item->data.matData);
        break;

    default:
        fprintf(stderr, "[ERROR] Improper type\n");
        free(item->name);
        free(item);
        return NULL;
    }

    return item;
}

void redefineUnit(Unit *unit) {
    switch (unit->type)
    {
    case SCALAR:
        printf("Enter a number: "); scanf("%lf", &unit->data.scalarData);
        break;

    case VECTOR:
        disposeVector(unit->data.vectData);
        initVector(&unit->data.vectData);
        break;

    case MATRIX:
        disposeMatrix(unit->data.matData);
        initMatrix(&unit->data.matData);
        break;

    default:
        fprintf(stderr, "[ERROR] Improper type\n");
    }
}

void removeUnit(Unit *unit) {
    // Dispose name
    free(unit->name);

    // Dispose data
    switch (unit->type)
    {
    case SCALAR:
        break;

    case VECTOR:
        disposeVector(unit->data.vectData);
        break;

    case MATRIX:
        disposeMatrix(unit->data.matData);
        break;

    default:
        fprintf(stderr, "[ERROR] Improper type\n");
    }

    // Dispose unit itself
    free(unit);
}

BOOL printUnit(Unit unit) {
    switch (unit.type)
    {
    case SCALAR:
        printf("%s = %.2lf\n", unit.name, unit.data.scalarData);
        break;

    case VECTOR:
        printf("%s = ", unit.name); printVector(unit.data.vectData);
        break;

    case MATRIX:
        printf("%s = \n", unit.name); printMatrix(unit.data.matData);
        break;

    default:
        fprintf(stderr, "[ERROR] Improper unit\n");
        return FALSE;
    }

    return TRUE;
}


/* Handler related functions */

void initHandler(Handler *handler) {
    handler->head = NULL;
    handler->tail = NULL;
    handler->unitNum = 0;
}

void appendNode(Handler *handler, Unit *unit) {
    if (isName(*handler, unit->name)) {
        fprintf(stderr, "[ERROR] %s is already defined\n", unit->name);
        free(unit);
        return;
    }

    if (handler->unitNum == 0) {
        handler->head = unit;
        handler->tail = unit;
        handler->unitNum++;
    } else {
        handler->tail->link = unit;
        handler->tail = unit;
        handler->unitNum++;
    }
}

void eraseNode(Handler *handler, const int  index) {
    if (handler->unitNum <= 0) {
        fprintf(stderr, "[ERROR] List is empty or not valid\n");
        return;
    }

    if (index > handler->unitNum - 1) {
        fprintf(stderr, "[ERROR] Index is out of range\n");
        return;
    }

    Unit *tmp = handler->head;

    if (handler->unitNum == 1) {
        removeUnit(tmp);
        handler->head = NULL;
        handler->tail = NULL;
    } else if (index == 0) {
        handler->head = tmp->link;
        removeUnit(tmp);
    } else {
        for (int count = 0; count < index - 1; count++) {
            tmp = tmp->link;
        }

        Unit *item = tmp->link;
        tmp->link = item->link;

        if (tmp->link == NULL) {
            handler->tail = tmp;
        }
    }

    handler->unitNum--;
}

void eraseHandler(Handler *handler) {
    int repeat = handler->unitNum;
    for (int count = 0; count < repeat; count++) {
        eraseNode(handler, 0);
    }
}

BOOL printNode(const Handler handler, const int index) {
    if (handler.unitNum == 0) {
        fprintf(stderr, "[ERROR] List is empty or not valid\n");
        return FALSE;
    }

    if (index > handler.unitNum - 1) {
        fprintf(stderr, "[ERROR] Index is out of range\n");
        return FALSE;
    }

    Unit *tmp = handler.head;
    for (int count = 0; count < index; count++) {
        tmp = tmp->link;
    }

    switch (tmp->type)
    {
    case SCALAR:
        printf("%s = %.2lf\n", tmp->name, tmp->data.scalarData);
        break;

    case VECTOR:
        printf("%s = ", tmp->name); printVector(tmp->data.vectData);
        break;

    case MATRIX:
        printf("%s = \n", tmp->name);
        printMatrix(tmp->data.matData);
        break;

    default:
        fprintf(stderr, "[ERROR] Improper unit\n");
        return FALSE;
    }

    return TRUE;
}

void printList(const Handler handler) {
    if (handler.unitNum == 0) {
        fprintf(stderr, "[ERROR] List is empty or not valid\n");
        return;
    }

    Unit *tmp = handler.head;
    for (int index = 0; index < handler.unitNum; index++) {
        printf("%d. %s\n", index, tmp->name);
        tmp = tmp->link;
    }
}

Unit *accessIndex(Handler handler, const int index) {
    Unit *tmp = handler.head;
    for (int count = 0; count < index; count++) {
        tmp = tmp->link;
    }
    return tmp;
}

Unit *accessName(Handler handler, const char *name) {
    for (Unit *tmp = handler.head; tmp; tmp = tmp->link) {
        if (!strcmp(tmp->name, name)) return tmp;
    }
    return NULL;
}


/* Keyword functions */

BOOL isName(const Handler handler, const char *str) {
    for (Unit *tmp = handler.head; tmp; tmp = tmp->link) {
        if (!strcmp(tmp->name, str)) {
            return TRUE;
        }
    }
    return FALSE;
}


/* Command Line interface related functions */
Command readCommand() {
    // Initialize buffer
    const int bufsize = LINE_BUFFER_SIZE;  // 1024
    char *buffer = (char *)malloc(bufsize * sizeof(char));

    // Read a line from stdin
    int index = 0;
    char tmp;

    while (TRUE) {
        scanf("%c", &tmp);
        if (tmp == EOF || tmp == '\n') {
            if (index != 0) break;
            else continue;
        } else {
            buffer[index] = tmp;
            index++;
        }
    }

    buffer[index] = '\0';

    // Copy buffer to command
    Command com = (Command)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(com, buffer);

    // Dispose buffer
    free(buffer);

    return com;
}

BOOL activateCommand(Command com, Handler *handler) {
    Token save, token = strtok_r(com, PARSE_DELIMITER, &save);
    lowerToken(token);

    /** [1] Initial keyword is define **/

    if (!strcmp(token, "define"))
    {
        token = strtok_r(NULL, PARSE_DELIMITER, &save);
        lowerToken(token);

        UNIT_TYPE type;
        if (!strcmp(token, "scalar")) type = SCALAR;
        else if (!strcmp(token, "vector")) type = VECTOR;
        else if (!strcmp(token, "matrix")) type = MATRIX;
        else {
            fprintf(stderr, "[ERROR] Invalid syntax (improper type)\n");
            free(com);
            return TRUE;
        }

        token = strtok_r(NULL, PARSE_DELIMITER, &save);

        if (strlen(token) > 15) {
            fprintf(stderr, "[ERROR] Name is too long (under 15)\n");
            free(com);
            return TRUE;
        }

        char *name = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(name, token);

        if (isName(*handler, name)) {
            fprintf(stderr, "[ERROR] %s is already defined\n", name);
            free(name); free(com);
            return TRUE;
        }

        token = strtok_r(NULL, PARSE_DELIMITER, &save);

        if (token) {
            fprintf(stderr, "[ERROR] Invalid syntax (unnecessary tokens)\n");
            free(com);
            return TRUE;
        }

        appendNode(handler, makeUnit(name, type));
    }

    /** [2] Initial keyword is show **/

    else if (!strcmp(token, "show"))
    {
        token = strtok_r(NULL, PARSE_DELIMITER, &save);
        lowerToken(token);

        if (!strcmp(token, "list")) {
            token = strtok_r(NULL, PARSE_DELIMITER, &save);

            if (token) {
                fprintf(stderr,
                        "{ERROR} Invalid syntax (unnecessary tokens)\n");
                free(com);
                return TRUE;
            } else {
                printList(*handler);
            }
        } else if (!strcmp(token, "index")) {
            token = strtok_r(NULL, PARSE_DELIMITER, &save);

            for (int pos = 0; pos < strlen(token); pos++) {
                if (!isdigit(token[pos])) {
                    fprintf(stderr, "[ERROR] Invalid syntax (not a number)\n");
                    free(com);
                    return TRUE;
                }
            }

            printNode(*handler, atoi(token));
        } else if (!strcmp(token, "name")){
            token = strtok_r(NULL, PARSE_DELIMITER, &save);

            Unit *tmp = accessName(*handler, token);

            if (!tmp) {
                fprintf(stderr, "[ERROR] Name %s cannot be found\n", token);
                free(com);
                return TRUE;
            } else {
                printUnit(*tmp);
            }
        } else {
            fprintf(stderr, "[ERROR] Invalid syntax (improper keyword %s)\n",
                    token);
            free(com);
            return TRUE;
        }
    }

    /** [3] Initial keyword is calculate  **/

    else if (!strcmp(token, "calculate"))
    {
        char buffer[STACK_MAX_SIZE]; int bufpos = 0;

        if (STACK_MAX_SIZE < strlen(save)) {
            fprintf(stderr, "[ERROR] Too long command line\n");
            free(com);
            return TRUE;
        }

        for (int pos = 0; pos < strlen(save); pos++) {
            if (save[pos] == '(' && bufpos != 0) {
                buffer[++bufpos] = '\0';
            }
        }
    }

    /** [4] Initial keyword is clear **/

    else if (!strcmp(token, "clear"))
    {
        if (save == NULL) {
            system("cls");
            free(com);
            return TRUE;
        }else {
            fprintf(stderr, "[ERROR] Unnecessary keywords exists\n");
            free(com);
            return TRUE;
        }
    }

    /** [5] Initial keyword is exit **/

    else if (!strcmp(token, "exit"))
    {
        if (save == NULL) {
            printf("Program terminated\n");
            free(com);
            return FALSE;
        }else {
            fprintf(stderr, "[ERROR] Unnecessary keywords exists\n");
            free(com);
            return TRUE;
        }
    }

    /** [6] Initial keyword is unknown **/

    else
    {
        fprintf(stderr, "[ERROR] Syntax error (unknown keyword %s)\n", token);
        free(com);
        return TRUE;
    }

    free(com);
    return TRUE;
}

void lowerToken(Token token) {
    for (char *ch = token; *ch; ch++) {
        *ch = (char)tolower(*ch);
    }
}
