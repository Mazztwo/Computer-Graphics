/*
LabAssignment1 Main
This file will contain the main() for 
Lab assignment 1.
*/

#include "funct.h"
#include <stdio.h>

int main()
{   
    Vec4 vector1 = {1.0,2.0,3.0,0.0};
    Vec4 vector2 = {3.0,2.0,3.0,0.0};
    Vec4 xProduct = {0,0,0,0};          // vec1 X vec2 = xProduct
    float scalarProduct = 0;            // vec1 dot vec2 = scalarProduct
    Vec4 scalarMultProduct = {0,0,0,0}; // C * vec1 = {C*x,C*y,C*z}
    Vec4 vecSum = {0,0,0,0};            // vec1 + vec2 = vecSum
    Vec4 vecSub = {0,0,0,0};            // vec - vec2 = vecSub

    // Test printVec4 
    printVec4(&vector1);
    printVec4(&vector2);
    //////////////////////////

    // Test crossProduct 
    xProduct = *crossProduct(&vector1, &vector2, &xProduct);
    printVec4(&xProduct);
    //////////////////////////

    // Test dotProduct 
    scalarProduct = dotProduct(&vector1, &vector2);
    printf("%f\n",scalarProduct);
    //////////////////////////

     // Test scalarMultVector
    scalarMultProduct = *scalarMultVector(5.0, &vector1,&scalarMultProduct);
    printVec4(&scalarMultProduct);    
    //////////////////////////

    // Test vec4addition
    vecSum = *vec4addition(&vector1, &vector2, &vecSum);
    printVec4(&vecSum);
    //////////////////////////

    // Test vec4subtraction
    vecSub = *vec4subtraction(&vector1, &vector2, &vecSub);
    printVec4(&vecSub);
    //////////////////////////
    

    return 0;
}

