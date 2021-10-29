#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "VectorMatrix.h"

/* Vector functions */

void initVector(Vector *vect) {
    printf("Enter the dimension: "); scanf("%d", &vect->dim);
    vect->body = (double*)malloc(vect->dim * sizeof(double));

    printf("Enter %d numbers for the vector: ", vect->dim);
    for (int index = 0; index < vect->dim; index++) {
        scanf("%lf", &vect->body[index]);
    }

    char emp = getchar();  // making stdin empty
}

void disposeVector(Vector vect) {
    free(vect.body);
    vect.dim = 0;
}

void printVector(const Vector vect) {
    for (int index = 0; index < vect.dim; index++) {
        if (index == 0) printf("(%.2lf, ", vect.body[index]);
        else if (index < vect.dim - 1) printf("%.2lf, ", vect.body[index]);
        else printf("%.2lf)\n", vect.body[index]);
    }
}

double getNorm(const Vector vect) {
    double result = 0;
    for (int index = 0; index < vect.dim; index++) {
        result += pow(vect.body[index], 2);
    }
    result = pow(result, 0.5);
    return result;
}

BOOL containedAngle(const Vector vectA, const Vector vectB, double *angle) {
    double dot;
    if (!dotProduct(vectA, vectB, &dot)) return FALSE;

    // return angle with degree (not radian)
    *angle = acos(dot / (getNorm(vectA) * getNorm(vectB))) * 180 / PI;

    return TRUE;
}

BOOL addVector(const Vector vectA, const Vector vectB, Vector *result) {
    if (vectA.dim != vectB.dim) return FALSE;

    copyVector(vectA, result);

    for (int index = 0; index < vectA.dim; index++) {
        result->body[index] += vectB.body[index];
    }

    return TRUE;
}

BOOL subVector(const Vector vectA, const Vector vectB, Vector *result) {
    if (vectA.dim != vectB.dim) return FALSE;

    copyVector(vectA, result);

    for (int index = 0; index < vectA.dim; index++) {
        result->body[index] -= vectB.body[index];
    }

    return TRUE;
}

void mulScalarVector(const Vector vect, const double factor, Vector *result) {
    copyVector(vect, result);

    for (int index = 0; index < vect.dim; index++) {
        result->body[index] *= factor;
    }
}

BOOL dotProduct(const Vector vectA, const Vector vectB, double *result) {
    if (vectA.dim != vectB.dim) return FALSE;
    else {
        *result = 0;
        for (int index = 0; index < vectA.dim; index++) {
            *result += vectA.body[index] * vectB.body[index];
        }
        return TRUE;
    }
}

BOOL crossProduct(const Vector vectA, const Vector vectB, Vector *result) {
    if (vectA.dim == 3 && vectB.dim == 3) {
        result->dim = vectA.dim;
        result->body = (double*)malloc(result->dim * sizeof(double));
        int a, b;

        for (int index = 0; index < vectA.dim; index++) {
            a = index + 1; b = index + 2;
            if (a >= vectA.dim) a -= vectA.dim;
            if (b >= vectA.dim) b -= vectA.dim;
            result->body[index] = vectA.body[a] * vectB.body[b]
                                - vectA.body[b] * vectB.body[a];
        }
        return TRUE;
    } else {
        result = NULL;
        return FALSE;
    }
}

BOOL copyVector(const Vector vect, Vector *result) {
    if (&vect == result) return FALSE;

    result->dim = vect.dim;
    result->body = (double *)malloc(result->dim * sizeof(double));
    for (int index = 0; index < result->dim; index++) {
        result->body[index] = vect.body[index];
    }

    return TRUE;
}


/* Matrix functions */

void initMatrix(Matrix *mat) {
    printf("Enter the size of row: ");   scanf("%d", &mat->rowNum);
    printf("Enter the size of column: ");  scanf("%d", &mat->colNum);

    mat->body = (double **)malloc(mat->colNum * sizeof(double *));
    for (int colIndex = 0; colIndex < mat->colNum; colIndex++) {
        mat->body[colIndex] = (double*)malloc(mat->rowNum * sizeof(double));

        printf("Enter %d numbers for %dth row: ", mat->rowNum, colIndex + 1);
        for (int rowIndex = 0; rowIndex < mat->rowNum; rowIndex++) {
            scanf("%lf", &mat->body[colIndex][rowIndex]);
        }
    }

    char emp = getchar();  // making stdin empty
}

