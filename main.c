/**
 *  Copyright (C) 2018 by Sungwook Kim, All rights reserved.
 *
 *  Project: [Example project 5] Linear Algebra
 *
 *  Written on October 29, 2018
 *  Modification history:
 *      1. Written by Sung Wook Kim on November 2, 2018
 *
 *  Compiler used: GCC version 7.2.0, 32bit
 */

#include <stdio.h>
#include <stdlib.h>

#include "VectorMatrix.h"
#include "UnitHandler.h"

int main() {
    Handler handler; initHandler(&handler);

    do {
        printf(">>> ");
    } while (activateCommand(readCommand(), &handler));

    eraseHandler(&handler);

    return 0;
}
