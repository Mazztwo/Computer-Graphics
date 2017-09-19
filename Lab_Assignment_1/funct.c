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



