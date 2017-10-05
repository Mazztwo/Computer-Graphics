/*
Alessio Mazzone
CS1566 Computer Graphics
University of Pittsburgh
 
Lab Assignment 1 functions
This file will contain all the implementations of the 
functions required of the Lab Assignment: 1 
instruction pdf.
 */


#include "funct.h"
#include <stdio.h>
#include <math.h>


/*
Function:
    This method will derefrence a vec4 pointer
    and print its x,y,z, and w components.
Inputs: 
    vectorToPrint --> Contains a
        pointer to a vec4 struc to 
        print to terminal.
Outputs:
    None
*/
void printVec4(Vec4 *vectorToPrint)
{   
    // Declare vec4 components and 
    // extract from vector
    float xComp, yComp, zComp, wComp;
    
    xComp = vectorToPrint->x;
    yComp = vectorToPrint->y;
    zComp = vectorToPrint->z;
    wComp = vectorToPrint->w;
    
    // Print each component of vector
    printf("[%f,%f,%f,%f]\n", xComp,yComp,zComp,wComp);
}

/*
Function:
    This method will calculate the cross
    product of two vectors, vec1 and vec2 and place
    the result in product productVector. Though Vec4
    are 4 compnents, we are only concerened with
    the cross product of their x, y, and z.
Inputs:
    vec1 --> pointer to first vector for cross product
    vec2 --> pointer tosecond vector for cross product
    productVector --> pointer to eventual product of operation
        vec1 X vec2
Outpus:
    productVector --> return pointer where result is held
*/
Vec4* crossProduct(Vec4 *vec1, Vec4 *vec2, Vec4 *productVector)
{
    /* 
    A = {a1, a2, a3}
    B = {b1, b2, b3}oo

    A X B = {a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1}  
    */
    
    // Calculate x,y,z of product
    float xProd = (vec1->y * vec2->z) - (vec1->z * vec2->y);
    float yProd = (vec1->z * vec2->x) - (vec1->x * vec2->z);
    float zProd = (vec1->x * vec2->y) - (vec1->y * vec2->x);

    // Place results in user-defined productVector
    productVector->x = xProd;
    productVector->y = yProd;
    productVector->z = zProd;

    return productVector;
}

/*
Function:
    Calculate the dot product of vec1 and vec2.
    Though Vec4 are 4 compnents, we are only 
    concerened with the dot product of their x, y, and z.
Inputs:
    vec1 --> pointer to user defined vector
    vec2 --> pointer to user defined vector
Outputs:
    sum --> the scalar result of vec1 dot vec2
*/
float dotProduct(Vec4 *vec1, Vec4 *vec2)
{
    /* 
    A = {a1, a2, a3}
    B = {b1, b2, b3}

    A dot B = a1b1 + a2b2 + a3b3 
    */

    // Calculate each part of dot product, and sum them
    float part1 = vec1->x * vec2->x;
    float part2 = vec1->y * vec2->y;
    float part3 = vec1->z * vec2->z;

    float sum = part1 + part2 + part3;

    return sum;
}

/*
Function:
    This function will multiply a vector by a scalar. 
    Though Vec4 are 4 compnents, we are only concerened 
    with the cross product of their x, y, and z.
Inputs:
    scalar --> scalar to multiply vector by
    vec --> vector to be multiplied by scalar
    productVector --> pointer to place result in
Outputs:
    productVector --> scalar multiplied vector 
*/
Vec4* scalarMultVector(float scalar, Vec4 *vec, Vec4 *productVector)
{
    /*
    A = {a1, a2, a3}
    C = constant

    C*A = {C*a1, C*a2, C*a3}
    */

    // Multiply each component of vector by the scalar
    float xProd = vec->x * scalar;
    float yProd = vec->y * scalar;
    float zProd = vec->z * scalar;

    // Update product vector and return to user
    productVector->x = xProd;
    productVector->y = yProd;
    productVector->z = zProd;

    return productVector;
}

