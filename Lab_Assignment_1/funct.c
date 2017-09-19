/*
Lab Assignment 1
This file will contain all the implementations of the 
functions required of the Lab Assignment: 1 
instruction pdf.
 */


#include "funct.h"
#include <stdio.h>


/*
Function:
    This method will derefrence a vec4 pointer
    and print its x,y,z, and w components.
Inputs: 
    ptr_vectorToPrint --> Contains a
        pointer to a vec4 struc to 
        print to terminal.
Outputs:
    None
*/
void printVec4(Vec4 *ptr_vectorToPrint)
{   
    // Declare vec4 components and 
    // extract from vector
    float xComp, yComp, zComp, wComp;
    
    xComp = ptr_vectorToPrint->x;
    yComp = ptr_vectorToPrint->y;
    zComp = ptr_vectorToPrint->z;
    wComp = ptr_vectorToPrint->w;
    
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
    B = {b1, b2, b3}

    A X B = {a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1}  
    */
    
    // Calculate x,y,z of product, set w to 0.
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
Inputs:
Outputs:
*/