void disposeMatrix(Matrix mat) {
    for (int index = 0; index < mat.colNum; index++) {free(mat.body[index]);}
    free(mat.body);
}

void printMatrix(const Matrix mat) {
    for (int colIndex = 0; colIndex < mat.colNum; colIndex++) {
        if (colIndex == 0) {
            printf("%c", 0x01);
            for (int count = 0; count < mat.rowNum; count++) {
                printf("         ");
            }
            printf("%c\n", 0x02);
        }

        for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
            if (rowIndex == 0) {
                printf("%c%7.2lf, ", 0x05, mat.body[colIndex][rowIndex]);
            } else if (rowIndex == mat.rowNum - 1) {
                printf("%7.2lf  %c\n", mat.body[colIndex][rowIndex], 0x05);
            } else {
                printf("%7.2lf, ", mat.body[colIndex][rowIndex]);
            }
        }

         if (colIndex == mat.colNum - 1) {
            printf("%c", 0x03);
            for (int count = 0; count < mat.rowNum; count++) {
                printf("         ");
            }
            printf("%c\n", 0x04);
        }
    }
}

BOOL determinantMatrix(const Matrix mat, double *result) {
    if (mat.rowNum != mat.colNum) {result = NULL;            return FALSE;}
    else if (mat.rowNum == 0)     {*result = 1;              return TRUE;}
    else if (mat.rowNum == 1)     {*result = mat.body[0][0]; return TRUE;}
    else if (mat.rowNum == 2)     {
        *result = mat.body[0][0] * mat.body[1][1]
                - mat.body[0][1] * mat.body[1][0];
        return TRUE;
    } else {
        // Declaration of tmp matrix
        Matrix tmp;
        tmp.colNum = mat.colNum - 1;
        tmp.rowNum = mat.rowNum - 1;
        tmp.body = (double**)malloc(tmp.colNum * sizeof(double*));

        for (int colIndex = 0; colIndex < tmp.colNum; colIndex++) {
            tmp.body[colIndex] = (double*)malloc(tmp.rowNum * sizeof(double));
        }

        // Initializing tmp matrix and calculating determinant recursively
        *result = 0;
        int tmpRowIndex, tmpColIndex;
        double minor;

        for (int skip = 0; skip < mat.rowNum; skip++) {
            tmpColIndex = 0;
            for (int colIndex = 1; colIndex < mat.colNum; colIndex++) {
                tmpRowIndex = 0;
                for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
                    if (rowIndex == skip) continue;
                    else {
                        tmp.body[tmpColIndex][tmpRowIndex]
                            = mat.body[colIndex][rowIndex];
                        tmpRowIndex++;
                    }
                }
                tmpColIndex++;
            }
            determinantMatrix(tmp, &minor);
            *result += pow(-1, skip) * mat.body[0][skip] * minor;
        }

        // Disposing tmp matrix
        disposeMatrix(tmp);

        return TRUE;
    }
}

void transposeMatrix(const Matrix mat, Matrix *result) {
    result->colNum = mat.rowNum;
    result->rowNum = mat.colNum;

    result->body = (double **)malloc(mat.rowNum * sizeof(double *));
    for (int colIndex = 0; colIndex < mat.rowNum; colIndex++) {
        result->body[colIndex] = (double *)malloc(mat.colNum * sizeof(double));
        for (int rowIndex = 0; rowIndex < mat.colNum; rowIndex++) {
            result->body[colIndex][rowIndex] = mat.body[rowIndex][colIndex];
        }
    }
}