/*
Function:
    This function performs the vector addition of two vectors.
    Though Vec4 are 4 compnents, we are only 
    concerened with the summation of their x, y, and z.    
Inputs:
    vec1,vec2 --> vector pointers to be added
    sumVec --> pointer to place resulting vector
Outputs:
    sumVec --> sum of vec1 and vec2
*/
Vec4* vec4addition(Vec4* vec1, Vec4* vec2, Vec4 *sumVec)
{
    /* 
    A = {a1, a2, a3}
    B = {b1, b2, b3}

    A + B = {a1+b1, a2+b2, a3+b3} 
    */
    
    // Caclulate each component and return vector to user
    float xSum = vec1->x + vec2->x;
    float ySum = vec1->y + vec2->y;
    float zSum = vec1->z + vec2->z;

    sumVec->x = xSum;
    sumVec->y = ySum;
    sumVec->z = zSum;

    return sumVec;
}

/*
Function:
    This function performs the vector subtraction of two vectors.
    Though Vec4 are 4 compnents, we are only 
    concerened with the summation of their x, y, and z.    
Inputs:
    vec1,vec2 --> vector pointers to be added
    sumVec --> ponter to place resulting vector
Outputs:
    sumVec --> subtraction of vec1 and vec2
*/
Vec4* vec4subtraction(Vec4* vec1, Vec4* vec2, Vec4 *subVec)
{
      /* 
    A = {a1, a2, a3}
    B = {b1, b2, b3}

    A + B = {a1-b1, a2-b2, a3-b3} 
    */
    
    // Caclulate each component and return vector to user
    float xSub = vec1->x - vec2->x;
    float ySub = vec1->y - vec2->y;
    float zSub = vec1->z - vec2->z;

    subVec->x = xSub;
    subVec->y = ySub;
    subVec->z = zSub;

    return subVec;
}

/*
 Function:
     This function calculates the angle between two vectors
 Inputs:
     vec1, vec2 --> pointers to vectors find angle between
 Outputs:
     angle --> angle between vec1 and vec2
 */
float angleBetweenVectors(Vec4 *vec1, Vec4 *vec2)
{
    float angle = 0.0;
    float DegreesToRadians = M_PI / 180.0;
    float dotProd = 0.0;
    float magVec1 = 0.0;
    float magVec2 = 0.0;
    
    printf("\n");
    printf("OriginVector: ");
    printVec4(vec1);
    printf("MotionVector: ");
    printVec4(vec2);
    
    // First calculate dot product between vectors
    dotProd = dotProduct(vec1, vec2);
    
    // Next calculate the magnitude of vec1 and vec2
    magVec1 = vecMagnitude(vec1);
    magVec2 = vecMagnitude(vec2);
    
    // Calcuate angle and convert to radians
    angle = acos((dotProd/(magVec1 * magVec2)) * DegreesToRadians) ;
    
    return angle;
}

/*
 Function:
    This function calculates the magnitude of a vector
 Inputs:
     vec -->pointer to vector to calculate magnitude of
 Outputs:
     magnitude --> magnitude of vec
 */
float vecMagnitude(Vec4 *vec)
{
    float magnitude = sqrt((vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z));
    
    return magnitude;
}


/*
Function:
    This function prints a 4x4 matrix in column major fashion.
Inputs:
    mat--> pointer to matrix to print
Outputs:
    None
*/
void printMat4(Mat4 *mat)
{
    /* 
    Matrix:
   | x.x | y.x | z.x | w.x |
   | x.y | y.y | z.y | w.y |
   | x.z | y.z | z.z | w.z |
   | x.w | y.w | z.w | w.w |
    */

    printf("| %f | %f | %f | %f |\n",mat->col1.x,mat->col2.x,mat->col3.x,mat->col4.x);
    printf("| %f | %f | %f | %f |\n",mat->col1.y,mat->col2.y,mat->col3.y,mat->col4.y);
    printf("| %f | %f | %f | %f |\n",mat->col1.z,mat->col2.z,mat->col3.z,mat->col4.z);
    printf("| %f | %f | %f | %f |\n",mat->col1.w,mat->col2.w,mat->col3.w,mat->col4.w);
}

