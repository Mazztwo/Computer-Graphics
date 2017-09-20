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

typedef struct
{
    Vec4 x;
    Vec4 y;
    Vec4 z;
    Vec4 w;
}Mat4;


void printVec4(Vec4 *vectorToPrint);
Vec4* crossProduct(Vec4 *vec1, Vec4 *vec2, Vec4 *productVector);
float dotProduct(Vec4 *vec1, Vec4 *vec2);
Vec4* scalarMultVector(float scalar, Vec4 *vec, Vec4 *productVector);
Vec4* vec4addition(Vec4* vec1, Vec4* vec2, Vec4 *sumVec);
Vec4* vec4subtraction(Vec4* vec1, Vec4* vec2, Vec4 *subVec);

void printMat4(Mat4 *mat);