BOOL inverseMatrix(const Matrix mat, Matrix *result) {
    if (mat.colNum != mat.rowNum) {
        fprintf(stderr, "[ERROR] Cannot calculate inverse matrix\n");
        if (&mat != result) result = NULL;
        return FALSE;
    }

    double det; determinantMatrix(mat, &det);

    if (det == 0) {
        fprintf(stderr, "[ERROR] Inverse matrix does not exist\n");
        if (&mat != result) result = NULL;
        return FALSE;
    }

    // Declaration of result
    result->colNum = mat.rowNum; result->rowNum = mat.colNum;
    result->body = (double **)malloc(result->colNum * sizeof(double *));
    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        result->body[colIndex]
            = (double *)malloc(result->rowNum * sizeof(double));
    }

    // Declaration of tmp matrix
    // tmp is transpose of cofactor matrix of mat
    Matrix tmp; tmp.colNum = mat.colNum - 1; tmp.rowNum = mat.rowNum - 1;
    tmp.body = (double **)malloc(tmp.colNum * sizeof(double));
    for (int colIndex = 0; colIndex < tmp.colNum; colIndex++) {
        tmp.body[colIndex] = (double *)malloc(tmp.rowNum * sizeof(double));
    }


    int tmpColIndex, tmpRowIndex;

    for (int colSkip = 0; colSkip < mat.colNum; colSkip++) {
        for (int rowSkip = 0; rowSkip < mat.rowNum; rowSkip++) {
            // Initialization of tmp matrix
            tmpColIndex = 0;
            for (int colIndex = 0; colIndex < mat.colNum; colIndex++) {
                tmpRowIndex = 0;
                for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
                    if (colIndex == colSkip || rowIndex == rowSkip) {
                        continue;
                    } else {
                        tmp.body[tmpColIndex][tmpRowIndex]
                            = mat.body[colIndex][rowIndex];
                        tmpRowIndex++;
                    }
                }
                if (colIndex != colSkip) tmpColIndex++;
            }

            // Initialization of result
            determinantMatrix(tmp, &result->body[rowSkip][colSkip]);
            result->body[rowSkip][colSkip] *= pow(-1, colSkip + rowSkip) / det;
        }
    }

    // Disposing memory of tmp
    for (int colIndex = 0; colIndex < tmp.colNum; colIndex++) {
        free(tmp.body[colIndex]);
    }
    free(tmp.body);

    return TRUE;
}

BOOL addMatrix(const Matrix matA, const Matrix matB, Matrix *result) {
    if (matA.colNum != matB.colNum || matA.rowNum != matB.rowNum) {
        if (&matA != result && &matB != result) result = NULL;
        return FALSE;
    }

    copyMatrix(matA, result);

    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            result->body[colIndex][rowIndex] += matB.body[colIndex][rowIndex];
        }
    }

    return TRUE;
}

BOOL subMatrix(const Matrix matA, const Matrix matB, Matrix *result) {
    if (matA.colNum != matB.colNum || matA.rowNum != matB.rowNum) {
        if (&matA != result && &matB != result) result = NULL;
        return FALSE;
    }

    copyMatrix(matA, result);

    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            result->body[colIndex][rowIndex] -= matB.body[colIndex][rowIndex];
        }
    }

    return TRUE;
}

void mulScalarMatrix(const Matrix mat, const double factor, Matrix *result) {
    copyMatrix(mat, result);

    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            result->body[colIndex][rowIndex] *= factor;
        }
    }
}

BOOL multiplyMatrix(const Matrix matA, const Matrix matB, Matrix *result) {
    if (matA.rowNum != matB.colNum) {
        if (&matA != result && &matB != result) result = NULL;
        return FALSE;
    }

    result->colNum = matA.colNum; result->rowNum = matB.rowNum;
    result->body = (double **)malloc(result->colNum * sizeof(double));
    for (int index = 0; index < result->colNum; index++) {
        result->body[index] = (double *)malloc(result->colNum * sizeof(double));
    }

    double tmp;

    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            tmp = 0;
            for (int index = 0; index < matA.rowNum; index++) {
                tmp += matA.body[colIndex][index] * matB.body[index][rowIndex];
            }
            result->body[colIndex][rowIndex] = tmp;
        }
    }

    return TRUE;
}

BOOL copyMatrix(const Matrix mat, Matrix *result) {
    if (&mat == result) return FALSE;

    // Initializing result matrix
    result->colNum = mat.colNum; result->rowNum = mat.rowNum;
    result->body = (double **)malloc(result->colNum * sizeof(double));
    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        result->body[colIndex]
            = (double *)malloc(result->rowNum * sizeof(double));
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            result->body[colIndex][rowIndex] = mat.body[colIndex][rowIndex];
        }
    }

    return TRUE;
}