/*
Function:
    Function takes a matrix and multiplies every element
    by a scalar.
Inputs:
    scalar --> scalar to multiply every element in matrix by
    mat --> matrix to be multiplied by scalar
    productMatrix --> pointer to place resulting matrix
Outputs:
    productMatrix --> resulting matrix to be returned to user
*/
Mat4* scalarMultMatrix(float scalar, Mat4 *mat, Mat4 *productMatrix)
{
    productMatrix->col1.x = mat->col1.x * scalar;
    productMatrix->col1.y = mat->col1.y * scalar;
    productMatrix->col1.z = mat->col1.z * scalar;
    productMatrix->col1.w = mat->col1.w * scalar;
    productMatrix->col2.x = mat->col2.x * scalar;
    productMatrix->col2.y = mat->col2.y * scalar;
    productMatrix->col2.z = mat->col2.z * scalar;
    productMatrix->col2.w = mat->col2.w * scalar;
    productMatrix->col3.x = mat->col3.x * scalar;
    productMatrix->col3.y = mat->col3.y * scalar;
    productMatrix->col3.z = mat->col3.z * scalar;
    productMatrix->col3.w = mat->col3.w * scalar;
    productMatrix->col4.x = mat->col4.x * scalar;
    productMatrix->col4.y = mat->col4.y * scalar;
    productMatrix->col4.z = mat->col4.z * scalar;
    productMatrix->col4.w = mat->col4.w * scalar;
   
    return productMatrix;
}

/*
Function:
    This function adds two matricies together.
Inputs:
    mat1, mat2 --> matricies to be added together.
    sumMatrix --> pointer to place resulting matrix
Outputs:
    sumMatrix --> summation of mat1 and mat2
*/
Mat4* mat4addition(Mat4 *mat1, Mat4 *mat2, Mat4 *sumMatrix)
{
    sumMatrix->col1.x = mat1->col1.x + mat2->col1.x;
    sumMatrix->col1.y = mat1->col1.y + mat2->col1.y;
    sumMatrix->col1.z = mat1->col1.z + mat2->col1.z;
    sumMatrix->col1.w = mat1->col1.w + mat2->col1.w;
    sumMatrix->col2.x = mat1->col2.x + mat2->col2.x;
    sumMatrix->col2.y = mat1->col2.y + mat2->col2.y;
    sumMatrix->col2.z = mat1->col2.z + mat2->col2.z;
    sumMatrix->col2.w = mat1->col2.w + mat2->col2.w;
    sumMatrix->col3.x = mat1->col3.x + mat2->col3.x;
    sumMatrix->col3.y = mat1->col3.y + mat2->col3.y;
    sumMatrix->col3.z = mat1->col3.z + mat2->col3.z;
    sumMatrix->col3.w = mat1->col3.w + mat2->col3.w;
    sumMatrix->col4.x = mat1->col4.x + mat2->col4.x;
    sumMatrix->col4.y = mat1->col4.y + mat2->col4.y;
    sumMatrix->col4.z = mat1->col4.z + mat2->col4.z;
    sumMatrix->col4.w = mat1->col4.w + mat2->col4.w;

    return sumMatrix;
}

/*
Function:
    This function subtracts two matricies together.
Inputs:
    mat1, mat2 --> matricies to be subtracted
    sumMatrix --> pointer to place resulting matrix
Outputs:
    sumMatrix --> subtraction of mat1 and mat2
*/
Mat4* mat4subtraction(Mat4 *mat1, Mat4 *mat2, Mat4 *subMatrix)
{
    subMatrix->col1.x = mat1->col1.x - mat2->col1.x;
    subMatrix->col1.y = mat1->col1.y - mat2->col1.y;
    subMatrix->col1.z = mat1->col1.z - mat2->col1.z;
    subMatrix->col1.w = mat1->col1.w - mat2->col1.w;
    subMatrix->col2.x = mat1->col2.x - mat2->col2.x;
    subMatrix->col2.y = mat1->col2.y - mat2->col2.y;
    subMatrix->col2.z = mat1->col2.z - mat2->col2.z;
    subMatrix->col2.w = mat1->col2.w - mat2->col2.w;
    subMatrix->col3.x = mat1->col3.x - mat2->col3.x;
    subMatrix->col3.y = mat1->col3.y - mat2->col3.y;
    subMatrix->col3.z = mat1->col3.z - mat2->col3.z;
    subMatrix->col3.w = mat1->col3.w - mat2->col3.w;
    subMatrix->col4.x = mat1->col4.x - mat2->col4.x;
    subMatrix->col4.y = mat1->col4.y - mat2->col4.y;
    subMatrix->col4.z = mat1->col4.z - mat2->col4.z;
    subMatrix->col4.w = mat1->col4.w - mat2->col4.w;

    return subMatrix;
}

