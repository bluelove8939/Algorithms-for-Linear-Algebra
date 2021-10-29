#ifndef UNIT_HANDLER_H
#define UNIT_HANDLER_H

#include "VectorMatrix.h"

#define LINE_BUFFER_SIZE      1024
#define STACK_MAX_SIZE        30
#define PARSE_DELIMITER       " \t"

// Unit related constants
typedef enum{SCALAR, VECTOR, MATRIX} UNIT_TYPE;

typedef union {
    double scalarData;
    Vector vectData;
    Matrix matData;
} Data;

typedef struct nodelink {
    UNIT_TYPE type;
    char *name;
    struct nodelink *link;
    Data data;
} Unit;

// Handler
typedef struct {
    Unit *head, *tail;
    int unitNum;
} Handler;

// Command line interface
typedef char* Command;
typedef char* Token;

// Calculator interpreter
typedef enum{FUNC, VAR, OP} KEY_TYPE;

typedef struct {
    KEY_TYPE type;
    char *body;
} KeyUnit;


// Unit related functions
Unit *makeUnit(const char *input_name, const UNIT_TYPE input_type);
void redefineUnit(Unit *unit);
void removeUnit(Unit *unit);
BOOL printUnit(Unit unit);


// Handler related functions
void initHandler(Handler *handler);
void appendNode(Handler *handler, Unit *unit);
void eraseNode(Handler *handler, const int index);
void eraseHandler(Handler *handler);
BOOL printNode(const Handler handler, const int index);
void printList(const Handler handler);
Unit *accessIndex(Handler handler, const int index);
Unit *accessName(Handler handler, const char *name);


// Keyword functions
BOOL isName(const Handler handler, const char *str);


// Command line interface related functions
Command readCommand();
BOOL activateCommand(Command com, Handler *handler);

void lowerToken(Token token);

#endif // UNIT_HANDLER_H
