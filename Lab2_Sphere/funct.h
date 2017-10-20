/*
Alessio Mazzone
CS1566 Computer Graphics
University of Pittsburgh
 
 Header file for Lab Assignment 1
*/


#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>


typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
}Vec4;

typedef struct
{
    Vec4 col1;
    Vec4 col2;
    Vec4 col3;
    Vec4 col4;
}Mat4;

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vec3;



void printVec4(Vec4 *vectorToPrint);
Vec4* crossProduct(Vec4 *vec1, Vec4 *vec2, Vec4 *productVector);
GLfloat dotProduct(Vec4 *vec1, Vec4 *vec2);
Vec4* scalarMultVector(float scalar, Vec4 *vec, Vec4 *productVector);
Vec4* vec4addition(Vec4* vec1, Vec4* vec2, Vec4 *sumVec);
Vec4* vec4subtraction(Vec4* vec1, Vec4* vec2, Vec4 *subVec);

void printMat4(Mat4 *mat);
Mat4* scalarMultMatrix(float scalar, Mat4 *mat, Mat4 *productMatrix);
Mat4* mat4addition(Mat4 *mat1, Mat4 *mat2, Mat4 *sumMatrix);
Mat4* mat4subtraction(Mat4 *mat1, Mat4 *mat2, Mat4 *subMatrix);
Mat4* matMultiplication(Mat4 *mat1, Mat4 *mat2, Mat4 *productMatrix);
Mat4* inverseMatrix(Mat4 *mat1, Mat4 *inverse);
Mat4* transposeMatrix(Mat4 *mat1, Mat4 *transpose);