/*
Function:
    This function multiplies two 4x4 matricies together.
Inputs:
    mat1,mat2 --> two matricies to be multiplied together
    productMatrix --> pointer to place resulting matrix
Outputs:
    productMatrix --> product of two matricies
*/
Mat4* matMultiplication(Mat4 *mat1, Mat4 *mat2, Mat4 *productMatrix)
{
    /* 
    Matrix:
   | x.x | y.x | z.x | w.x |     | x.x | y.x | z.x | w.x |
   | x.y | y.y | z.y | w.y |     | x.y | y.y | z.y | w.y |
   | x.z | y.z | z.z | w.z |     | x.z | y.z | z.z | w.z |
   | x.w | y.w | z.w | w.w |     | x.w | y.w | z.w | w.w |
    */
    // row x col
    
    // Row 1
    productMatrix->col1.x = (mat1->col1.x * mat2->col1.x) +
                            (mat1->col2.x * mat2->col1.y) + 
                            (mat1->col3.x * mat2->col1.z) +
                            (mat1->col4.x * mat2->col1.w);

    productMatrix->col2.x = (mat1->col1.x * mat2->col2.x) +
                            (mat1->col2.x * mat2->col2.y) + 
                            (mat1->col3.x * mat2->col2.z) +
                            (mat1->col4.x * mat2->col2.w);

    productMatrix->col3.x = (mat1->col1.x * mat2->col3.x) +
                            (mat1->col2.x * mat2->col3.y) + 
                            (mat1->col3.x * mat2->col3.z) +
                            (mat1->col4.x * mat2->col3.w);

    productMatrix->col4.x = (mat1->col1.x * mat2->col4.x) +
                            (mat1->col2.x * mat2->col4.y) + 
                            (mat1->col3.x * mat2->col4.z) +
                            (mat1->col4.x * mat2->col4.w);

    // Row 2
    productMatrix->col1.y = (mat1->col1.y * mat2->col1.x) +
                            (mat1->col2.y * mat2->col1.y) + 
                            (mat1->col3.y * mat2->col1.z) +
                            (mat1->col4.y * mat2->col1.w);

    productMatrix->col2.y = (mat1->col1.y * mat2->col2.x) +
                            (mat1->col2.y * mat2->col2.y) + 
                            (mat1->col3.y * mat2->col2.z) +
                            (mat1->col4.y * mat2->col2.w);

    productMatrix->col3.y = (mat1->col1.y * mat2->col3.x) +
                            (mat1->col2.y * mat2->col3.y) + 
                            (mat1->col3.y * mat2->col3.z) +
                            (mat1->col4.y * mat2->col3.w);

    productMatrix->col4.y = (mat1->col1.y * mat2->col4.x) +
                            (mat1->col2.y * mat2->col4.y) + 
                            (mat1->col3.y * mat2->col4.z) +
                            (mat1->col4.y * mat2->col4.w);

    // Row 3
    productMatrix->col1.z = (mat1->col1.z * mat2->col1.x) +
                            (mat1->col2.z * mat2->col1.y) + 
                            (mat1->col3.z * mat2->col1.z) +
                            (mat1->col4.z * mat2->col1.w);

    productMatrix->col2.z = (mat1->col1.z * mat2->col2.x) +
                            (mat1->col2.z * mat2->col2.y) + 
                            (mat1->col3.z * mat2->col2.z) +
                            (mat1->col4.z * mat2->col2.w);

    productMatrix->col3.z = (mat1->col1.z * mat2->col3.x) +
                            (mat1->col2.z * mat2->col3.y) + 
                            (mat1->col3.z * mat2->col3.z) +
                            (mat1->col4.z * mat2->col3.w);

    productMatrix->col4.z = (mat1->col1.z * mat2->col4.x) +
                            (mat1->col2.z * mat2->col4.y) + 
                            (mat1->col3.z * mat2->col4.z) +
                            (mat1->col4.z * mat2->col4.w);

    // Row 4
    productMatrix->col1.w = (mat1->col1.w * mat2->col1.x) +
                            (mat1->col2.w * mat2->col1.y) + 
                            (mat1->col3.w * mat2->col1.z) +
                            (mat1->col4.w * mat2->col1.w);

    productMatrix->col2.w = (mat1->col1.w * mat2->col2.x) +
                            (mat1->col2.w * mat2->col2.y) + 
                            (mat1->col3.w * mat2->col2.z) +
                            (mat1->col4.w * mat2->col2.w);

    productMatrix->col3.w = (mat1->col1.w * mat2->col3.x) +
                            (mat1->col2.w * mat2->col3.y) + 
                            (mat1->col3.w * mat2->col3.z) +
                            (mat1->col4.w * mat2->col3.w);

    productMatrix->col4.w = (mat1->col1.w * mat2->col4.x) +
                            (mat1->col2.w * mat2->col4.y) + 
                            (mat1->col3.w * mat2->col4.z) +
                            (mat1->col4.w * mat2->col4.w);  

    return productMatrix;
}

