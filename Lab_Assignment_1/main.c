/*
Alessio Mazzone
CS1566 Computer Graphics
University of Pittsburgh
 
Lab Assignment1 Main
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
    Mat4 matrix1 = {{1,2,3,4},{3,2,1,4},{4,5,6,5},{0,0,0,0}};
    Mat4 matrix2 = {{2,2,2,2},{1,1,2,0},{1,1,1,1},{0,0,0,0}};
    Mat4 scalarProdMatrix = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Mat4 matSum = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Mat4 matSub = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Mat4 matProd = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Mat4 matrix3 = {{1,0,2,1},{1,3,3,0},{1,1,1,2},{0,2,0,1}};
    Mat4 inverse = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Mat4 transpose = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

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

    // Test printMat4
    printMat4(&matrix1);
    //////////////////////////
    
    // Test scalarMultMatrix
    scalarProdMatrix = *scalarMultMatrix(3,&matrix1,&scalarProdMatrix);
    printf("\n");
    printMat4(&scalarProdMatrix);
    //////////////////////////
    
    // Test mat4addition
    matSum = *mat4addition(&matrix1,&matrix2,&matSum);
    printf("\n");
    printMat4(&matSum);
    //////////////////////////

    // Test mat4subtraction
    matSub = *mat4subtraction(&matrix1,&matrix2,&matSub);
    printf("\n");
    printMat4(&matSub);
    //////////////////////////

    // Test matMultiplication
    matProd = *matMultiplication(&matrix1,&matrix2,&matProd);
    printf("\n");
    printMat4(&matProd);
    //////////////////////////

    // Test inverseMatrix
    printf("\n");
    inverse = *inverseMatrix(&matrix1,&inverse);
    printf("\n");
    inverse = *inverseMatrix(&matrix2,&inverse);
    printf("\n");
    inverse = *inverseMatrix(&matrix3,&inverse);
    printf("\n");
    printMat4(&inverse);
    //////////////////////////
    
    // Test transposeMatrix
    transpose = *transposeMatrix(&matrix3,&transpose);
    printf("\n");
    printMat4(&transpose);
    //////////////////////////

    return 0;
}

