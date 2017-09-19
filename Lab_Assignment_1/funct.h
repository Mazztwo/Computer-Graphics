/*
Header file for Lab Assignment 1
*/

// Define a vector, Vec4
typedef struct
{
    float x;
    float y; 
    float z;
    float w;
}Vec4;

void printVec4(Vec4 *ptr_vectorToPrint);
Vec4* crossProduct(Vec4 *vec1, Vec4 *vec2, Vec4 *productVector);