/*
Function:
Inputs:
Outputs:
*/
Mat4* inverseMatrix(Mat4 *mat1, Mat4 *inverse)
{
    /*
    inverse of matrix A = (1/determinant(A)) * adjoin(A)
    */

    // Calculate determinant
    float determinant = (mat1->col1.x * mat1->col2.y * mat1->col3.z * mat1->col4.w) +
                        (mat1->col1.x * mat1->col3.y * mat1->col4.z * mat1->col2.w) +
                        (mat1->col1.x * mat1->col4.y * mat1->col2.z * mat1->col3.w) +
                        (mat1->col2.x * mat1->col1.y * mat1->col4.z * mat1->col3.w) +
                        (mat1->col2.x * mat1->col3.y * mat1->col1.z * mat1->col4.w) +
                        (mat1->col2.x * mat1->col4.y * mat1->col3.z * mat1->col1.w) +
                        (mat1->col3.x * mat1->col1.y * mat1->col2.z * mat1->col4.w) +
                        (mat1->col3.x * mat1->col2.y * mat1->col4.z * mat1->col1.w) +
                        (mat1->col3.x * mat1->col4.y * mat1->col1.z * mat1->col2.w) +
                        (mat1->col4.x * mat1->col1.y * mat1->col3.z * mat1->col2.w) +
                        (mat1->col4.x * mat1->col2.y * mat1->col1.z * mat1->col3.w) +
                        (mat1->col4.x * mat1->col3.y * mat1->col2.z * mat1->col1.w) -
                        (mat1->col1.x * mat1->col2.y * mat1->col4.z * mat1->col3.w) -
                        (mat1->col1.x * mat1->col3.y * mat1->col2.z * mat1->col4.w) -
                        (mat1->col1.x * mat1->col4.y * mat1->col3.z * mat1->col2.w) -
                        (mat1->col2.x * mat1->col1.y * mat1->col3.z * mat1->col4.w) -
                        (mat1->col2.x * mat1->col3.y * mat1->col4.z * mat1->col1.w) -
                        (mat1->col2.x * mat1->col4.y * mat1->col1.z * mat1->col3.w) -
                        (mat1->col3.x * mat1->col1.y * mat1->col4.z * mat1->col2.w) -
                        (mat1->col3.x * mat1->col2.y * mat1->col1.z * mat1->col4.w) -
                        (mat1->col3.x * mat1->col4.y * mat1->col2.z * mat1->col1.w) -
                        (mat1->col4.x * mat1->col1.y * mat1->col2.z * mat1->col3.w) -
                        (mat1->col4.x * mat1->col2.y * mat1->col3.z * mat1->col1.w) -
                        (mat1->col4.x * mat1->col3.y * mat1->col1.z * mat1->col2.w);
    
    // If determinant is 0, inverse of matrix does not exist.
    if(determinant == 0)
    {
        printf("Inverse of matrix does not exist because determinant = 0.\n");
        return inverse;
    }
    
    // Calculate adjoin
    Mat4 adjoin = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    adjoin.col1.x = (mat1->col2.y * mat1->col3.z * mat1->col4.w) + 
                    (mat1->col3.y * mat1->col4.z * mat1->col2.w) +
                    (mat1->col4.y * mat1->col2.z * mat1->col3.w) -
                    (mat1->col2.y * mat1->col4.z * mat1->col3.w) -
                    (mat1->col3.y * mat1->col2.z * mat1->col4.w) -
                    (mat1->col4.y * mat1->col3.z * mat1->col2.w);
    adjoin.col2.x = (mat1->col2.x * mat1->col4.z * mat1->col3.w) + 
                    (mat1->col3.x * mat1->col2.z * mat1->col4.w) +
                    (mat1->col4.x * mat1->col3.z * mat1->col2.w) -
                    (mat1->col2.x * mat1->col3.z * mat1->col4.w) -
                    (mat1->col3.x * mat1->col4.z * mat1->col2.w) -
                    (mat1->col4.x * mat1->col2.z * mat1->col3.w);
    adjoin.col3.x = (mat1->col2.x * mat1->col3.y * mat1->col4.w) + 
                    (mat1->col3.x * mat1->col4.y * mat1->col2.w) +
                    (mat1->col4.x * mat1->col2.y * mat1->col3.w) -
                    (mat1->col2.x * mat1->col4.y * mat1->col3.w) -
                    (mat1->col3.x * mat1->col2.y * mat1->col4.w) -
                    (mat1->col4.x * mat1->col3.y * mat1->col2.w);                
    adjoin.col4.x = (mat1->col2.x * mat1->col4.y * mat1->col3.z) + 
                    (mat1->col3.x * mat1->col2.y * mat1->col4.z) +
                    (mat1->col4.x * mat1->col3.y * mat1->col2.z) -
                    (mat1->col2.x * mat1->col3.y * mat1->col4.z) -
                    (mat1->col3.x * mat1->col4.y * mat1->col2.z) -
                    (mat1->col4.x * mat1->col2.y * mat1->col3.z);
    adjoin.col1.y = (mat1->col1.y * mat1->col4.z * mat1->col3.w) + 
                    (mat1->col3.y * mat1->col1.z * mat1->col4.w) +
                    (mat1->col4.y * mat1->col3.z * mat1->col1.w) -
                    (mat1->col1.y * mat1->col3.z * mat1->col4.w) -
                    (mat1->col3.y * mat1->col4.z * mat1->col1.w) -
                    (mat1->col4.y * mat1->col1.z * mat1->col3.w);
    adjoin.col2.y = (mat1->col1.x * mat1->col3.z * mat1->col4.w) + 
                    (mat1->col3.x * mat1->col4.z * mat1->col1.w) +
                    (mat1->col4.x * mat1->col1.z * mat1->col3.w) -
                    (mat1->col1.x * mat1->col4.z * mat1->col3.w) -
                    (mat1->col3.x * mat1->col1.z * mat1->col4.w) -
                    (mat1->col4.x * mat1->col3.z * mat1->col1.w);
    adjoin.col3.y = (mat1->col1.x * mat1->col4.y * mat1->col3.w) + 
                    (mat1->col3.x * mat1->col1.y * mat1->col4.w) +
                    (mat1->col4.x * mat1->col3.y * mat1->col1.w) -
                    (mat1->col1.x * mat1->col3.y * mat1->col4.w) -
                    (mat1->col3.x * mat1->col4.y * mat1->col1.w) -
                    (mat1->col4.x * mat1->col1.y * mat1->col3.w);
    adjoin.col4.y = (mat1->col1.x * mat1->col3.y * mat1->col4.z) + 
                    (mat1->col3.x * mat1->col4.y * mat1->col1.z) +
                    (mat1->col4.x * mat1->col1.y * mat1->col3.z) -
                    (mat1->col1.x * mat1->col4.y * mat1->col3.z) -
                    (mat1->col3.x * mat1->col1.y * mat1->col4.z) -
                    (mat1->col4.x * mat1->col3.y * mat1->col1.z);
    adjoin.col1.z = (mat1->col1.y * mat1->col2.z * mat1->col4.w) + 
                    (mat1->col2.y * mat1->col4.z * mat1->col1.w) +
                    (mat1->col4.y * mat1->col1.z * mat1->col2.w) -
                    (mat1->col1.y * mat1->col4.z * mat1->col2.w) -
                    (mat1->col2.y * mat1->col1.z * mat1->col4.w) -
                    (mat1->col4.y * mat1->col2.z * mat1->col1.w);
    adjoin.col2.z = (mat1->col1.x * mat1->col4.z * mat1->col2.w) + 
                    (mat1->col2.x * mat1->col1.z * mat1->col4.w) +
                    (mat1->col4.x * mat1->col2.z * mat1->col1.w) -
                    (mat1->col1.x * mat1->col2.z * mat1->col4.w) -
                    (mat1->col2.x * mat1->col4.z * mat1->col1.w) -
                    (mat1->col4.x * mat1->col1.z * mat1->col2.w);
    adjoin.col3.z = (mat1->col1.x * mat1->col2.y * mat1->col4.w) + 
                    (mat1->col2.x * mat1->col4.y * mat1->col1.w) +
                    (mat1->col4.x * mat1->col1.y * mat1->col2.w) -
                    (mat1->col1.x * mat1->col4.y * mat1->col2.w) -
                    (mat1->col2.x * mat1->col1.y * mat1->col4.w) -
                    (mat1->col4.x * mat1->col2.y * mat1->col1.w);
    adjoin.col4.z = (mat1->col1.x * mat1->col4.y * mat1->col2.z) + 
                    (mat1->col2.x * mat1->col1.y * mat1->col4.z) +
                    (mat1->col4.x * mat1->col2.y * mat1->col1.z) -
                    (mat1->col1.x * mat1->col2.y * mat1->col4.z) -
                    (mat1->col2.x * mat1->col4.y * mat1->col1.z) -
                    (mat1->col4.x * mat1->col1.y * mat1->col2.z);
    adjoin.col1.w = (mat1->col1.y * mat1->col3.z * mat1->col2.w) + 
                    (mat1->col2.y * mat1->col1.z * mat1->col3.w) +
                    (mat1->col3.y * mat1->col2.z * mat1->col1.w) -
                    (mat1->col1.y * mat1->col2.z * mat1->col3.w) -
                    (mat1->col2.y * mat1->col3.z * mat1->col1.w) -
                    (mat1->col3.y * mat1->col1.z * mat1->col2.w);
    adjoin.col2.w = (mat1->col1.x * mat1->col2.z * mat1->col3.w) + 
                    (mat1->col2.x * mat1->col3.z * mat1->col1.w) +
                    (mat1->col3.x * mat1->col1.z * mat1->col2.w) -
                    (mat1->col1.x * mat1->col3.z * mat1->col2.w) -
                    (mat1->col2.x * mat1->col1.z * mat1->col3.w) -
                    (mat1->col3.x * mat1->col2.z * mat1->col1.w);
    adjoin.col3.w = (mat1->col1.x * mat1->col3.y * mat1->col2.w) + 
                    (mat1->col2.x * mat1->col1.y * mat1->col3.w) +
                    (mat1->col3.x * mat1->col2.y * mat1->col1.w) -
                    (mat1->col1.x * mat1->col2.y * mat1->col3.w) -
                    (mat1->col2.x * mat1->col3.y * mat1->col1.w) -
                    (mat1->col3.x * mat1->col1.y * mat1->col2.w);
    adjoin.col4.w = (mat1->col1.x * mat1->col2.y * mat1->col3.z) + 
                    (mat1->col2.x * mat1->col3.y * mat1->col1.z) +
                    (mat1->col3.x * mat1->col1.y * mat1->col2.z) -
                    (mat1->col1.x * mat1->col3.y * mat1->col2.z) -
                    (mat1->col2.x * mat1->col1.y * mat1->col3.z) -
                    (mat1->col3.x * mat1->col2.y * mat1->col1.z);
    
    inverse = scalarMultMatrix((1/determinant),&adjoin,inverse);

    return inverse;

}