void REF(const Matrix mat, Matrix *result) {
    copyMatrix(mat, result);

    for (int stPoint = 0; stPoint < mat.colNum; stPoint++)
    {
        // Array for pivot position (initialized as 0)
        int *pivot = (int *)calloc(mat.colNum, sizeof(int));

        // Filling pivot array
        for (int colIndex = stPoint; colIndex < mat.colNum; colIndex++) {
            for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
                if (result->body[colIndex][rowIndex] == 0) pivot[colIndex]++;
                else break;
            }
        }

        // Aligning rows with pivot position
        int tmp;
        for (int count = stPoint; count < mat.colNum; count++) {
            for (int colIndex = stPoint+1; colIndex < mat.colNum; colIndex++) {
                if (pivot[colIndex] < pivot[colIndex - 1]) {
                    tmp = pivot[colIndex];
                    pivot[colIndex - 1] = pivot[colIndex];
                    pivot[colIndex] = tmp;
                    ERO_Enm(result, colIndex, colIndex - 1);
                }
            }
        }

        // Making row echelon form
        ERO_kEn(result, 1 / result->body[stPoint][pivot[stPoint]], stPoint);
        for (int colIndex = stPoint + 1; colIndex < mat.colNum; colIndex++) {
            ERO_kEnm(result, -1 * result->body[colIndex][pivot[colIndex]],
                     colIndex, stPoint);
        }

        // Disposing pivot array
        free(pivot);
    }

    // Solution for negative 0 glitch
    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            if (result->body[colIndex][rowIndex] == 0) {
                result->body[colIndex][rowIndex] = 0;
            }
        }
    }
}

void RREF(const Matrix mat, Matrix *result) {
    // Starts from row echelon form
    REF(mat, result);

    // Array for pivot position (initialized as 0)
    int *pivot = (int *)calloc(mat.colNum, sizeof(int));

    // Filling pivot array
    for (int colIndex = 0; colIndex < mat.colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
            if (result->body[colIndex][rowIndex] == 0) pivot[colIndex]++;
            else break;
        }
    }

    // Reduce rows with elementary row operators
    for (int endPoint = mat.colNum - 1; endPoint > 0; endPoint--) {
        for (int colIndex = 0; colIndex < endPoint; colIndex++) {
            ERO_kEnm(result, -1 * result->body[colIndex][pivot[endPoint]],
                     colIndex, endPoint);
        }
    }

    // Filling pivot array again
    for (int colIndex = 0; colIndex < mat.colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < mat.rowNum; rowIndex++) {
            if (result->body[colIndex][rowIndex] == 0) pivot[colIndex]++;
            else break;
        }
    }

    // Aligning rows with the new pivot position
    int tmp;
    for (int count = 0; count < mat.colNum; count++) {
        for (int colIndex = 1; colIndex < mat.colNum; colIndex++) {
            if (pivot[colIndex] < pivot[colIndex - 1]) {
                tmp = pivot[colIndex];
                pivot[colIndex - 1] = pivot[colIndex];
                pivot[colIndex] = tmp;
                ERO_Enm(result, colIndex, colIndex - 1);
            }
        }
    }

    // Disposing pivot array
    free(pivot);

    // Solution for negative 0 glitch
    for (int colIndex = 0; colIndex < result->colNum; colIndex++) {
        for (int rowIndex = 0; rowIndex < result->rowNum; rowIndex++) {
            if (result->body[colIndex][rowIndex] == 0) {
                result->body[colIndex][rowIndex] = 0;
            }
        }
    }
}

BOOL ERO_kEn(Matrix *mat, const double k, const int n) {
    if (n > mat->colNum) {
        return FALSE;
    }

    for (int rowIndex = 0; rowIndex < mat->rowNum; rowIndex++) {
        mat->body[n][rowIndex] *= k;
    }

    return TRUE;
}

BOOL ERO_Enm(Matrix *mat, const int n, const int m) {
    if (n > mat->colNum || m > mat->colNum) {
        return FALSE;
    }

    double tmp;
    for (int rowIndex = 0; rowIndex < mat->rowNum; rowIndex++) {
        tmp = mat->body[n][rowIndex];
        mat->body[n][rowIndex] = mat->body[m][rowIndex];
        mat->body[m][rowIndex] = tmp;
    }

    return TRUE;
}

BOOL ERO_kEnm(Matrix *mat, const double k, const int n, const int m) {
    if (n > mat->colNum || m > mat->colNum) {
        return FALSE;
    }

    for (int rowIndex = 0; rowIndex < mat->rowNum; rowIndex++) {
        mat->body[n][rowIndex] += k * mat->body[m][rowIndex];
    }

    return TRUE;
}
