#ifndef VECTOR_MATRIX_H
#define VECTOR_MATRIX_H

#ifndef BOOLEAN
#define BOOLEAN

#define TRUE  1
#define FALSE 0
typedef char BOOL;

#endif // BOOLEAN

#define PI 3.1415926535

typedef struct {
    double* body;
    int dim;
} Vector;

typedef struct {
    double** body;
    int rowNum, colNum;
} Matrix;


/* Vector functions */

// Fundamental functions
void initVector(Vector *vect);
void disposeVector(Vector vect);
void printVector(const Vector vect);

// Basic operators
double getNorm(const Vector vect);
BOOL containedAngle(const Vector vectA, const Vector vectB, double *angle);
BOOL addVector(const Vector vectA, const Vector vectB, Vector *result);
BOOL subVector(const Vector vectA, const Vector vectB, Vector *result);
void mulScalarVector(const Vector vect, const double factor, Vector *result);
BOOL dotProduct(const Vector vectA, const Vector vectB, double *result);
BOOL crossProduct(const Vector vectA, const Vector vectB, Vector *result);
BOOL copyVector(const Vector vect, Vector *result);


/* Matrix functions */

// Fundamental functions
void initMatrix(Matrix *mat);
void disposeMatrix(Matrix mat);
void printMatrix(const Matrix mat);

// Basic operators
BOOL determinantMatrix(const Matrix mat, double *result);
void transposeMatrix(const Matrix mat, Matrix *result);
BOOL inverseMatrix(const Matrix mat, Matrix *result);
BOOL addMatrix(const Matrix matA, const Matrix matB, Matrix *result);
BOOL subMatrix(const Matrix matA, const Matrix matB, Matrix *result);
void mulScalarMatrix(const Matrix mat, const double factor, Matrix *result);
BOOL multiplyMatrix(const Matrix matA, const Matrix matB, Matrix *result);
BOOL copyMatrix(const Matrix mat, Matrix *result);

// Gauss elimination related functions
void REF(const Matrix mat, Matrix *result);
void RREF(const Matrix mat, Matrix *result);

// Elementary row operators
/*
1. ERO_kEn  -> Row multiplication (Rn *= k)
2. ERO_Enm  -> Row switching (Rn <-> Rm)
3. ERO_kEnm -> Row addition (Rn += k * Rm)
*/
BOOL ERO_kEn(Matrix *mat, const double k, const int n);
BOOL ERO_Enm(Matrix *mat, const int n, const int m);
BOOL ERO_kEnm(Matrix *mat, const double k, const int n, const int m);

#endif // VECTOR_MATRIX_H