/*
Function:
     This function transposes a matrix.
Inputs:
     mat1 --> matrix to be transposed
     transpose --> pointer where transpose will be placed
Outputs:
     transpose --> transposed matrix
*/
Mat4* transposeMatrix(Mat4 *mat1, Mat4 *transpose)
{
    /*
     | A1 | B1 | C1 | D1 |T        | A1 | A2 | A3 | A4 |
     | A2 | B2 | C2 | D2 |         | B1 | B2 | B3 | B4 |
     | A3 | B3 | C3 | D3 |    =    | C1 | C2 | C3 | C4 |
     | A4 | B4 | C4 | D4 |         | D1 | D2 | D3 | D4 |
     */
    
    transpose->col1.x = mat1->col1.x;
    transpose->col2.x = mat1->col1.y;
    transpose->col3.x = mat1->col1.z;
    transpose->col4.x = mat1->col1.w;
    
    transpose->col1.y = mat1->col2.x;
    transpose->col2.y = mat1->col2.y;
    transpose->col3.y = mat1->col2.z;
    transpose->col4.y = mat1->col2.w;
    
    transpose->col1.z = mat1->col3.x;
    transpose->col2.z = mat1->col3.y;
    transpose->col3.z = mat1->col3.z;
    transpose->col4.z = mat1->col3.w;

    transpose->col1.w = mat1->col4.x;
    transpose->col2.w = mat1->col4.y;
    transpose->col3.w = mat1->col4.z;
    transpose->col4.w = mat1->col4.w;
    
    return transpose;
}

/*
 Function:
     This function applies a rotation matrix in the x direction
     to the current transformation matrix.
 Inputs:
     tr_matrix --> current transformation matrix
     theta --> angle of rotation about x axis
     productMatrix --> pointer to eventual location of altered tr_matrix
 Outputs:
     productMatrix --> tr_matrix with rotation about x applied
 */
Mat4* matRotateAboutX(float theta, Mat4 *productMatrix)
{
    float DegreesToRadians = M_PI / 180.0;
    float thetar = theta*DegreesToRadians;
    
    // Generate rotation matrix
    Mat4 rotationMatrix =
    {
        {1.0,0.0,0.0,0.0},
        {0.0, cos(thetar),sin(thetar),0.0},
        {0.0, -sin(thetar),cos(thetar),0.0},
        {0.0,0.0,0.0,1.0},
    };
    
    productMatrix = &rotationMatrix;
    
    return productMatrix;
}

/*
 Function:
     This function applies a rotation matrix in the y direction
     to the current transformation matrix.
 Inputs:
     tr_matrix --> current transformation matrix
     theta --> angle of rotation about y axis
     productMatrix --> pointer to eventual location of altered tr_matrix
 Outputs:
     productMatrix --> tr_matrix with rotation about y applied
 */
Mat4* matRotateAboutY(float theta, Mat4 *productMatrix)
{
    float DegreesToRadians = M_PI / 180.0;
    float thetar = theta*DegreesToRadians;
    
    
    // Generate rotation matrix
    Mat4 rotationMatrix =
    {
        {cos(thetar),0.0,-sin(thetar),0.0},
        {0.0,1.0,0.0,0.0},
        {sin(thetar),0.0,cos(thetar),0.0},
        {0.0,0.0,0.0,1.0},
    };
 
    productMatrix = &rotationMatrix;
    
    return productMatrix;
}

/*
 Function:
     This function applies a rotation matrix in the z direction
     to the current transformation matrix.
 Inputs:
     tr_matrix --> current transformation matrix
     theta --> angle of rotation about z axis
     productMatrix --> pointer to eventual location of altered tr_matrix
 Outputs:
     productMatrix --> tr_matrix with rotation about z applied
 */
Mat4* matRotateAboutZ(float theta, Mat4 *productMatrix)
{
    float DegreesToRadians = M_PI / 180.0;
    float thetar = theta*DegreesToRadians;
    
    
    // Generate rotation matrix
    Mat4 rotationMatrix =
    {
        {cos(thetar),sin(thetar),0.0,0.0},
        {-sin(thetar),cos(thetar),0.0,0.0},
        {0.0,0.0,1.0,0.0},
        {0.0,0.0,0.0,1.0},
    };
    
    
    productMatrix = &rotationMatrix;
    
    return productMatrix;
}

/*
 Function:
     This function scales the transofrmation matrix acording to the given scaling factor
 Inputs:
     tr_matrix --> transformation matrix
     scalingFactor --> factor to scale transformation matrix by
     productMatrix --> location to store resulting scaled matrix
 Outputs:
     productMatrix --> result of scaling matrix
 */
Mat4* scaleMatrix(Mat4 *tr_matrix, float scalingFactor, Mat4 *productMatrix)
{
    Mat4 scalingMatrix =
    {
        {scalingFactor, 0.0,0.0,0.0},
        {0.0, scalingFactor,0.0,0.0},
        {0.0,0.0,scalingFactor,0.0},
        {0.0,0.0,0.0,1.0}
    };
    
    productMatrix = matMultiplication(tr_matrix, &scalingMatrix, productMatrix);
    
    return productMatrix;
}









